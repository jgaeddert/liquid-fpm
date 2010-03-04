;    .globl _q32_mul
;_q32_mul:
    ; perform actual computation
    mullw   r11,r3,r4   ; multiply lo-order word, store in r11
    mulhw   r12,r3,r4   ; multiply hi-order word, store in r12

    ;rlmi    ...    ; only available for POWER architectures

    srwi    r5,r11,qfrac ; shift lower >> by qfrac (FRACBITS)
    slwi    r6,r12,qint  ; shift upper << by qint  (INTBITS)

    or      r3,r5,r6    ; mask the upper with the lower

    blr                 ; return from routine
    .subsections_via_symbols
