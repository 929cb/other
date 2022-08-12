#pragma once

#define CCM_BASE    0x020C4000

#define CCM_CCGR0   *((volatile unsigned int*)(CCM_BASE + 0x68))
#define CCM_CCGR1   *((volatile unsigned int*)(CCM_BASE + 0x6C))
#define CCM_CCGR2   *((volatile unsigned int*)(CCM_BASE + 0x70))
#define CCM_CCGR3   *((volatile unsigned int*)(CCM_BASE + 0x74))
#define CCM_CCGR4   *((volatile unsigned int*)(CCM_BASE + 0x78))
#define CCM_CCGR5   *((volatile unsigned int*)(CCM_BASE + 0x7C))
#define CCM_CCGR6   *((volatile unsigned int*)(CCM_BASE + 0x80))

#define SW_MUX_GPIO1_IO03   *((volatile unsigned int*)0x020E0068)
#define SW_PAD_GPIO1_IO03   *((volatile unsigned int*)0x020E02F4)


#define GPIOx_BASE  0x0209C000

#define GPIOx_DR          *((volatile unsigned int*)(GPIOx_BASE + 0x0))
#define GPIOx_GDIR        *((volatile unsigned int*)(GPIOx_BASE + 0x04))
#define GPIOx_PSR         *((volatile unsigned int*)(GPIOx_BASE + 0x08))
#define GPIOx_ICR1        *((volatile unsigned int*)(GPIOx_BASE + 0x0C))
#define GPIOx_ICR2        *((volatile unsigned int*)(GPIOx_BASE + 0x10))
#define GPIOx_IMR         *((volatile unsigned int*)(GPIOx_BASE + 0x14))
#define GPIOx_ISR         *((volatile unsigned int*)(GPIOx_BASE + 0x18))
#define GPIOx_EDGE_SEL    *((volatile unsigned int*)(GPIOx_BASE + 0x1C))


