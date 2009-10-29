# q32_t q32_mul(q32_t _x, q32_t _y)
#
# multiplies 2 q32_t fixed-point numbers
#

.text
.globl q32_mul
q32_mul:
    pushl   %ebp            # create the stack frame
    movl    %esp, %ebp

    movl    12(%ebp),%eax   # grab the first argument
    movl    8(%ebp),%ecx    # grab the second argument
    imull   %ecx            # multiply arguments (signed long)
                            # with result stored in %edx:%eax

    # Now we need to shift the 64-bit number %edx:%eax to the
    # right by 25 (FRACBITS). This can be easily accomplished
    # using the shrd instruction

    mov     $25, %cl        # set shift amount in %cl
    shrdl   %edx,%eax       # shift right double, store in %eax

    popl    %ebp            # restore the base pointer
    ret                     # return from routine

