;    .globl _q32_mul
;_q32_mul:
    ; perform actual computation
    mullw   r11,r3,r4   ; multiply lo-order word, store in r11
    mulhw   r12,r3,r4   ; multiply hi-order word, store in r12

    ;rlmi    ...    ; only available for POWER architectures

    srwi    r5,r11,25   ; shift lower >> by 25
    ;andi.   r5,r5,0x0f  ; mask with 0x0f

    slwi    r6,r12,7    ; shift upper << by 7

    or      r3,r5,r6    ; mask the upper with the lower

    blr                 ; return from routine
    .subsections_via_symbols
