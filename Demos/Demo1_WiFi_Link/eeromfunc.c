#include "eeromfunc.h"
/**
 **@brief  初始化SCL SDA 脚
 **/  
__IO uint8_t   EEDataNum; 

void swi2c_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//开漏模式
	GPIO_InitStructure.GPIO_Pin = sEE_I2C_SDA_PIN;
	GPIO_Init(sEE_I2C_SDA_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽模式
	GPIO_InitStructure.GPIO_Pin = sEE_I2C_SCL_PIN;
	GPIO_Init(sEE_I2C_SDA_GPIO_PORT, &GPIO_InitStructure);
}
/**
 **@brief  I2C协议的起始位
 **/  
	
void swi2c_start(void)   
{
	  swi2c_sda_high();    
    delay_us(DELAYTIME); 
    swi2c_scl_high();    
    delay_us(DELAYTIME);            
    swi2c_sda_low();
    delay_us(DELAYTIME);           
    swi2c_scl_low();
    delay_us(DELAYTIME);            
    return;
}   

/**
 **@brief  I2C协议的停止位
 **/  
	
void swi2c_stop(void)   
{
		swi2c_scl_low();  
		delay_us(DELAYTIME);            // tSU, Min:4.7us
    swi2c_sda_low();
    delay_us(DELAYTIME);            // tSU, Min:4.7us
    swi2c_scl_high();    
    delay_us(DELAYTIME);            // tSU, Min:4.7us
    swi2c_sda_high();
    return;
}

/**
 **@brief  I2C写数据操作
 **/   
int swi2c_wdata(uint8_t data)   
{
    uint8_t        ibit;   
    int            retval = 0;

    // An I2C output byte is bits 7-0 (MSB to LSB). Shift one bit at a time to 
    // the SDA output, and then clock the data to the I2C Slave device.    
    // Send 8 bits out the port     
    for(ibit = 0; ibit < 8; ibit++) {
        if (data & 0x80) {
            swi2c_sda_high();
        } else {
            swi2c_sda_low();
        }
        delay_us(DELAYTIME);    // tSU, Min:4.7us            
        swi2c_scl_high();
        delay_us(DELAYTIME);    // tHIGH, Min:4.0us
        swi2c_scl_low();
        delay_us(DELAYTIME);    // tLOW, Min:4.7us
        data <<= 1;        			//shift the byte by one bit    
    }
		//等待接收ACK
    swi2c_sda_input();
    swi2c_scl_high();
    delay_us(DELAYTIME);    // tSU, Min:4.7us

    if (!swi2c_sda()) { 	  //判断是否接收到ACK
        retval = 0;        // ACK from slave    
    } else {
        retval = -1;    // NACK from slave    
    }

    swi2c_scl_low(); 
    swi2c_sda_output();
    swi2c_sda_high();

    delay_us(DELAYTIME);    // tLOW, Min:4.7us

    return retval;
}

//------------------------------------------------------------------------------ 
//  Routine:    Read    
//  Inputs:     *data_in, send_ack (if true send the ACK signal else send NACK)    
//  Outputs:    bool    
//  Purpose:    Reads data from the I2C bus and return it in data_in.    
//              Returns status.    
//------------------------------------------------------------------------------ 
     
