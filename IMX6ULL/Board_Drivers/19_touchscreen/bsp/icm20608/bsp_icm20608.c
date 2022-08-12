#include "bsp_icm20608.h"
#include "../spi/bsp_spi.h"
#include "../delay/bsp_delay.h"

struct icm20608_dev_struc icm20608_dev;

//  0:初始化成功，1：失败
unsigned char icm20608_init(void)
{
    //  SPI引脚初始化
    IOMUXC_SetPinMux(IOMUXC_UART2_RX_DATA_ECSPI3_SCLK, 0);
    IOMUXC_SetPinMux(IOMUXC_UART2_CTS_B_ECSPI3_MOSI, 0);
    IOMUXC_SetPinMux(IOMUXC_UART2_RTS_B_ECSPI3_MISO, 0);

    IOMUXC_SetPinConfig(IOMUXC_UART2_RX_DATA_ECSPI3_SCLK, 0x10B1);
    IOMUXC_SetPinConfig(IOMUXC_UART2_CTS_B_ECSPI3_MOSI, 0x10B1);
    IOMUXC_SetPinConfig(IOMUXC_UART2_RTS_B_ECSPI3_MISO, 0x10B1);
    //  软件片选
    IOMUXC_SetPinMux(IOMUXC_UART2_TX_DATA_GPIO1_IO20, 0);
    IOMUXC_SetPinConfig(IOMUXC_UART2_TX_DATA_GPIO1_IO20, 0x10B0);
    gpio_pin_config_t cs_config;
    cs_config.direction = GPIO_DigitalOutput;
    cs_config.outputLogic = 0;
    gpio_init(GPIO1, 20, cs_config);

    //  SPI控制器初始化
    spi_init(ECSPI3);

    //  ICM20608初始化
    icm20608_write(ICM20_PWR_MGMT_1, 0x80); //  复位，复位后为0x40，睡眠模式
    delay_ms(50);
    icm20608_write(ICM20_PWR_MGMT_1, 0x01); //  关闭睡眠，自动选择时钟
    delay_ms(50);

    unsigned char reg_val;
    reg_val = icm20608_read(ICM20_WHO_AM_I);
    if(reg_val == ICM20608D_ID || reg_val == ICM20608G_ID)
    {
        icm20608_write(ICM20_SMPLRT_DIV, 0x00); 	/* 输出速率是内部采样率					*/
        icm20608_write(ICM20_GYRO_CONFIG, 0x18); 	/* 陀螺仪±2000dps量程 				*/
        icm20608_write(ICM20_ACCEL_CONFIG, 0x18); 	/* 加速度计±16G量程 					*/
        icm20608_write(ICM20_CONFIG, 0x04); 		/* 陀螺仪低通滤波BW=20Hz 				*/
        icm20608_write(ICM20_ACCEL_CONFIG2, 0x04); 	/* 加速度计低通滤波BW=21.2Hz 			*/
        icm20608_write(ICM20_PWR_MGMT_2, 0x00); 	/* 打开加速度计和陀螺仪所有轴 				*/
        icm20608_write(ICM20_LP_MODE_CFG, 0x00); 	/* 关闭低功耗 						*/
        icm20608_write(ICM20_FIFO_EN, 0x00);		/* 关闭FIFO						*/

        return 0;
    }
    return 1;
}

unsigned char icm20608_read(unsigned char reg)
{
    unsigned char reg_val = 0;
    reg |= 0x80;    //  地址的bit7置1，表示需要读

    ICM20608_LOW;   //  片选

    spich0_readwrite_byte(ECSPI3, reg); //  发送要读取的寄存器地址
    reg_val = spich0_readwrite_byte(ECSPI3, 0xFF);  //  发送无效数据，只为提供一个时钟，得到返回数据

    ICM20608_HIGH;

    return reg_val;
}

