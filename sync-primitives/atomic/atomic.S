.globl AtomicLoad
.globl AtomicStore
.globl AtomicExchange
.globl AtomicFetchAdd

AtomicLoad:
    movq (%rdi), %rax
    retq

AtomicStore:
    movq %rsi, (%rdi)
    retq

AtomicExchange:
    movq %rsi, %rax
    xchgq %rax, (%rdi)
    retq

AtomicFetchAdd:
    movq %rsi, %rax
    lock xaddq %rax, (%rdi)
    retq
