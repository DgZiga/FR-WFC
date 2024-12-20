
#ifndef WFC_C
#define WFC_C

#include "wfc.h"

u32 get_probs(struct Wfc wfc, u8 x, u8 y){
    return (u32)wfc.probs[x + y * wfc.width];
}
struct MapTile get_output(struct Wfc wfc, u8 x, u8 y){
    return wfc.output[x + y * wfc.width];
}
bool get_prob_calc_ctr(struct Wfc wfc, u8 x, u8 y){
    return wfc.prob_calc_ctr[x + y * wfc.width];
}
void set_prob_calc_ctr(struct Wfc wfc, u8 x, u8 y, bool val){
    wfc.prob_calc_ctr[x + y * wfc.width] = val;
}
void set_output(struct Wfc wfc, u8 x, u8 y, struct MapTile val){
    wfc.output[x + y * wfc.width] = val;
}
void set_probs(struct Wfc wfc, u8 x, u8 y, u32 val){
    wfc.probs[x + y * wfc.width] = val;
}

const struct MapTile EMPTY_OUTPUT = {.tile=1023, .permission=63};//equal to 0xFFFF
bool is_output_empty(struct MapTile output){
    return output.permission == EMPTY_OUTPUT.permission && output.tile == EMPTY_OUTPUT.tile;
}

void print(struct Wfc wfc){
    for(u8 j=0; j<wfc.height; j++){
        if(j==0){dprintf("\t");}
        dprintf("%x\t", j);
    }
    dprintf("\n");
    for(u8 i=0; i<wfc.width; i++){
        dprintf("%x\t",i);
        for(u8 j=0; j<wfc.height; j++){
            dprintf("%x\t", get_probs(wfc, j, i));
        }
        dprintf("\n");
    }
}


struct Wfc init(u8 width, u8 height){
    //alloc output, probs, prob_calc_ctr.
    u32 outputSize = sizeof(struct MapTile)*width*height;
    u32 probsSize = sizeof(u32)*width*height;
    u32 probsCalcCtrSize = sizeof(bool)*width*height;
    //align to 4
    probsSize        += probsSize % 4;
    outputSize       += outputSize % 4; 
    probsCalcCtrSize += probsCalcCtrSize % 4; 

    u8 *wfcDataPtr = malloc(outputSize + probsSize + probsCalcCtrSize);
    void *wfcOuputPtr = wfcDataPtr;
    u32 *wfcProbsPtr = (u32 *) ((u8 *)wfcDataPtr+outputSize);
    void *wfcProbsCalcCtrPtr = ((u8 *)wfcProbsPtr)+probsSize;

    //dprintf("out: %x, probs: %x, calcPtr: %x\n", wfcOuputPtr, wfcProbsPtr, wfcProbsCalcCtrPtr);

    memset(wfcOuputPtr, 0xFF, outputSize);
    for(u8 i=0; i<width; i++){
        for(u8 j=0; j<height; j++){
            wfcProbsPtr[j + i * width] = tileset_non_walkable_superpos;
        }
    }
    memset(wfcProbsCalcCtrPtr, 0, probsCalcCtrSize);

    struct Wfc wfc = {.addr=wfcDataPtr, .height=height, .width=width, .output=wfcOuputPtr, .probs=wfcProbsPtr, .prob_calc_ctr=wfcProbsCalcCtrPtr};
    return wfc;
}

struct Coords8 find_lowest_entropy_cell(struct Wfc wfc){
    u8 lowestI = 0xFF;
    u8 lowestJ = 0xFF;
    u8 lowestCnt = 0xFF;
    for(u8 i=0; i<wfc.width; i++){
        for(u8 j=0; j<wfc.height; j++){
            u8 bitsNo=count_bits(get_probs(wfc, i, j));
            //dprintf("comparing i:%x, j:%x, prob:%x, bitsNo: %x, lowestCnt: %x\n", i, j, get_probs(wfc, i, j), bitsNo, lowestCnt);
            if(is_output_empty(get_output(wfc, i, j)) && bitsNo<lowestCnt){
                lowestCnt = bitsNo;
                lowestI = i;
                lowestJ = j;
            }
        }
    }
    struct Coords8 coords = {.x=lowestI, .y=lowestJ};
    return coords;
}


struct Position {
    u8 x;
    u8 y;
};

