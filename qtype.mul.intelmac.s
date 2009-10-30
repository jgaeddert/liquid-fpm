# q32_t q32_mul(q32_t _x, q32_t _y)
#
# multiplies 2 q32_t fixed-point numbers
#

.include "q32.include.s"

.text
.globl _q32_mul
_q32_mul:
    pushq   %rbp            # create the stack frame
    movq    %rsp, %rbp

    #movl    12(%ebp),%eax   # grab the first argument
    #movl    8(%ebp),%ecx    # grab the second argument
    #imull   %ecx            # multiply arguments (signed long)
                            # with result stored in %edx:%eax

    movl    %edi, %eax      # grab the first argument
    movl    %esi, %ecx      # grab the second argument
    imull   %ecx            # multiply arguments (signed long)
                            # with result stored in %edx:%eax

    # Now we need to shift the 64-bit number %edx:%eax to the
    # right by _qfracbits (FRACBITS). This can be easily accomplished
    # using the shrd instruction

    #mov     _qfracbits(%rip), %cl        # set shift amount in %cl
    mov     $qfrac, %cl        # set shift amount in %cl
    shrdl   %edx,%eax       # shift right double, store in %eax

    leave                   # restore the base pointer
    ret                     # return from routine

