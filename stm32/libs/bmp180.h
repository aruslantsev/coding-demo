#ifndef BMP180_H
#define BMP180_H

/*
 * BMP180 temperature and pressure sensor.
 * Usually uses address 0x77.
 * 1. Define macro for your MCU
 * 2. Allocate BMP180 structure
 * 3. Call bmp180_init to initialize BMP180 structure. This checks if sensor is 
 *    present, resets it and fetches calibration data.
 * 4. Call bmp180_set_oversampling to change oversampling setting.
 * 5. Call bmp180_get_temperature. This fills fields ut and temperature in BMP180 structure.
 * 6. Call bmp180_get_pressure. Uses results of bmp180_get_temperature.
 *    This fills fields up and pressure in BMP180 structure.
 */

#if defined(STM32WB)
#include "stm32wbxx_hal.h"
#elif defined(STM32F0)
#include "stm32f0xx_hal.h"
#elif defined(STM32F1)
#include "stm32f1xx_hal.h"
#elif defined(STM32F4)
#include "stm32f4xx_hal.h"
#elif defined(STM32L0)
#include "stm32l0xx_hal.h"
#elif defined(STM32L1)
#include "stm32l1xx_hal.h"
#elif defined(STM32L4)
#include "stm32l4xx_hal.h"
#elif defined(STM32L5)
#include "stm32l5xx_hal.h"
#elif defined(STM32F3)
#include "stm32f3xx_hal.h"
#elif defined(STM32H5)
#include "stm32h5xx_hal.h"
#elif defined(STM32H7)
#include "stm32h7xx_hal.h"
#elif defined(STM32F7)
#include "stm32f7xx_hal.h"
#elif defined(STM32G0)
#include "stm32g0xx_hal.h"
#elif defined(STM32G4)
#include "stm32g4xx_hal.h"
#elif defined(STM32C0)
#include "stm32c0xx_hal.h"
#elif defined(STM32U5)
#include "stm32u5xx_hal.h"
#else
#error "BMP180H library was tested only on STM32F0, STM32F1, STM32F3, STM32F4, \
STM32F7, STM32L0, STM32L1, STM32L4, STM32H7, STM32G0, STM32G4, STM32WB, STM32C0, \
STM32U5 MCU families. Please modify bmp180h.h if you know what you are doing. \
Also please send a pull request if it turns out the library works on other MCU's as well!"
#endif


/* Possible device statuses */
typedef enum {
    BMP180_OK           = 0x00U,
    BMP180_NORESPONSE   = 0x01U,
    BMP180_NOTFOUND     = 0x02U,
    BMP180_RESET_ERR    = 0x03U,
    BMP180_WRITE_ERR    = 0x04U,
    BMP180_READ_ERR     = 0x05U,
    BMP180_VALUE_ERR    = 0x06U,
    BMP180_TIMEOUT_ERR  = 0x07U
} BMP180_STATUS;


/* Possible values of oversampling setting */
typedef enum {
    BMP180_OSS_SINGLE   = 0b00,
    BMP180_OSS_DOUBLE   = 0b01,
    BMP180_OSS_QUAD     = 0b10,
    BMP180_OSS_OCT      = 0b11
} BMP180_OSS;


typedef struct {
    I2C_HandleTypeDef   *i2c_bus;
    uint16_t addr; // 0x77
    /* Calibration data */
    int16_t             ac1;
    int16_t             ac2;
    int16_t             ac3;
    uint16_t            ac4;
    uint16_t            ac5;
    uint16_t            ac6;
    int16_t             b1;
    int16_t             b2;
    int16_t             mb;
    int16_t             mc;
    int16_t             md;
    /* Oversampling setting: 0b00 <= oss <= 0b11 */
    uint8_t             oss;
    /* Uncompensated temperature and pressure */
    int32_t             ut;
    int32_t             up;
    /* Compensated temperature and pressure */
    int32_t             temperature; // 0.1 deg. Celsius
    int32_t             pressure; // Pa
} BMP180;


