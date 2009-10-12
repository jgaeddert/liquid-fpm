//
//
//

#include <stdio.h>

void qtype_mul_ppc(const char * _name,
                   unsigned int _intbits,
                   unsigned int _fracbits)
{
    printf(";    .section __TEXT,__text,regular,pure_instructions\n");
    printf(";    .section __TEXT,__picsymbolstub1,symbol_stubs,pure_instructions,32\n");
    printf(";    .machine ppc7400\n");
    printf("    .text\n");
    printf(";    .align 2\n");
    printf(";    .p2align 4,,15\n");
    printf("    .globl _%s_mul\n", _name);
    printf("_%s_mul:\n", _name);
    printf("    ; perform actual computation\n");
    printf("    mullw   r11,r3,r4   ; multiply lo-order word, store in r11\n");
    printf("    mulhw   r12,r3,r4   ; multiply hi-order word, store in r12\n");
    printf("\n");
    printf("    ;rlmi    ...    ; only available for POWER architectures\n");
    printf("\n");
    printf("    srwi    r5,r11,%-2u   ; shift lower >> by %2u\n", _fracbits, _fracbits);
    printf("    ;andi.   r5,r5,0x0f  ; mask with 0x0f\n");
    printf("\n");
    printf("    slwi    r6,r12,%-2u   ; shift upper << by %u\n", _intbits, _intbits);
    printf("\n");
    printf("    or      r3,r5,r6    ; mask the upper with the lower\n");
    printf("\n");
    printf("    blr                 ; return from routine\n");
    printf("    .subsections_via_symbols\n");
}

int main()
{
    qtype_mul_ppc("q32",4,28);
    return 0;
}
