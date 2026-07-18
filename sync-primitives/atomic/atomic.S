.globl AtomicLoad
.globl AtomicStore
.globl AtomicExchange

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
