#include "bsp_clk.h"

void clk_enable(void)
{
    //  使能所有时钟
    CCM->CCGR0 = 0xFFFFFFFF;
    CCM->CCGR1 = 0xFFFFFFFF;
    CCM->CCGR2 = 0xFFFFFFFF;
    CCM->CCGR3 = 0xFFFFFFFF;
    CCM->CCGR4 = 0xFFFFFFFF;
    CCM->CCGR5 = 0xFFFFFFFF;
    CCM->CCGR6 = 0xFFFFFFFF;
}   

/* 初始化时钟主频为528MHz */
void imx6u_clkinit(void)
{
    //  判断当前时钟使用的是pll1_main_clk
    if(((CCM->CCSR >> 2) & 1) == 0)
    {
        //  选择osc_clk(24M)时钟源
        CCM->CCSR &= ~(1 << 8);
        //  暂时换成step_clk
        CCM->CCSR |= (1 << 2);
    }
    //  设置pll的频率1056M，并使能时钟输出
    CCM_ANALOG->PLL_ARM = (88 & 0x7F) | (1 << 13);
    //  设置二分频
    CCM->CACRR = 1;
    //  切换回pll1_main_clk，此时主频为528MHz
    CCM->CCSR &= ~(1 << 2);

    //  (12~35)
    //  设置pll2的4路pfd
    unsigned int reg = CCM_ANALOG->PFD_528;
    reg &= ~(0x3F3F3F3F);
    reg |= (32 << 24);  //  pfd3=297MHz
    reg |= (24 << 16);  //  pfd2=396MHz
    reg |= (16 << 8);   //  pfd1=594MHz
    reg |= (27 << 0);   //  pfd0=352MHz
    //  设置pll3的4路pfd
    reg = 0;
    reg = CCM_ANALOG->PFD_528;
    reg &= ~(0x3F3F3F3F);
    reg |= (19 << 24);  //  pfd3=404.7MHz
    reg |= (17 << 16);  //  pfd2=508.2MHz
    reg |= (16 << 8);   //  pfd1=540MHz
    reg |= (12 << 0);   //  pfd0=720MHz

    //  设置AHB_CLK_ROOT=132MHz
    CCM->CBCMR &= ~(3 << 18);
    CCM->CBCMR |= 1 << 18;  //  396MHz
    CCM->CBCDR &= ~(1 << 25);
    while(CCM->CDHIPR & (1 << 5));  //  等待握手信号
#if 0
    CCM->CBCDR &= ~(7 << 10);
    CCM->CBCDR |= (2 << 10);    //  F/3
    while(CCM->CDHIPR & (1 << 1));
#endif

    //  IPG_CLK_ROOT=66MHz(来自AHB的时钟)
    CCM->CBCDR &= ~(3 << 8);
    CCM->CBCDR |= (1 << 8);
    //  PER_CLK=66MHz
    CCM->CSCMR1 &= ~(1 << 6);   //  选择来自ipg的时钟
    CCM->CSCMR1 &= ~(0x3F < 0);


    //  设置uart1为pll3=80MHz
    CCM->CSCDR1 &= ~(1 << 6);
    CCM->CSCDR1 &= ~0x3F;   //  1分频
}