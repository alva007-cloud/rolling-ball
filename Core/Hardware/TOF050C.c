/////*
//// * TOF050C.c
//// *
//// *  Created on: Mar 24, 2025
//// *      Author: Alva
//// */



#include "TOF050C.h"
#include "usart_with_zdt.h"
#define OFFSET_CALIBRATION_TARGET_RANGE 50
#define CT_CALIBRATION_TARGET_RANGE 100
#define TIME_FOR_OFFSET_CALIBRATION 50
#define TIME_FOR_CT_CALIBRATION 50
const uint16_t ScalerValues[] = {0, 253, 127, 84};
uint8_t ptp_offset;
uint8_t scaling;
uint16_t io_timeout;

/*
 * @brief 测距模块写命令
 */
void VL6180X_WR_CMD(uint16_t cmd, uint8_t data)
{
    uint8_t data_write[3];
    data_write[0]=(cmd>>8)&0xff;
    data_write[1]=cmd&0xff;
    data_write[2]=data&0xff;
    HAL_I2C_Master_Transmit(&hi2c1, addr_write, data_write, 3, 0x100);
}

void VL6180X_WR_CMD2(uint16_t cmd, uint16_t data)
{
    uint8_t data_write[4];
    data_write[0]=(cmd>>8)&0xff;
    data_write[1]=cmd&0xff;
     data_write[2]=(data>>8)&0xff;
    data_write[3]=data&0xff;
    HAL_I2C_Master_Transmit(&hi2c1, addr_write, data_write, 4, 0x100);
}

uint8_t VL6180X_ReadByte(uint16_t reg)
{
    uint8_t data_write[2];
    uint8_t receive_data=0;
    data_write[0]=(reg>>8)&0xff;
    data_write[1]=reg&0xff;
    HAL_I2C_Master_Transmit(&hi2c1, addr_write, data_write, 2, 0xffff);
    HAL_I2C_Master_Receive(&hi2c1, addr_write, &receive_data, 1, 0xffff);
    return receive_data;
}

uint8_t VL6180X_Init()
{
    ptp_offset = 0;
    scaling = 0;
    io_timeout = 2;

    ptp_offset = VL6180X_ReadByte(SYSRANGE__PART_TO_PART_RANGE_OFFSET);
    uint8_t reset=VL6180X_ReadByte(0x016);//check wether reset over
    if(reset==1)
    {
        VL6180X_WR_CMD(0X0207,0X01);
        VL6180X_WR_CMD(0X0208,0X01);
        VL6180X_WR_CMD(0X0096,0X00);
        VL6180X_WR_CMD(0X0097,0XFD);
        VL6180X_WR_CMD(0X00E3,0X00);
        VL6180X_WR_CMD(0X00E4,0X04);
        VL6180X_WR_CMD(0X00E5,0X02);
        VL6180X_WR_CMD(0X00E6,0X01);
        VL6180X_WR_CMD(0X00E7,0X03);
        VL6180X_WR_CMD(0X00F5,0X02);
        VL6180X_WR_CMD(0X00D9,0X05);
        VL6180X_WR_CMD(0X00DB,0XCE);
        VL6180X_WR_CMD(0X02DC,0X03);
        VL6180X_WR_CMD(0X00DD,0XF8);
        VL6180X_WR_CMD(0X009F,0X00);
        VL6180X_WR_CMD(0X00A3,0X3C);
        VL6180X_WR_CMD(0X00B7,0X00);
        VL6180X_WR_CMD(0X00BB,0X3C);
        VL6180X_WR_CMD(0X00B2,0X09);
        VL6180X_WR_CMD(0X00CA,0X09);
        VL6180X_WR_CMD(0X0198,0X01);
        VL6180X_WR_CMD(0X01B0,0X17);
        VL6180X_WR_CMD(0X01AD,0X00);
        VL6180X_WR_CMD(0X00FF,0X05);
        VL6180X_WR_CMD(0X0100,0X05);
        VL6180X_WR_CMD(0X0199,0X05);
        VL6180X_WR_CMD(0X01A6,0X1B);
        VL6180X_WR_CMD(0X01AC,0X3E);
        VL6180X_WR_CMD(0X01A7,0X1F);
        VL6180X_WR_CMD(0X0030,0X00);

        VL6180X_WR_CMD(0X0011,0X10);
        VL6180X_WR_CMD(0X010A,0X30);
        VL6180X_WR_CMD(0X003F,0X46);
        VL6180X_WR_CMD(0X0031,0XFF);
        VL6180X_WR_CMD(0X0040,0X63);
        VL6180X_WR_CMD(0X002E,0X01);
        VL6180X_WR_CMD(0X001B,0X09);
        VL6180X_WR_CMD(0X003E,0X31);
        VL6180X_WR_CMD(0X0014,0X24);

        VL6180X_WR_CMD(0x016,0x00);
        return 1;
    }
    return 0;
}


void VL6180X_SetScaling(uint8_t new_scaling)
{
  uint8_t const DefaultCrosstalkValidHeight = 20; // default value of SYSRANGE__CROSSTALK_VALID_HEIGHT

  // do nothing if scaling value is invalid
  if (new_scaling < 1 || new_scaling > 3) { return; }

    scaling = new_scaling;
  VL6180X_WR_CMD2(RANGE_SCALER, ScalerValues[scaling]);

  // apply scaling on part-to-part offset
  VL6180X_WR_CMD(SYSRANGE__PART_TO_PART_RANGE_OFFSET, ptp_offset / scaling);

  // apply scaling on CrossTalkValidHeight
  VL6180X_WR_CMD(SYSRANGE__CROSSTALK_VALID_HEIGHT, DefaultCrosstalkValidHeight / scaling);

  // This function does not apply scaling to RANGE_IGNORE_VALID_HEIGHT.

  // enable early convergence estimate only at 1x scaling
  uint8_t rce = VL6180X_ReadByte(SYSRANGE__RANGE_CHECK_ENABLES);
  VL6180X_WR_CMD(SYSRANGE__RANGE_CHECK_ENABLES, (rce & 0xFE) | (scaling == 1));
}