/**
 * @brief Initialize BMP180 structure
 *
 * Initializes structure with i2c handler and sensor address, checks chip id,
 * sends soft reset signal, fetches calibration data
 *
 * @param BMP180 *: pointer to allocated structure, it will be initialized during call
 * @param I2C_HandleTypeDef *: pointer to I2C handle
 * @param uint16_t: device address, usually 0x77
 * @return BMP180_STATUS
 */
BMP180_STATUS bmp180_init(BMP180 *bmp180, I2C_HandleTypeDef *i2c_bus, const uint16_t addr);


/**
 * @brief Changes oversampling setting
 *
 * @param BMP180 *: pointer to allocated and initialized structure
 * @param BMP180_OSS: oversampling setting. Possible values: 
 *                    BMP180_OSS_SINGLE, BMP180_OSS_DOUBLE, BMP180_OSS_QUAD, BMP180_OSS_OCT
 * @return BMP180_STATUS
 */
BMP180_STATUS bmp180_set_oversampling(BMP180 *bmp180, const BMP180_OSS oss);


/**
 * @brief Reads uncompensated temperature and calculates temperature in 0.1 degrees Celsius
 *
 * Fills fields ut and temperature in the structure. ut is the raw value of output registers. 
 * temperature is real temperature value in 0.1 deg. Celsius, calculated using internal calibration data
 *
 * @return BMP180_STATUS
 */
BMP180_STATUS bmp180_get_temperature(BMP180 *bmp180);


/**
 * @brief Reads uncompensated pressure and calculates pressure in Pa
 *
 * Fills fields up and pressure in the structure. up is the raw value of output registers. 
 * pressure is real pressure value in Pa, calculated using internal calibration data
 *
 * @return BMP180_STATUS
 */
BMP180_STATUS bmp180_get_pressure(BMP180 *bmp180);


/**
 * @brief Performs calls of bmp180_get_temperature and bmp180_get_pressure
 *
 * Calculates temperature and pressure
 *
 * @return BMP180_STATUS
 */
BMP180_STATUS bmp180_get_all(BMP180 *bmp180);


#define BMP180_AC1_MSB_REG      0xAA
#define BMP180_AC1_LSB_REG      0xAB
#define BMP180_AC2_MSB_REG      0xAC
#define BMP180_AC2_LSB_REG      0xAD
#define BMP180_AC3_MSB_REG      0xAE
#define BMP180_AC3_LSB_REG      0xAF
#define BMP180_AC4_MSB_REG      0xB0
#define BMP180_AC4_LSB_REG      0xB1
#define BMP180_AC5_MSB_REG      0xB2
#define BMP180_AC5_LSB_REG      0xB3
#define BMP180_AC6_MSB_REG      0xB4
#define BMP180_AC6_LSB_REG      0xB5
#define BMP180_B1_MSB_REG       0xB6
#define BMP180_B1_LSB_REG       0xB7
#define BMP180_B2_MSB_REG       0xB8
#define BMP180_B2_LSB_REG       0xB9
#define BMP180_MB_MSB_REG       0xBA
#define BMP180_MB_LSB_REG       0xBB
#define BMP180_MC_MSB_REG       0xBC
#define BMP180_MC_LSB_REG       0xBD
#define BMP180_MD_MSB_REG       0xBE
#define BMP180_MD_LSB_REG       0xBF

#define BMP180_CTRL_MEAS_REG    0xF4

#define BMP180_OUT_MSB_REG      0xF6
#define BMP180_OUT_LSB_REG      0xF7
#define BMP180_OUT_XLSB_REG     0xF8

#define BMP180_ID_REG           0xD0
#define BMP180_SOFT_RST_REG     0xE0

#define BMP180_CHIP_ID          0x55

#define BMP180_RST_VAL          0xB6

#define BMP180_SCO_BIT          5
#define BMP180_OSS_LSB_BIT      6 /* Bits 6 and 7,  zero indexed */

#define BMP180_TEMP_MEAS_VAL    0x2E
#define BMP180_PRES_MEAS_VAL    0x34

#define BMP180_MAX_RETRIES      128

#define BMP180_HAL_I2C_TIMEOUT  128
#define BMP180_RESET_TIMEOUT    50
#define BMP180_MEAS_TIMEOUT     5

#endif
