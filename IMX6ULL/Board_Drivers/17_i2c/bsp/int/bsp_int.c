#include "bsp_int.h"

static unsigned int irqNesting;

static sys_irq_handle irqTable[NUMBER_OF_INT_VECTORS];

//  初始化中断处理函数表
void system_irqtable_init(void)
{
    irqNesting = 0;

    unsigned int i = 0;
    for(; i < NUMBER_OF_INT_VECTORS; i++)
    {
        irqTable[i].irqHandler = default_irqhandler;
        irqTable[i].userParam = NULL;
    }
}

//  注册中断处理函数
void system_regiter_irqhandler(IRQn_Type irq, system_irqhandler_t handler, void* param)
{
    irqTable[irq].irqHandler = handler;
    irqTable[irq].userParam = param;
}

//  中断初始化函数
void int_init(void)
{
    GIC_Init();
    system_irqtable_init();

    //  中断向量偏移（汇编或这里设置）
    __set_VBAR(0x87800000);
}

//  具体中断处理函数，IRQ中断
void system_irqhandler(unsigned int gicciar)
{
    uint32_t intNum = gicciar & 0x3FF;   //  bit9~0表示中断ID
    //  检查中断ID
    if(intNum >= 160)    //  32+128
    {
        return;
    }

    irqNesting++;
    irqTable[intNum].irqHandler(intNum, irqTable[intNum].userParam);
    irqNesting--;
}

//  默认中断处理函数
void default_irqhandler(unsigned int gicciar, void* param)
{
    while(1);
}