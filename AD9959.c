/*
 * AD9959.c
 *
 *  Created on: Jul 27, 2022
 *      Author: Vergil
 */


#include "AD9959.h"
#define BASE_FREQ 8589.935 // 1 kHz

uint8_t FR2_DATA[2] = {0x20, 0x00};//default Value = 0x0000
uint8_t CFR_DATA[3] = {0x00, 0x03, 0x02};//default Value = 0x000302
uint8_t CSR_DATA0[1] = {0x10};     // 开 CH0
uint8_t CSR_DATA1[1] = {0x20};      // 开 CH1
uint8_t CSR_DATA2[1] = {0x40};      // 开 CH2
uint8_t CSR_DATA3[1] = {0x80};      // 开 CH3

uint8_t CPOW0_DATA[2] = {0x00, 0x00};//default Value = 0x0000   @ = POW/2^14*360

uint8_t LSRR_DATA[2] = {0x00, 0x00};//default Value = 0x----

uint8_t RDW_DATA[4] = {0x00, 0x00, 0x00, 0x00};//default Value = 0x--------

uint8_t FDW_DATA[4] = {0x00, 0x00, 0x00, 0x00};//default Value = 0x--------

/**
* @brief AD9959延迟函数
* */
static void ad9959_delay(uint32_t length) {
    length = length * 12;
    while (length--);
}

/**
* @brief 这个函数用于初始化AD9959。它首先调用ad9959_io_init()和ad9959_reset()来初始化IO并重置AD9959。
* 然后，它写入一些默认值到FR1和FR2寄存器，并为每个通道设置默认的频率、相位和幅度。
* */
void ad9959_init(void) {
    uint8_t FR1_DATA[3] = {0xD0, 0x00,
                           0x00};//20��棰� Charge pump control = 75uA FR1<23> -- VCO gain control =0�� system clock below 160 MHz;
    //重置IO 和 内部寄存器

    ad9959_io_init();
    ad9959_reset();

    //将默认值写入寄存器
    ad9959_write_data(AD9959_REG_FR1, 3, FR1_DATA, 1);//
    ad9959_write_data(AD9959_REG_FR2, 2, FR2_DATA, 1);

    //设置值频率，相位，，幅度的默认值
    ad9959_write_frequency(AD9959_CHANNEL_0, 1);
    ad9959_write_frequency(AD9959_CHANNEL_1, 1);
    ad9959_write_frequency(AD9959_CHANNEL_2, 1);
    ad9959_write_frequency(AD9959_CHANNEL_3, 1);

    ad9959_write_phase(AD9959_CHANNEL_0, 0);
    ad9959_write_phase(AD9959_CHANNEL_1, 0);
    ad9959_write_phase(AD9959_CHANNEL_2, 0);
    ad9959_write_phase(AD9959_CHANNEL_3, 0);

    ad9959_write_amplitude(AD9959_CHANNEL_0, 0xFF);
    ad9959_write_amplitude(AD9959_CHANNEL_1, 0xFF);
    ad9959_write_amplitude(AD9959_CHANNEL_2, 0xFF);
    ad9959_write_amplitude(AD9959_CHANNEL_3, 0xFF);
}

/**
* @brief 这个函数用于重置AD9959。它通过操作RESET引脚来实现。
* */
void ad9959_reset(void) {
    AD9959_RESET_0;
    ad9959_delay(1);
    AD9959_RESET_1;
    ad9959_delay(30);
    AD9959_RESET_0;
}

/**
* @brief 这个函数用于初始化AD9959的IO引脚。
* */
void ad9959_io_init(void) {

    AD9959_PDC_0;
    AD9959_CS_1;
    AD9959_SCLK_0;
    AD9959_UPDATE_0;
    AD9959_PS0_0;
    AD9959_PS1_0;
    AD9959_PS2_0;
    AD9959_PS3_0;
    AD9959_SDIO0_0;
    AD9959_SDIO1_0;
    AD9959_SDIO2_0;
    AD9959_SDIO3_0;
}

/**
 * @brief 这个函数用于更新AD9959的IO更新引脚。当你向AD9959写入新的数据后，需要调用这个函数来使新的数据生效。
 * */
void ad9959_io_update(void) {
    AD9959_UPDATE_0;
    ad9959_delay(2);
    AD9959_UPDATE_1;
    ad9959_delay(4);
    AD9959_UPDATE_0;
}

/**
 * @brief 这个函数用于向AD9959写入数据。你可以指定寄存器地址、寄存器数量以及寄存器数据。
 * 此外，你还可以选择是否在写入数据后立即更新IO。
 * */
