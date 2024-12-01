.gba
.thumb
.open "./BPRE0.gba","./multi.gba", 0x08000000
.loadtable "./charmap.txt"


.org 0x08058A02
.thumb
.align 2
LDR r6, = hijack|1
BX r6
.pool

.org freespace
.importobj "./build/linked.o"


.thumb
.align 2
hijack:
    push{r0-r4}
    LDR r6, =wfc_entrypoint|1
    BL goto_r6
    pop{r0-r4}
back_to_normal:
    SUB sp, sp, #4
    MOV r6, r0
    LDR r5, [r6]
    LDR R0, =0x3FF03FF
    STR R0, [SP]
    LDR r4, =0x08058A0C|1
    BX r4

goto_r6:
    BX r6

sqrt:
    SWI 0x8
    BX LR

.pool
