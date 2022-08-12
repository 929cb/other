.global _start

.global _bss_start
_bss_start:
    .word __bss_start

.global _bss_end
_bss_end:
    .word __bss_end

_start:
    /* 设置处理器进入SVC模式 */
    mrs r0, cpsr
    bic r0, #0x1F
    orr r0, #0x13   @使用SVC模式
    msr cpsr, r0

    /* 清零bss段 */
    ldr r0, =_bss_start
    ldr r1, =_bss_end
    mov r2, #0
bss_loop:
    stmia r0!, {r2};
    cmp r0, r1
    ble bss_loop

    /* 设置sp指针 */
    ldr sp, =0x80200000

    b main
    