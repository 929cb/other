#include "bsp_i2c.h"

void i2c_init(I2C_Type* base)
{
    base->I2CR &= ~(1 << 7);    //  关闭i2c
    base->IFDR = 0x15;  //  66MHz/640
    base->I2CR |= (1 << 7);
}

//  产生start信号、发送从机地址、读写方向
unsigned char i2c_master_start(I2C_Type* base, unsigned char addr, enum I2C_TRANSFER dir)
{
    if(base->I2SR & (1 << 5))
        return 1;   //  I2C忙

    //  设置为主机发送模式
    base->I2CR |= (1 << 5) | (1 << 4);

    //  发送地址会自动产生start信号
    base->I2DR = ((unsigned int)addr << 1) | ((dir == I2C_READ) ? 1 : 0);

    return 0;
}

unsigned char i2c_master_stop(I2C_Type* base)
{
    unsigned short timeout = 0xffff;

    base->I2CR &= ~((1 << 5) | (1 << 4) | (1 << 3));

    //  等待I2C忙结束
    while((base->I2SR & (1 << 5)))
    {
        if(timeout-- == 0)
            return I2C_STATUS_BUSY;
    }

    return I2C_STATUS_OK;
}

unsigned char i2c_master_repeated_start(I2C_Type* base, unsigned char addr, enum I2C_TRANSFER dir)
{
    //  i2c是否忙或工作在从机模式
    if(base->I2SR & (1 << 5) && ((base->I2CR & (1 << 5)) == 0))
        return 1;

    base->I2CR |= (1 << 4) | (1 << 2);
    base->I2DR = ((unsigned int)addr << 1) | ((dir == I2C_READ) ? 1 : 0);

    return I2C_STATUS_OK;
}

unsigned char i2c_check_clear_error(I2C_Type* base, unsigned int status)
{
    //  检查是否为仲裁丢失错误
    if(status & (1 << 4))
    {
        base->I2SR &= ~(1 << 4);
        base->I2CR &= ~(1 << 7);
        base->I2CR |= (1 << 7);
        return I2C_STATUS_ARBITRATIONLOST;
    }
    else if(status & (1 << 0))
        return I2C_STATUS_NAK;
    else return I2C_STATUS_OK;
}

void i2c_master_write(I2C_Type* base, const unsigned char* buf, unsigned int size)
{
    //  等待传输完成
    while(!(base->I2SR & (1 << 7)));

    base->I2SR &= ~(1 << 1);
    base->I2CR |= (1 << 4);

    while(size--)
    {
        base->I2DR = *buf++;

        while(!(base->I2SR & (1 << 1)));    // 等待传输完成
        base->I2SR &= ~(1 << 1);

        if(i2c_check_clear_error(base, base->I2SR)) break;
    }

    base->I2SR &= ~(1 << 1);
    i2c_master_stop(base);
}

void i2c_master_read(I2C_Type* base, unsigned char* buf, unsigned int size)
{
    volatile uint8_t dummy = 0;
    dummy++;    //  防止编译报错

    //  等待传输完成
    while(!(base->I2SR & (1 << 7)));

    base->I2SR &= ~(1 << 1);
    base->I2CR &= ~((1 << 4) | (1 << 3));

    if(size == 1)
        base->I2CR |= (1 << 3); //  nack

    dummy = base->I2DR; //  假读、触发下一个字节的传输

    while(size--)
    {
        while(!(base->I2SR & (1 << 1)));    //  等待传输完成
        base->I2SR &= ~(1 << 1);

        if(size == 0)   //  数据发送完成
            i2c_master_stop(base);

        if(size == 1)
            base->I2CR |= (1 << 3); //  nack

        *buf++ = base->I2DR;
    }
}


unsigned char i2c_master_transfer(I2C_Type *base, struct i2c_transfer *xfer)
{
	unsigned char ret = 0;
	 enum I2C_TRANSFER direction = xfer->dir;	

	base->I2SR &= ~((1 << 1) | (1 << 4));			/* 清除标志位 */

	/* 等待传输完成 */
	while(!((base->I2SR >> 7) & 0X1)){}; 

	/* 如果是读的话，要先发送寄存器地址，所以要先将方向改为写 */
    if ((xfer->subaddrSize > 0) && (xfer->dir == I2C_READ))
    {
        direction = I2C_WRITE;
    }

	ret = i2c_master_start(base, xfer->slaveAddr, direction); /* 发送开始信号 */
    if(ret)
    {	
		return ret;
	}

	while(!(base->I2SR & (1 << 1))){};			/* 等待传输完成 */

    ret = i2c_check_clear_error(base, base->I2SR);	/* 检查是否出现传输错误 */
    if(ret)
    {
      	i2c_master_stop(base); 						/* 发送出错，发送停止信号 */
        return ret;
    }
	
    /* 发送寄存器地址 */
    if(xfer->subaddrSize)
    {
        do
        {
			base->I2SR &= ~(1 << 1);			/* 清除标志位 */
            xfer->subaddrSize--;				/* 地址长度减一 */
			
            base->I2DR =  ((xfer->subaddr) >> (8 * xfer->subaddrSize)); //向I2DR寄存器写入子地址
  
			while(!(base->I2SR & (1 << 1)));  	/* 等待传输完成 */

            /* 检查是否有错误发生 */
            ret = i2c_check_clear_error(base, base->I2SR);
            if(ret)
            {
             	i2c_master_stop(base); 				/* 发送停止信号 */
             	return ret;
            }  
        } while ((xfer->subaddrSize > 0) && (ret == I2C_STATUS_OK));

        if(xfer->dir == I2C_READ) 		/* 读取数据 */
        {
            base->I2SR &= ~(1 << 1);			/* 清除中断挂起位 */
            i2c_master_repeated_start(base, xfer->slaveAddr, I2C_READ); /* 发送重复开始信号和从机地址 */
    		while(!(base->I2SR & (1 << 1))){};/* 等待传输完成 */

            /* 检查是否有错误发生 */
			ret = i2c_check_clear_error(base, base->I2SR);
            if(ret)
            {
             	ret = I2C_STATUS_ADDRNAK;
                i2c_master_stop(base); 		/* 发送停止信号 */
                return ret;  
            }
           	          
        }
    }	


    /* 发送数据 */
    if ((xfer->dir == I2C_WRITE) && (xfer->dataSize > 0))
    {
    	i2c_master_write(base, xfer->data, xfer->dataSize);
	}

    /* 读取数据 */
    if ((xfer->dir == I2C_READ) && (xfer->dataSize > 0))
    {
       	i2c_master_read(base, xfer->data, xfer->dataSize);
	}
	return 0;	
}