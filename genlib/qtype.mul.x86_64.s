# q32_t q32_mul(q32_t _x, q32_t _y)
#
# multiplies 2 q32_t fixed-point numbers
#

# .text
# .set  qint,   7
# .set  qfrac,  25
# .globl q32_mul
# q32_mul:

    pushq   %rbp            # create the stack frame
    movq    %rsp, %rbp

    # grab the two 4-byte arguments 
    movl    %edi, -4(%rbp)
    movl    %esi, -8(%rbp)
    movl    -4(%rbp), %eax
    movl    -8(%rbp), %ecx

    # multiply arguments (signed long) with result stored
    # in %edx:%eax
    imull   %ecx

    # Now we need to shift the 64-bit number %edx:%eax to the
    # right by literal $qfrac (FRACBITS). This can be easily
    # accomplished using the shrd instruction

    mov     $qfrac, %cl     # set shift amount in %cl
    shrdl   %edx,%eax       # shift right double, store in %eax

    # return from routine
    leave
    ret

