/**
 * @file application.cpp
 * @author Viacheslav (viacheslav@mcublog.ru)
 * @brief
 * @version 0.1
 * @date 2023-12-23
 *
 * @copyright Mcublog Copyright (c) 2023
 *
 */
#include <cstdint>

#include "app/application.h"
#include "app/io/gpio/gpio.h"
#include "app/utils/delay.h"
#include "libs/adxl345/ADXL345.h"
#include "main.h"
//>>---------------------- Log control
#define LOG_MODULE_NAME acctst
#define LOG_MODULE_LEVEL (3)
#include "app/debug/log_libs.h"
//<<----------------------

//>>---------------------- Private
extern SPI_HandleTypeDef hspi1;

#define LIS3DH_READBIT  (0x80)
#define LIS3DH_MSBIT    (0x40)

/* Read/Write command */
#define READWRITE_CMD ((uint8_t)0x80)
/* Multiple byte read/write command */
#define MULTIPLEBYTE_CMD ((uint8_t)0x40)
/* Dummy Byte Send by the SPI Master device in order to generate the Clock to the Slave
 * device */
#define DUMMY_BYTE ((uint8_t)0x00)

#define ACCELERO_CS_LOW()                                                                \
    HAL_GPIO_WritePin(ACC_nCS0_GPIO_Port, ACC_nCS0_Pin, GPIO_PIN_RESET)
#define ACCELERO_CS_HIGH()                                                               \
    HAL_GPIO_WritePin(ACC_nCS0_GPIO_Port, ACC_nCS0_Pin, GPIO_PIN_SET)

static volatile bool data_ready = true;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    static bool red = true;
    if (GPIO_Pin == ACC_INT_1_Pin)
    {
        data_ready = true;
        io_gpio_red_led(red);
        red ^= true;
        LOG_DEBUG("acc irq");
    }
}

static uint8_t SPIx_WriteRead(uint8_t byte)
{
    uint8_t receivedbyte = 0;

    /* Send a Byte through the SPI peripheral */
    /* Read byte from the SPI bus */
    if (HAL_SPI_TransmitReceive(&hspi1, (uint8_t *)&byte, (uint8_t *)&receivedbyte, 1,
                                100) != HAL_OK)
    {
        LOG_ERROR("spi error");
    }

    return receivedbyte;
}


static int8_t spi_init(void)
{
    return 0;
}

static int8_t spi_deinit(void)
{
    return 0;
}

static int8_t spi_write(uint8_t WriteAddr, uint8_t *pBuffer, uint8_t NumByteToWrite)
{
    /* Configure the MS bit:
       - When 0, the address will remain unchanged in multiple read/write commands.
       - When 1, the address will be auto incremented in multiple read/write commands.
    */
    if (NumByteToWrite > 0x01)
    {
        WriteAddr |= (uint8_t)MULTIPLEBYTE_CMD;
    }
    /* Set chip select Low at the start of the transmission */
    ACCELERO_CS_LOW();

    /* Send the Address of the indexed register */
    SPIx_WriteRead(WriteAddr);

    /* Send the data that will be written into the device (MSB First) */
    while (NumByteToWrite >= 0x01)
    {
        SPIx_WriteRead(*pBuffer);
        NumByteToWrite--;
        pBuffer++;
    }

    /* Set chip select High at the end of the transmission */
    ACCELERO_CS_HIGH();

    return 0;
}

static int8_t spi_read(uint8_t ReadAddr, uint8_t *pBuffer, uint8_t NumByteToRead)
{
    if (NumByteToRead > 0x01)
    {
        ReadAddr |= (uint8_t)(READWRITE_CMD | MULTIPLEBYTE_CMD);
    }
    else
    {
        ReadAddr |= (uint8_t)READWRITE_CMD;
    }
    /* Set chip select Low at the start of the transmission */
    ACCELERO_CS_LOW();
    /* Send the Address of the indexed register */
    // SPIx_WriteRead(ReadAddr);
    HAL_SPI_Transmit(&hspi1, &ReadAddr, 1, 10);

    /* Receive the data that will be read from the device (MSB First) */
    HAL_SPI_Receive(&hspi1, pBuffer, NumByteToRead, 10);
    // while (NumByteToRead > 0x00)
    // {
    //     /* Send dummy byte (0x00) to generate the SPI clock to ACCELEROMETER (Slave
    //      * device) */
    //     *pBuffer = SPIx_WriteRead(DUMMY_BYTE);
    //     NumByteToRead--;
    //     pBuffer++;
    // }

    /* Set chip select High at the end of the transmission */
    ACCELERO_CS_HIGH();

    return 0;
}

static ADXL345_Handler_t adxl345h = {};

//<<----------------------

/**
 * @brief Test application
 *
 */
void application(void)
{
    ACCELERO_CS_HIGH();
    delay_ms(5);

    adxl345h.AddressI2C = 0;
    adxl345h.PlatformInit = spi_init;
    adxl345h.PlatformDeInit = spi_deinit;
    adxl345h.PlatformSend = spi_write;
    adxl345h.PlatformReceive = spi_read;

    ADXL345_PowerControl_t pctrl = {};
    ADXL345_Result_t res = ADXL345_Set_PowerControl(&adxl345h,
                         &pctrl);

    while (true)
    {
        res = ADXL345_CheckDeviceID(&adxl345h);
        LOG_INFO("result: %d", res);

        delay_ms(500);
    }
}