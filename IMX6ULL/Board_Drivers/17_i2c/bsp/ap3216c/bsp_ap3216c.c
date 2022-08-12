#include "bsp_ap3216c.h"
#include "../i2c/bsp_i2c.h"
#include "../gpio/bsp_gpio.h"
#include "../delay/bsp_delay.h"
#include "stdio.h"

void ap3216c_init(void)
{
    unsigned char val;

    //  IO初始化
    IOMUXC_SetPinMux(IOMUXC_UART4_TX_DATA_I2C1_SCL, 1);
    IOMUXC_SetPinMux(IOMUXC_UART4_RX_DATA_I2C1_SDA, 1);

    IOMUXC_SetPinConfig(IOMUXC_UART4_TX_DATA_I2C1_SCL, 0x70B0);
    IOMUXC_SetPinConfig(IOMUXC_UART4_RX_DATA_I2C1_SDA, 0x70B0);

    //  I2C接口初始化
    i2c_init(I2C1);

    //  AP3216C传感器初始化
    ap3216c_write(AP3216C_ADDR, AP3216C_SYSTEMCONG, 0x4);   //  复位
    delay_ms(50);
    ap3216c_write(AP3216C_ADDR, AP3216C_SYSTEMCONG, 0x3);
    val = ap3216c_read(AP3216C_ADDR, AP3216C_SYSTEMCONG);
 
    printf("AP3216C_SYSTEMCONG = %#x\r\n", val);    
}

//  ap3216c读一个字节
unsigned char ap3216c_read(unsigned char addr, unsigned char reg)
{
    unsigned char val = 0;
    struct i2c_transfer xfer;

    xfer.slaveAddr = addr;
    xfer.dir = I2C_READ;
    xfer.subaddr = reg;
    xfer.subaddrSize = 1;
    xfer.data = &val;
    xfer.dataSize = 1;

    i2c_master_transfer(I2C1, &xfer);

    return val;
}

unsigned char ap3216c_write(unsigned char addr, unsigned char reg, unsigned char data)
{
    unsigned char status = 0;
    unsigned char data_w = data;
    struct i2c_transfer xfer;

    xfer.slaveAddr = addr;
    xfer.dir = I2C_WRITE;
    xfer.subaddr = reg;
    xfer.subaddrSize = 1;
    xfer.data = &data_w;
    xfer.dataSize = 1;

    if(i2c_master_transfer(I2C1, &xfer))
        status = 1;
    return status;
}

void ap3216c_readreg(unsigned short* ir, unsigned short* ps, unsigned short* als)
{
    unsigned char buf[6] = "";
    unsigned char i = 0;

    //  循环读取寄存器数据
    for(; i < 6; i++)
    {
        buf[i] = ap3216c_read(AP3216C_ADDR, AP3216C_IRDATALOW + i);
    }

    //  判断IR和PS数据是否有效，真无效
    if(buf[0] & 0x80)
    {
        *ir = 0;
        *ps = 0;
    }
    else
    {
        *ir = ((unsigned short)buf[1] << 2) | (buf[0] & 0x03);
        *ps = (((unsigned short)buf[5] & 0x3F) << 4) | (buf[4] & 0x0F);
    }

    *als = ((unsigned short)buf[5] << 8) | buf[2];
}