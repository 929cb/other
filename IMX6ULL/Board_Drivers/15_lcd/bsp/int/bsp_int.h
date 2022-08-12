#pragma once

#include "../../imx6u/imx6u.h"


//  中断处理函数类型定义
typedef void (*system_irqhandler_t)(unsigned int gicciar, void* param);

//  中断处理函数结构体
typedef struct _sys_irq_handle
{
    system_irqhandler_t irqHandler;
    void* userParam;
}sys_irq_handle;


void int_init(void);
void system_regiter_irqhandler(IRQn_Type irq, system_irqhandler_t handler, void* param);
void system_irqhandler(unsigned int gicciar);
void default_irqhandler(unsigned int gicciar, void* param);
