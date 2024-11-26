
#ifndef WFC_C
#define WFC_C

#include "wfc.h"

u32 get_probs(struct Wfc wfc, u8 x, u8 y){
    //dprintf("x %x y %x res %x\n", x, y, wfc.probs[x + y * wfc.width]);
    return wfc.probs[x + y * wfc.width];
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


struct Wfc init(u8 width, u8 height){
    //alloc output, probs, prob_calc_ctr.
    u32 outputSize = sizeof(struct MapTile)*width*height;
    u32 probsSize = sizeof(u32)*width*height;
    u32 probsCalcCtrSize = sizeof(bool)*width*height;
    u8 *wfcDataPtr = malloc(outputSize + probsSize + probsCalcCtrSize);
    void *wfcOuputPtr = wfcDataPtr;
    u32 *wfcProbsPtr = (u32 *) ((u8 *)wfcDataPtr+outputSize);
    void *wfcProbsCalcCtrPtr = ((u8 *)wfcProbsPtr)+probsSize;

    dprintf("asd %x\n", wfcOuputPtr);

    memset(wfcOuputPtr, 0xFF, outputSize);
    for(u8 i=0; i<width; i++){
        for(u8 j=0; j<height; j++){
            wfcProbsPtr[i + j * width]=tileset_allow_all_tiles;
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

void recalc_prob(struct Wfc wfc, u8 x, u8 y){
    if(x<0 || y<0 || x>=wfc.width || y>=wfc.height || get_prob_calc_ctr(wfc, x, y)){
        return;
    }
    set_prob_calc_ctr(wfc, x, y, true);
    if(!is_output_empty(get_output(wfc, x, y))){
        recalc_prob(wfc, x-1, y  );
        recalc_prob(wfc, x+1, y  );
        recalc_prob(wfc, x  , y-1);
        recalc_prob(wfc, x  , y+1);
        return;
    }
    u32 l = 0;
    u32 r = 0;
    u32 u = 0;
    u32 d = 0;
    if(x>0)           {l=get_probs(wfc, x-1, y  );}
    if(x<wfc.width-1) {r=get_probs(wfc, x+1, y  );}
    if(y>0)           {u=get_probs(wfc, x  , y-1);}
    if(y<wfc.height-1){d=get_probs(wfc, x  , y+1);}

    u32 l_allowed_tiles = l == 0 ? tileset_allow_all_tiles : sum_left_rules_for_tileids(l);
    u32 r_allowed_tiles = r == 0 ? tileset_allow_all_tiles : sum_right_rules_for_tileids(r);
    u32 u_allowed_tiles = u == 0 ? tileset_allow_all_tiles : sum_up_rules_for_tileids(u);
    u32 d_allowed_tiles = d == 0 ? tileset_allow_all_tiles : sum_down_rules_for_tileids(d);

    u32 probs = get_probs(wfc, x, y);
    probs = probs & l_allowed_tiles;
    probs = probs & r_allowed_tiles;
    probs = probs & u_allowed_tiles;
    probs = probs & d_allowed_tiles;
    
    
    if(probs == 0){
        //debugger;
        //throw new Error('Collision! Tile '+x+', '+y+' has 0 possibilities (was '+this.output_probs[x][y]+'n)');
    }
    set_probs(wfc, x, y, probs);

    if(count_bits(probs) != count_bits(tileset_allow_all_tiles)){
        recalc_prob(wfc, x-1, y  );
        recalc_prob(wfc, x+1, y  );
        recalc_prob(wfc, x  , y-1);
        recalc_prob(wfc, x  , y+1);
    }
}   

void start_recalc_prob(struct Wfc wfc, u8 x, u8 y){
    //propagate changes to nearby nodes
    recalc_prob(wfc, x, y);

    //reset prob_calced_ctr
    for(u8 i=0; i<wfc.width; i++){
        for(u8 j=0; j<wfc.height; j++){
            set_prob_calc_ctr(wfc, i, j, false);
        }
    }
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
    start_recalc_prob(wfc, x, y);
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
    start_recalc_prob(wfc, x, y);
}

void print(struct Wfc wfc){
    for(u8 i=0; i<wfc.width; i++){
        for(u8 j=0; j<wfc.height; j++){
            dprintf("%x  ", get_output(wfc, i, j));
        }
    }
}
//returns 0 if succesful, 1 otherwise
u8 start(struct Wfc wfc){
    struct Coords8 coords = find_lowest_entropy_cell(wfc);
    u8 x = coords.x;
    u8 y = coords.y;
    do{
        dprintf("collpasing %x,%x\n", x,y);
        u8 failed = observe(wfc, x, y);
        if(failed == 1){
            return 1;
        }
        coords = find_lowest_entropy_cell(wfc);
        x = coords.x;
        y = coords.y;
    } while(x != 0xFF && y != 0xFF);

    print(wfc);

    return 0;
}


#endif