void VL6180X_ConfigureDefault()
{
  VL6180X_WR_CMD(READOUT__AVERAGING_SAMPLE_PERIOD,0x30);
  VL6180X_WR_CMD(SYSALS__ANALOGUE_GAIN, 0x46);
  VL6180X_WR_CMD(SYSRANGE__VHV_REPEAT_RATE, 0xFF);
  VL6180X_WR_CMD2(SYSALS__INTEGRATION_PERIOD, 0x0063);
  VL6180X_WR_CMD(SYSRANGE__VHV_RECALIBRATE, 0x01);
  VL6180X_WR_CMD(SYSRANGE__INTERMEASUREMENT_PERIOD, 0x09);
  VL6180X_WR_CMD(SYSALS__INTERMEASUREMENT_PERIOD, 0x31);
  VL6180X_WR_CMD(SYSTEM__INTERRUPT_CONFIG_GPIO, 0x24);
  VL6180X_WR_CMD(SYSRANGE__MAX_CONVERGENCE_TIME, 0x31);
  VL6180X_WR_CMD(INTERLEAVED_MODE__ENABLE, 0);
  VL6180X_SetScaling(1);
}

static uint8_t VL6180_Read_Range()
{
    int range;
    range=VL6180X_ReadByte(0x062);
    return range;
}

void VL6180X_SetTimeout(uint16_t timeout)
{
  io_timeout = timeout;
}

uint8_t VL6180X_Start_Range()
{
  VL6180X_WR_CMD(0x018,0x01);
  return 0;
}

uint16_t timeoutcnt=0;

/*poll for new sample ready */
uint8_t VL6180X_Poll_Range()
{
    uint8_t status;
    uint8_t range_status;
    status=VL6180X_ReadByte(0x04f);
    range_status=status&0x07;
    while(range_status!=0x04)
    {
        timeoutcnt++;
        if(timeoutcnt > io_timeout)
        {
            break;
        }
        status=VL6180X_ReadByte(0x04f);
        range_status=status&0x07;
        HAL_Delay(1);
    }
    return 0;
}


/*clear interrupt*/
void VL6180X_Clear_Interrupt()
{
  VL6180X_WR_CMD(0x015,0x07);
}

uint16_t VL6180X_ReadRangeSingleMillimeters()
{
    /*Start Single measure mode*/
    VL6180X_Start_Range();
    /* Wait for measurement ready. */
    VL6180X_Poll_Range();
    HAL_Delay(100);
    return (uint16_t)scaling * VL6180_Read_Range();
}

static uint16_t VL6280X_GetRtn(void)
{
	uint16_t raw = VL6180X_ReadByte(RESULT__RANGE_RETURN_RATE);
	uint16_t return_rate = raw / 128;
	return return_rate;
}

/*
 * @brief 用于手动校准的函数
 * @note 使用方法：上电后先将小球放在离模块50mm远的地方，按校准按钮开启校准
 *                等待串口显示校准完成
 */
void VL6180X_Offset_Calibration(void)
{
	uint16_t sum = 0;
	uint8_t offset = 0;
	VL6180X_WR_CMD(SYSRANGE__PART_TO_PART_RANGE_OFFSET,0x00);
	Send_sprintf(PC_USART,"ready to offset calibration:\n");
//	HAL_Delay(2000);   //等待两秒
	for(uint8_t i = 0;i<TIME_FOR_OFFSET_CALIBRATION;i++)
	{
		sum += VL6180X_ReadRangeSingleMillimeters();
	}
	offset = (uint8_t)(OFFSET_CALIBRATION_TARGET_RANGE - sum / TIME_FOR_OFFSET_CALIBRATION);
	VL6180X_WR_CMD(SYSRANGE__PART_TO_PART_RANGE_OFFSET,offset);
	Send_sprintf(PC_USART,"offset calibration success!\n");

}

uint8_t VL6180X_CrossTalk_Callibration(void)
{
	uint8_t sum_of_rate = 0;
	uint8_t sum_of_range = 0;
	uint8_t cross_talk = 0;
	if(VL6180X_ReadByte(SYSRANGE__CROSSTALK_COMPENSATION_RATE)== 0x00)
	{
		Send_sprintf(PC_USART,"ready to cross talk calibration:\n");
		for(uint8_t i =0;i<50;i++)
		{
			sum_of_rate += VL6280X_GetRtn();
			sum_of_range += VL6180X_ReadRangeSingleMillimeters();
		}
		cross_talk = sum_of_rate*1.0f + (1-sum_of_range*1.0f/100.0f)*128;
		VL6180X_WR_CMD(SYSRANGE__CROSSTALK_COMPENSATION_RATE,cross_talk);
		Send_sprintf(PC_USART,"cross talk calibration success!\n");
		return 0 ;
    }
	else
	{
		return 1;
	}
}

/*
 * @brief TOF050C 滤波函数
 */
float filter(uint16_t raw_range)
{
	uint16_t real_range = 0;
	static uint16_t his_range = 0;
	real_range = INDEX * raw_range + (1.0-INDEX)*his_range;
	his_range = real_range;
	return real_range;
}


