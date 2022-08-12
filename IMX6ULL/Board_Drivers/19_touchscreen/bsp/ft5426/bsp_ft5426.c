#include "bsp_ft5426.h"
#include "../gpio/bsp_gpio.h"
#include "../int/bsp_int.h"
#include "../delay/bsp_delay.h"
#include "../i2c/bsp_i2c.h"
#include <stdio.h>

struct ft5426_dev_t ft5426_dev;

void ft5426_init(void)
{
    //  IO初始化
    IOMUXC_SetPinMux(IOMUXC_UART5_TX_DATA_I2C2_SCL, 1);
    IOMUXC_SetPinMux(IOMUXC_UART5_RX_DATA_I2C2_SDA, 1);

    IOMUXC_SetPinConfig(IOMUXC_UART5_TX_DATA_I2C2_SCL, 0x70B0);
    IOMUXC_SetPinConfig(IOMUXC_UART5_RX_DATA_I2C2_SDA, 0x70B0);

    //  初始化INT引脚，使能中断
    IOMUXC_SetPinMux(IOMUXC_GPIO1_IO09_GPIO1_IO09, 0);
    IOMUXC_SetPinConfig(IOMUXC_GPIO1_IO09_GPIO1_IO09, 0xF080);

    gpio_pin_config_t ctint_config;
    ctint_config.direction = GPIO_DigitalInput;
    ctint_config.interruptMode = GPIO_IntRisingOrFallingEdge;
    gpio_init(GPIO1, 9, ctint_config);

    GIC_EnableIRQ(GPIO1_Combined_0_15_IRQn);
    system_regiter_irqhandler(GPIO1_Combined_0_15_IRQn, (system_irqhandler_t)gpio1_0_15_irqhandler, NULL);

    gpio_enableint(GPIO1, 9);

    //  初始化RST引脚
    IOMUXC_SetPinMux(IOMUXC_SNVS_SNVS_TAMPER9_GPIO5_IO09, 0);
    IOMUXC_SetPinConfig(IOMUXC_SNVS_SNVS_TAMPER9_GPIO5_IO09, 0x10B0);

    gpio_pin_config_t ctrst_config;
    ctrst_config.direction = GPIO_DigitalOutput;
    ctrst_config.outputLogic = 1;
    gpio_init(GPIO5, 9, ctrst_config);

    gpio_writePin(GPIO5, 9, 0); //  复位FT5426
    delay_ms(50);
    gpio_writePin(GPIO5, 9, 1);
    delay_ms(50);

    /* 初始化I2C2接口 */
    i2c_init(I2C2);

    /* FT5426初始化 */
    unsigned char reg_val[2] = "";
    ft5426_read_len(FT5426_ADDR, FT5426_IDGLIB_VERSION, reg_val, 2);
    printf("touch frimware version: %#x\r\n", ((unsigned short)reg_val[0] << 8) | reg_val[1]);

    ft5426_write_byte(FT5426_ADDR, FT5426_DEVICE_MODE, 0);  //  工作在正常模式
    ft5426_write_byte(FT5426_ADDR, FT5426_IDG_MODE, 1); //  中断触发

    //  表示初始化完成
    ft5426_dev.initflag = FT5426_INIT_FINISHED;
}

void gpio1_0_15_irqhandler(unsigned int gicciar, void* param)
{
    static unsigned int count;
    printf("touch int: %d\r\n", ++count);

    gpio_clearintflag(GPIO1, 9); /* 清除中断标志位 */
}

//  向FT5426写一个字节数据
unsigned char ft5426_write_byte(unsigned char addr, unsigned char reg, unsigned char data)
{
    unsigned char data_w = data;
    struct i2c_transfer xfer;

    xfer.slaveAddr = addr;
    xfer.dir = I2C_WRITE;
    xfer.subaddr = reg;
    xfer.subaddrSize = 1;
    xfer.data = &data_w;
    xfer.dataSize = 1;
    if(i2c_master_transfer(I2C2, &xfer))
        return 1;
    else return 0;
}

//  从FT5426读取一个字节数据
unsigned char ft5426_read_byte(unsigned char addr, unsigned char reg)
{
    unsigned char data_r;
    struct i2c_transfer xfer;

    xfer.slaveAddr = addr;
    xfer.dir = I2C_READ;
    xfer.subaddr = reg;
    xfer.subaddrSize = 1;
    xfer.data = &data_r;
    xfer.dataSize = 1;
    if(i2c_master_transfer(I2C2, &xfer))
        return 1;
    else return 0;
}

//  读取len个字节
void ft5426_read_len(unsigned char addr, unsigned char reg, unsigned char* buf, unsigned char len)
{
    struct i2c_transfer xfer;

    xfer.slaveAddr = addr;
    xfer.dir = I2C_READ;
    xfer.subaddr = reg;
    xfer.subaddrSize = 1;
    xfer.data = buf;
    xfer.dataSize = len;
    i2c_master_transfer(I2C2, &xfer);
}