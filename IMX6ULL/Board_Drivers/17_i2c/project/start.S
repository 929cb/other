.global _start


_start:
    /* 中断向量表 */
    ldr pc, =Reset_Handler      @复位中断
    ldr pc, =Undefined_Handler  @未定义指令中断
    ldr pc, =SVC_Handler        @SVC
    ldr pc, =PreAbort_Handler   @预取终止
    ldr pc, =DataAbort_Handler  @数据终止
    ldr pc, =NotUsed_Handler    @未使用中断
    ldr pc, =IRQ_Handler        @IRQ中断
    ldr pc, =FIQ_Handler        @快速中断

/* 复位中断服务函数 */
Reset_Handler:
    @ 关闭IRQ
    cpsid i

    @ 关闭I/D Cache和MMU
    @ 修改cp15协处理器SCTLR寄存器
    MRC p15, 0, r0, c1, c0, 0   @ 读取SCTLR寄存器到r0
    bic r0, r0, #(1 << 12)  @ 关闭I Cache
    bic r0, r0, #(1 << 11)  @ 关闭分支预测
    bic r0, r0, #(1 << 2)   @ 关闭D Cache
    bic r0, r0, #(1 << 1)   @ 关闭对齐
    bic r0, r0, #(1 << 0)   @ 关闭MMU
    MCR p15, 0, r0, c1, c0, 0   @ 将r0写入SCTLR寄存器
/*
    @ 设置中断向量偏移
    ldr r0, =0x87800000
    dsb @ 数据同步
    isb @ 指令同步
    MCR p15, 0, r0, c12, c0, 0  @ 设置VBAR寄存器
    dsb
    isb
*/


.global _bss_start
_bss_start:
    .word __bss_start

.global _bss_end
_bss_end:
    .word __bss_end
    
    @清零bss段
    ldr r0, =_bss_start
    ldr r1, =_bss_end
    mov r2, #0
bss_loop:
    stmia r0!, {r2};
    cmp r0, r1
    ble bss_loop

    @ 设置处理器进入IRQ模式
    mrs r0, cpsr
    bic r0, #0x1F
    orr r0, #0x12   @使用IRQ模式
    msr cpsr, r0
    @ 设置IRQ下sp指针
    ldr sp, =0x80600000

    @ 设置处理器进入SYS模式
    mrs r0, cpsr
    bic r0, #0x1F
    orr r0, #0x1F   @使用SYS模式
    msr cpsr, r0
    @ 设置SYS下sp指针
    ldr sp, =0x80400000

    @ 设置处理器进入SVC模式
    mrs r0, cpsr
    bic r0, #0x1F
    orr r0, #0x13   @使用SVC模式
    msr cpsr, r0
    @ 设置SVC下sp指针
    ldr sp, =0x80200000

    @ 打开IRQ
    cpsie i

    b main

/* 未定义指令中断服务函数 */
Undefined_Handler:
    ldr r0, =Undefined_Handler
    bx r0

/* SVC中断服务函数 */
SVC_Handler:
    ldr r0, = SVC_Handler
    bx r0

/* /预取终止中断服务函数 */
PreAbort_Handler:
    ldr r0, =PreAbort_Handler
    bx r0

/* 数据终止中断服务函数 */
DataAbort_Handler:
    ldr r0, =DataAbort_Handler
    bx r0

/* 未使用中断服务函数 */
NotUsed_Handler:
    ldr r0, =NotUsed_Handler
    bx r0

/* IRQ中断服务函数 */
IRQ_Handler:
    push {lr}   @ 保存lr地址
    push {r0-r3, r12}   @ 保存r0～r3，r12寄存器

    mrs r0, spsr    @ 读取spsr寄存器
    push {r0}   @ 保存spsr寄存器

    @ 保存cp15的c0寄存器内的值到r1寄存器
    MRC p15, 4, r1, c15, c0, 0

    add r1, r1, #0x2000 @ GIC基址+0x2000为GIC的CPU接口端基址
    ldr r0, [r1, #0xC]  @ 读取CPU接口端的GICC_IAR寄存器的值(bit9~0为中断ID)

    push {r0, r1}   @ 保存r0，r1寄存器
    
    cps #0x13   @ 进入SVC模式，允许其他中断

    push {lr}   @ 保存SVC模式lr寄存器
    ldr r2, =system_irqhandler  @ 加载c语言中断处理函数
    blx r2  @ 运行中断处理函数

    pop {lr}    @ 执行完c语言中断处理函数，lr出栈
    cps #0x12   @ 进入IRQ模式
    pop {r0, r1}    
    str r0, [r1, #0x10] @ 中断执行完成，写GICC_EOIR(写中断ID值)

    pop {r0}    
    msr spsr_cxsf, r0   @ 恢复spsr
    
    pop {r0-r3, r12}    
    pop {lr}
    subs pc, lr, #4 @ ARM架构问题(取指、译指、执行)，更新pc时+8

/* FIQ中断服务函数 */
FIQ_Handler:
    ldr r0, =FIQ_Handler
    bx r0


    
    