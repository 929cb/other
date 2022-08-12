#pragma once

#include "../../imx6u/imx6u.h"

enum I2C_TRANSFER
{
    I2C_WRITE,  //  主机向从机写
    I2C_READ
};

struct i2c_transfer
{
    unsigned char slaveAddr;
    enum I2C_TRANSFER dir;
    unsigned int subaddr;       //  寄存器地址
    unsigned char subaddrSize;  //  寄存器地址长度
    unsigned char* volatile data;
    volatile unsigned int dataSize;
};

enum I2C_STATUS
{
    I2C_STATUS_OK,
    I2C_STATUS_BUSY,
    I2C_STATUS_IDLE,
    I2C_STATUS_NAK,
    I2C_STATUS_ARBITRATIONLOST,  //  仲裁丢失
    I2C_STATUS_TIMEOUT,
    I2C_STATUS_ADDRNAK
};



void i2c_init(I2C_Type* base);
unsigned char i2c_master_start(I2C_Type* base, unsigned char addr, enum I2C_TRANSFER dir);
unsigned char i2c_master_stop(I2C_Type* base);
unsigned char i2c_master_repeated_start(I2C_Type* base, unsigned char addr, enum I2C_TRANSFER dir);
unsigned char i2c_check_clear_error(I2C_Type* base, unsigned int status);
void i2c_master_write(I2C_Type* base, const unsigned char* buf, unsigned int size);
void i2c_master_read(I2C_Type* base, unsigned char* buf, unsigned int size);
unsigned char i2c_master_transfer(I2C_Type *base, struct i2c_transfer *xfer);
