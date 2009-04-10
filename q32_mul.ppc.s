;	.section __TEXT,__text,regular,pure_instructions
;	.section __TEXT,__picsymbolstub1,symbol_stubs,pure_instructions,32
;	.machine ppc7400
	.text
;	.align 2
;	.p2align 4,,15
	.globl _q32_mul
_q32_mul:
    ; perform actual computation
    mullw   r11,r3,r4   ; multiply lo-order word, store in r11
    mulhw   r12,r3,r4   ; multiply hi-order word, store in r12

    ;rlmi    ...    ; only available for POWER architectures

    srwi    r5,r11,28   ; shift lower >> by 28
    ;andi.   r5,r5,0x0f  ; mask with 0x0f

    slwi    r6,r12,4    ; shift upper << by 4

    or      r3,r5,r6    ; mask the upper with the lower

    blr                 ; return from routine
    .subsections_via_symbols