void swi2c_rdata(uint8_t *data, int ack)
{
    uint8_t        ibit;

    swi2c_sda_input();
     
    // Get 8 bits from the device    
    for (*data = 0, ibit = 0; ibit < 8; ibit ++) {
        swi2c_scl_high();
        delay_us(DELAYTIME);    // tHIGH, Min:4.7us

        if (swi2c_sda()) {
            *data = *data | (1 << (7 - ibit));
        }

        swi2c_scl_low();    //
        delay_us(DELAYTIME);    // tLOW, Min:4.7us
    }
    
    swi2c_sda_output();

    if (ack) {
        swi2c_sda_low();    // Set data pin to output/low to ACK the read
    } else {
        swi2c_sda_high();    // Set data pin to input/high to NACK the read
    }
    delay_us(DELAYTIME);    // tSU, Min:4.7us
     
    swi2c_scl_high();    // Set SCLK high    
    delay_us(DELAYTIME);    // tHIGH, Min:4.0us

    swi2c_scl_low();    // Set SCLK high    
    swi2c_sda_high();

    delay_us(DELAYTIME);    // tLOW, Min:4.7us

    return;
}   
//------------------------------------------------------------------------------ 
// @brif: 单字节写  
//------------------------------------------------------------------------------ 
int swi2c_write(uint8_t i2caddr, uint16_t subaddr, uint8_t data)
{
    // posix_print("swi2c_write, (%02x, %02x, %02x)\n", i2caddr, subaddr, data);
		
    swi2c_start();    // Send start signal    
    if (swi2c_wdata(i2caddr & 0xfe)) {    // Send identifier I2C address
        swi2c_stop();    // Send I2C Stop Transfer    
        return -1;   
    }
    if (swi2c_wdata((uint8_t)(subaddr>>8)& 0x1f)){    // Send address to device
        swi2c_stop();    // Send I2C Stop Transfer
        return -1;   
    }
		if (swi2c_wdata((uint8_t)subaddr)){    // Send address to device
      swi2c_stop();    // Send I2C Stop Transfer
      return -1;   
    }
    if (swi2c_wdata(data)) {    // Send address to device
        swi2c_stop();    // Send I2C Stop Transfer
        return -1;   
    }
    swi2c_stop();    // Send I2C Stop Transfer
    return 0;   
}
//------------------------------------------------------------------------------ 
// @brif: 以页的形式进行写   
//------------------------------------------------------------------------------ 
int swi2c_blkwrite(uint8_t i2caddr, uint16_t subaddr, uint8_t *databuf, uint32_t blksize)
{
	uint32_t index_i = 0;
	while(1)
	{
		/*!< Send START condition */
		swi2c_start();      
		/*!< Send EEPROM address for write */
		if (swi2c_wdata(i2caddr & 0xfe)) {    // Send identifier I2C address
				swi2c_stop();    // Send I2C Stop Transfer    
				return -1;   
		}
		/*!< Send the EEPROM's internal address to write to : MSB of the address first */
		if (swi2c_wdata((uint8_t)(subaddr>>8)& 0x1f)){    // Send address to device
				swi2c_stop();    // Send I2C Stop Transfer
				return -1;   
		}
		if (swi2c_wdata((uint8_t)subaddr)){    // Send address to device
			swi2c_stop();    // Send I2C Stop Transfer
			return -1;   
		}
		
		while(blksize)
		{
			if (swi2c_wdata(databuf[index_i++])) // Send data to device
			{    
				swi2c_stop();    // Send I2C Stop Transfer
				return -1;   
			}
			if(index_i >= PAGELENGHT)
			{
					swi2c_stop();    // Send I2C Stop Transfer
					subaddr += PAGELENGHT;
					break;
			}
			if(blksize != 0)
				blksize--;
		}
		if(blksize == 0) 
		{
			swi2c_stop();    // Send I2C Stop Transfer
			break;		
		}
	}
	return 0;
}
//------------------------------------------------------------------------------ 
// @brif: 单字节读 
//------------------------------------------------------------------------------ 
int swi2c_read(uint8_t i2caddr, uint16_t subaddr, uint8_t *data)
{   
    swi2c_start();    // Send start signal    
    if (swi2c_wdata(i2caddr & 0xfe)) {    // Send identifier I2C address
        swi2c_stop();    // Send I2C Stop Transfer    
        return -1;   
    }
    if (swi2c_wdata((uint8_t)(subaddr>>8)&0x1f)) {    // Send high address to device
        swi2c_stop();    // Send I2C Stop Transfer
        return -1;   
    }
		if (swi2c_wdata((uint8_t)subaddr)) {    // Send high address to device
        swi2c_stop();    // Send I2C Stop Transfer
        return -1;   
    }
    swi2c_start();    // Send start signal
    if (swi2c_wdata(i2caddr | 0x01)) {    // Send identifier I2C address
        swi2c_stop();    // Send I2C Stop Transfer    
        return -1;   
    }   
    swi2c_rdata(data, 0);
    swi2c_stop();    // Send I2C Stop Transfer
    return 0;
}
//------------------------------------------------------------------------------ 
// @brif: 以页的形式进行读   
//------------------------------------------------------------------------------ 
int swi2c_blkread(uint8_t i2caddr, uint16_t subaddr, uint8_t *data, uint32_t blksize)
{   
		int index_i = 0;
    swi2c_start();    // Send start signal    
    if (swi2c_wdata(i2caddr & 0xfe)) {    // Send identifier I2C address
        swi2c_stop();    // Send I2C Stop Transfer    
        return -1;   
    }
    if (swi2c_wdata((uint8_t)(subaddr>>8)&0x1f)) {    // Send high address to device
        swi2c_stop();    // Send I2C Stop Transfer
        return -1;   
    }
		if (swi2c_wdata((uint8_t)subaddr)) {    // Send high address to device
        swi2c_stop();    // Send I2C Stop Transfer
        return -1;   
    }
    swi2c_start();    // Send start signal
    if (swi2c_wdata(i2caddr | 0x01)) {    // Send identifier I2C address
        swi2c_stop();    // Send I2C Stop Transfer    
        return -1;   
    }   
		while((blksize--)>1)
		{
			swi2c_rdata(data+index_i, 1);
			index_i++;
		}
		swi2c_rdata(data+index_i, 0);
    swi2c_stop();    // Send I2C Stop Transfer
    return 0;
}
void swi2c_scl_low()
{
	sEE_I2C_SCL_GPIO_PORT->BSRRH = sEE_I2C_SCL_PIN;
}
void swi2c_scl_high()
{
	sEE_I2C_SCL_GPIO_PORT->BSRRL = sEE_I2C_SCL_PIN;
}
void swi2c_sda_input()
{
	GPIO_InitTypeDef GPIO_InitStructure;		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Pin = sEE_I2C_SDA_PIN;
	GPIO_Init(sEE_I2C_SDA_GPIO_PORT, &GPIO_InitStructure);
}
void swi2c_sda_output()
{
	GPIO_InitTypeDef GPIO_InitStructure;		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Pin = sEE_I2C_SDA_PIN;
	GPIO_Init(sEE_I2C_SDA_GPIO_PORT, &GPIO_InitStructure);
}	
void swi2c_sda_low()
{
	sEE_I2C_SDA_GPIO_PORT->BSRRH = sEE_I2C_SDA_PIN;
}	
void swi2c_sda_high()
{
	sEE_I2C_SDA_GPIO_PORT->BSRRL = sEE_I2C_SDA_PIN;
}	
int swi2c_sda()
{
	return GPIO_ReadInputDataBit(sEE_I2C_SDA_GPIO_PORT,sEE_I2C_SDA_PIN);
}