void ad9959_write_data(AD9959_REG_ADDR register_address, uint8_t number_of_registers, const uint8_t *register_data,
                       bool update) {
    uint8_t ControlValue = 0;
    uint8_t ValueToWrite = 0;
    uint8_t RegisterIndex = 0;
    uint8_t i = 0;

    assert_param(IS_AD9959_REG_ADDR(register_address));

    ControlValue = register_address;
//���ュ�板��
    AD9959_SCLK_0;
    AD9959_CS_0;
    for (i = 0; i < 8; i++) {
        AD9959_SCLK_0;
        if (0x80 == (ControlValue & 0x80))
            AD9959_SDIO0_1;
        else
            AD9959_SDIO0_0;
        AD9959_SCLK_1;
        ControlValue <<= 1;
    }
    AD9959_SCLK_0;
//���ユ�版��
    for (RegisterIndex = 0; RegisterIndex < number_of_registers; RegisterIndex++) {
        ValueToWrite = register_data[RegisterIndex];
        for (i = 0; i < 8; i++) {
            AD9959_SCLK_0;
            if (0x80 == (ValueToWrite & 0x80))
                AD9959_SDIO0_1;
            else
                AD9959_SDIO0_0;
            AD9959_SCLK_1;
            ValueToWrite <<= 1;
        }
        AD9959_SCLK_0;
    }
    if (update) ad9959_io_update();
    AD9959_CS_1;
}

/**
 * @brief 这个函数用于设置指定通道的相位。你可以为每个通道设置一个14位的相位值，范围是0~16383，对应0°~360°。
 * */
void ad9959_write_phase(AD9959_CHANNEL channel, uint16_t phase) {
    uint8_t cs_data = channel;
    assert_param(IS_AD9959_CHANNEL(channel));
    CPOW0_DATA[1] = (uint8_t) phase;
    CPOW0_DATA[0] = (uint8_t) (phase >> 8);
    ad9959_write_data(AD9959_REG_CSR, 1, &cs_data, 1);
    ad9959_write_data(AD9959_REG_CPOW0, 2, CPOW0_DATA, 1);

}

/**
 * @brief 这个函数用于设置指定通道的频率。你可以为每个通道设置一个频率值，范围是1 ~ 200000000Hz。最大200M Hz
 * */

void ad9959_write_frequency(AD9959_CHANNEL channel, uint32_t FreqMultiplier) {
    uint8_t CFTW0_DATA[4] = {0x00, 0x00, 0x00, 0x00};
    uint64_t frequency;
    uint8_t cs_data = channel;

    assert_param(IS_AD9959_CHANNEL(channel));

    frequency = (uint64_t) FreqMultiplier * BASE_FREQ ;
    CFTW0_DATA[3] = (uint32_t) frequency;
    CFTW0_DATA[2] = (uint32_t) (frequency >> 8);
    CFTW0_DATA[1] = (uint32_t) (frequency >> 16);
    CFTW0_DATA[0] = (uint32_t) (frequency >> 24);

    ad9959_write_data(AD9959_REG_CSR, 1, &cs_data, 1);
    ad9959_write_data(AD9959_REG_CFTW0, 4, CFTW0_DATA, 1);
}



/**
 * @brief 这个函数用于设置指定通道的幅度。你可以为每个通道设置一个10位的幅度值，范围是0~1023，对应0 ~ 530mV。
 * */
void ad9959_write_amplitude(AD9959_CHANNEL channel, uint16_t amplitude) {
    uint8_t ACR_DATA[3] = {0x00, 0x00, 0x00};//default Value = 0x--0000 Rest = 18.91/Iout
    uint8_t cs_data = channel;

    assert_param(IS_AD9959_CHANNEL(channel));

    amplitude = amplitude | 0x1000;
    ACR_DATA[2] = (uint8_t) amplitude;
    ACR_DATA[1] = (uint8_t) (amplitude >> 8);

    ad9959_write_data(AD9959_REG_CSR, 1, &cs_data, 1);
    ad9959_write_data(AD9959_REG_ACR, 3, ACR_DATA, 1);

}
void AD9959ChangeCurrent()
{
	uint8_t CFR1_DATA[3] = {0x00, 0x01, 0x02};

	ad9959_write_data(AD9959_REG_CSR,1,CSR_DATA0,1);
	ad9959_write_data(AD9959_REG_CFR,3,CFR1_DATA,1);
	ad9959_write_data(AD9959_REG_CSR,1,CSR_DATA2,1);
	ad9959_write_data(AD9959_REG_CFR,3,CFR1_DATA,1);

}