void recalc_prob_iterative(struct Wfc wfc, u8 start_x, u8 start_y) {
    // Stack for positions to process
    u8 pad = 30;
    struct Position *stack = (struct Position *)malloc((wfc.width+pad) * (wfc.height+pad) * sizeof(struct Position));
    if (!stack) {
        // Memory allocation failed
        //dprintf("malloc failed");
        return;
    }
    int top = -1; // Stack pointer

    // Push the starting position onto the stack
    stack[++top] = (struct Position){start_x, start_y};

    //reset prob_calced_ctr
    for(u8 i=0; i<wfc.width; i++){
        for(u8 j=0; j<wfc.height; j++){
            set_prob_calc_ctr(wfc, i, j, false);
        }
    }

    while (top >= 0) {
        // Pop the current position from the stack
        //dprintf("%x  ", top);
        struct Position current = stack[top--];
        u8 x = current.x;
        u8 y = current.y;

        //dprintf("calcing from %x %x\n", x, y);
        if (x >= wfc.width || y >= wfc.height || get_prob_calc_ctr(wfc, x, y)) {
            continue;
        }
        set_prob_calc_ctr(wfc, x, y, true);

        if (!is_output_empty(get_output(wfc, x, y))) {
            // Add neighboring positions to the stack
            if (x > 0)           stack[++top] = (struct Position){x - 1, y};
            if (x < wfc.width-1) stack[++top] = (struct Position){x + 1, y};
            if (y > 0)           stack[++top] = (struct Position){x, y - 1};
            if (y < wfc.height-1)stack[++top] = (struct Position){x, y + 1};
            continue;
        }

        u32 l = 0, r = 0, u = 0, d = 0;
        if (x > 0)            l = get_probs(wfc, x - 1, y);
        if (x < wfc.width-1)  r = get_probs(wfc, x + 1, y);
        if (y > 0)            u = get_probs(wfc, x, y - 1);
        if (y < wfc.height-1) d = get_probs(wfc, x, y + 1);

        u32 l_allowed_tiles = l == 0 ? tileset_allow_all_tiles : sum_right_rules_for_tileids(l);
        u32 r_allowed_tiles = r == 0 ? tileset_allow_all_tiles : sum_left_rules_for_tileids(r);
        u32 u_allowed_tiles = u == 0 ? tileset_allow_all_tiles : sum_down_rules_for_tileids(u);
        u32 d_allowed_tiles = d == 0 ? tileset_allow_all_tiles : sum_up_rules_for_tileids(d);

        u32 probs = get_probs(wfc, x, y);
        probs &= l_allowed_tiles & r_allowed_tiles & u_allowed_tiles & d_allowed_tiles;

        if (probs == 0) {
            dprintf("contraddiction about to happen at %x %x \n", x, y);
            print(wfc);
            // Handle collision
            // e.g., debug or log the error
        }

        set_probs(wfc, x, y, probs);

        if (count_bits(probs) != count_bits(tileset_allow_all_tiles)) {
            // Add neighboring positions to the stack
            if (x > 0)           stack[++top] = (struct Position){x - 1, y};
            if (x < wfc.width-1) stack[++top] = (struct Position){x + 1, y};
            if (y > 0)           stack[++top] = (struct Position){x, y - 1};
            if (y < wfc.height-1)stack[++top] = (struct Position){x, y + 1};
        }
    }

    // Free allocated stack memory
    free(stack);
}

//returns 0 if succesful, 1 otherwise
u8 observe(struct Wfc wfc, u8 x, u8 y){
    u32 probs = get_probs(wfc, x, y);
    if(probs == 0){
        return 1;
    }
    u16 rands = (u16)((u32)rand() * (u32)count_bits(probs) / (u32) 0xFFFF);
    u8 tileId = get_nth_set_bit(probs, rands);
    set_output(wfc, x, y, tileId_to_mapTile[tileId]);
    set_probs(wfc, x, y, 1 << tileId);

    //propagate changes to nearby nodes
    recalc_prob_iterative(wfc, x, y);
    return 0;
}

void observe_forced(struct Wfc wfc, u8 x, u8 y, u8 forcedVal){
    u32 probs = get_probs(wfc, x, y);
    if(probs == 0){
        //TODO: handle error
        return;
    }
    u8 tileId = forcedVal;
    set_output(wfc, x, y, tileId_to_mapTile[tileId]);
    set_probs(wfc, x, y, 1 << tileId);

    //propagate changes to nearby nodes
    recalc_prob_iterative(wfc, x, y);
}
//returns 0 if succesful, 1 otherwise
u8 start(struct Wfc wfc){
    struct Coords8 coords = find_lowest_entropy_cell(wfc);
    u8 x = coords.x;
    u8 y = coords.y;
    //print(wfc);
    do{
        //dprintf("\n\n\n");
        u8 failed = observe(wfc, x, y);
        if(failed == 1){
            return 1;
        }
        coords = find_lowest_entropy_cell(wfc);
        x = coords.x;
        y = coords.y;
    //} while(false);
    } while(x != 0xFF && y != 0xFF);

    return 0;
}


#endif