void icm20608_write(unsigned char reg, unsigned char data)
{
    reg &= ~0x80;    //  地址的bit7置0，表示需要写

    ICM20608_LOW;   //  片选

    spich0_readwrite_byte(ECSPI3, reg); //  发送要读取的寄存器地址
    spich0_readwrite_byte(ECSPI3, data);  //  发送数据

    ICM20608_HIGH;
}

//  读取多个寄存器
void icm20608_read_regs(unsigned char reg, unsigned char* buf, unsigned char len)
{
    unsigned char i = 0;

    reg |= 0x80;

    ICM20608_LOW;

    spich0_readwrite_byte(ECSPI3, reg);

    for(; i < len; i++)
    {
        buf[i] = spich0_readwrite_byte(ECSPI3, 0xFF);
    }

    ICM20608_HIGH;
}


/*
 * @description : 获取陀螺仪的分辨率
 * @param		: 无
 * @return		: 获取到的分辨率
 */
float icm20608_gyro_scaleget(void)
{
	unsigned char data;
	float gyroscale;
	
	data = (icm20608_read(ICM20_GYRO_CONFIG) >> 3) & 0X3;
	switch(data) {
		case 0: 
			gyroscale = 131;
			break;
		case 1:
			gyroscale = 65.5;
			break;
		case 2:
			gyroscale = 32.8;
			break;
		case 3:
			gyroscale = 16.4;
			break;
	}
	return gyroscale;
}

/*
 * @description : 获取加速度计的分辨率
 * @param		: 无
 * @return		: 获取到的分辨率
 */
unsigned short icm20608_accel_scaleget(void)
{
	unsigned char data;
	unsigned short accelscale;
	
	data = (icm20608_read(ICM20_ACCEL_CONFIG) >> 3) & 0X3;
	switch(data) {
		case 0: 
			accelscale = 16384;
			break;
		case 1:
			accelscale = 8192;
			break;
		case 2:
			accelscale = 4096;
			break;
		case 3:
			accelscale = 2048;
			break;
	}
	return accelscale;
}

//  获取ICM20608内部数据
void icm20608_getdata(void)
{
    float gyroscale;
	unsigned short accescale;
    unsigned char data[14] = "";

    icm20608_read_regs(ICM20_ACCEL_XOUT_H, data, 14);

    gyroscale = icm20608_gyro_scaleget();
	accescale = icm20608_accel_scaleget();

    icm20608_dev.accel_x_adc = (signed short)((data[0] << 8) | data[1]);
    icm20608_dev.accel_y_adc = (signed short)((data[2] << 8) | data[3]); 
	icm20608_dev.accel_z_adc = (signed short)((data[4] << 8) | data[5]); 
	icm20608_dev.temp_adc    = (signed short)((data[6] << 8) | data[7]); 
	icm20608_dev.gyro_x_adc  = (signed short)((data[8] << 8) | data[9]); 
	icm20608_dev.gyro_y_adc  = (signed short)((data[10] << 8) | data[11]);
	icm20608_dev.gyro_z_adc  = (signed short)((data[12] << 8) | data[13]);

	/* 计算实际值 */
	icm20608_dev.gyro_x_act = ((float)(icm20608_dev.gyro_x_adc)  / gyroscale) * 100;
	icm20608_dev.gyro_y_act = ((float)(icm20608_dev.gyro_y_adc)  / gyroscale) * 100;
	icm20608_dev.gyro_z_act = ((float)(icm20608_dev.gyro_z_adc)  / gyroscale) * 100;

	icm20608_dev.accel_x_act = ((float)(icm20608_dev.accel_x_adc) / accescale) * 100;
	icm20608_dev.accel_y_act = ((float)(icm20608_dev.accel_y_adc) / accescale) * 100;
	icm20608_dev.accel_z_act = ((float)(icm20608_dev.accel_z_adc) / accescale) * 100;

	icm20608_dev.temp_act = (((float)(icm20608_dev.temp_adc) - 25 ) / 326.8 + 25) * 100;
}