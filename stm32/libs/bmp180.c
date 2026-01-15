#include "bmp180.h"


BMP180_STATUS bmp180_init(BMP180 *bmp180, I2C_HandleTypeDef *i2c_bus, const uint16_t addr) {
    bmp180->i2c_bus = i2c_bus;
    bmp180->addr = addr;
    bmp180->oss = 0;
    HAL_StatusTypeDef ret;
    uint8_t buf[2];

    /* Check chip id */
    ret = HAL_I2C_Mem_Read(
        bmp180->i2c_bus,
        (bmp180->addr) << 1,
        BMP180_ID_REG,
        I2C_MEMADD_SIZE_8BIT,
        (uint8_t *) buf,
        1,
        BMP180_HAL_I2C_TIMEOUT
    );
    if (ret != HAL_OK) return BMP180_NORESPONSE;
    if (buf[0] != BMP180_CHIP_ID) return BMP180_NOTFOUND;

    /* Soft reset */
    const uint8_t reg_val = BMP180_RST_VAL;
    ret = HAL_I2C_Mem_Write(
        bmp180->i2c_bus,
        (bmp180->addr) << 1,
        BMP180_SOFT_RST_REG,
        I2C_MEMADD_SIZE_8BIT,
        (uint8_t *) &reg_val,
        1,
        BMP180_HAL_I2C_TIMEOUT
    );
    if (ret != HAL_OK) return BMP180_RESET_ERR;
    HAL_Delay(BMP180_RESET_TIMEOUT);

    /* Calibration data */
    ret = HAL_I2C_Mem_Read(
        bmp180->i2c_bus,
        (bmp180->addr) << 1,
        BMP180_AC1_MSB_REG,
        I2C_MEMADD_SIZE_8BIT,
        (uint8_t *) buf,
        2,
        BMP180_HAL_I2C_TIMEOUT
    );
    if (ret != HAL_OK) return BMP180_READ_ERR;
    bmp180->ac1 = (buf[0] << 8) | buf[1];

    ret = HAL_I2C_Mem_Read(
        bmp180->i2c_bus,
        (bmp180->addr) << 1,
        BMP180_AC2_MSB_REG,
        I2C_MEMADD_SIZE_8BIT,
        (uint8_t *) buf,
        2,
        BMP180_HAL_I2C_TIMEOUT
    );
    if (ret != HAL_OK) return BMP180_READ_ERR;
    bmp180->ac2 = (buf[0] << 8) | buf[1];

    ret = HAL_I2C_Mem_Read(
        bmp180->i2c_bus,
        (bmp180->addr) << 1,
        BMP180_AC3_MSB_REG,
        I2C_MEMADD_SIZE_8BIT,
        (uint8_t *) buf,
        2,
        BMP180_HAL_I2C_TIMEOUT
    );
    if (ret != HAL_OK) return BMP180_READ_ERR;
    bmp180->ac3 = (buf[0] << 8) | buf[1];

    ret = HAL_I2C_Mem_Read(
        bmp180->i2c_bus,
        (bmp180->addr) << 1,
        BMP180_AC4_MSB_REG,
        I2C_MEMADD_SIZE_8BIT,
        (uint8_t *) buf,
        2,
        BMP180_HAL_I2C_TIMEOUT
    );
    if (ret != HAL_OK) return BMP180_READ_ERR;
    bmp180->ac4 = (buf[0] << 8) | buf[1];

    ret = HAL_I2C_Mem_Read(
        bmp180->i2c_bus,
        (bmp180->addr) << 1,
        BMP180_AC5_MSB_REG,
        I2C_MEMADD_SIZE_8BIT,
        (uint8_t *) buf,
        2,
        BMP180_HAL_I2C_TIMEOUT
    );
    if (ret != HAL_OK) return BMP180_READ_ERR;
    bmp180->ac5 = (buf[0] << 8) | buf[1];

    ret = HAL_I2C_Mem_Read(
        bmp180->i2c_bus,
        (bmp180->addr) << 1,
        BMP180_AC6_MSB_REG,
        I2C_MEMADD_SIZE_8BIT,
        (uint8_t *) buf,
        2,
        BMP180_HAL_I2C_TIMEOUT
    );
    if (ret != HAL_OK) return BMP180_READ_ERR;
    bmp180->ac6 = (buf[0] << 8) | buf[1];

    ret = HAL_I2C_Mem_Read(
        bmp180->i2c_bus,
        (bmp180->addr) << 1,
        BMP180_B1_MSB_REG,
        I2C_MEMADD_SIZE_8BIT,
        (uint8_t *) buf,
        2,
        BMP180_HAL_I2C_TIMEOUT
    );
    if (ret != HAL_OK) return BMP180_READ_ERR;
    bmp180->b1 = (buf[0] << 8) | buf[1];

    ret = HAL_I2C_Mem_Read(
        bmp180->i2c_bus,
        (bmp180->addr) << 1,
        BMP180_B2_MSB_REG,
        I2C_MEMADD_SIZE_8BIT,
        (uint8_t *) buf,
        2,
        BMP180_HAL_I2C_TIMEOUT
    );
    if (ret != HAL_OK) return BMP180_READ_ERR;
    bmp180->b2 = (buf[0] << 8) | buf[1];

    ret = HAL_I2C_Mem_Read(
        bmp180->i2c_bus,
        (bmp180->addr) << 1,
        BMP180_MB_MSB_REG,
        I2C_MEMADD_SIZE_8BIT,
        (uint8_t *) buf,
        2,
        BMP180_HAL_I2C_TIMEOUT
    );
    if (ret != HAL_OK) return BMP180_READ_ERR;
    bmp180->mb = (buf[0] << 8) | buf[1];

    ret = HAL_I2C_Mem_Read(
        bmp180->i2c_bus,
        (bmp180->addr) << 1,
        BMP180_MC_MSB_REG,
        I2C_MEMADD_SIZE_8BIT,
        (uint8_t *) buf,
        2,
        BMP180_HAL_I2C_TIMEOUT
    );
    if (ret != HAL_OK) return BMP180_READ_ERR;
    bmp180->mc = (buf[0] << 8) | buf[1];

    ret = HAL_I2C_Mem_Read(
        bmp180->i2c_bus,
        (bmp180->addr) << 1,
        BMP180_MD_MSB_REG,
        I2C_MEMADD_SIZE_8BIT,
        (uint8_t *) buf,
        2,
        BMP180_HAL_I2C_TIMEOUT
    );
    if (ret != HAL_OK) return BMP180_READ_ERR;
    bmp180->md = (buf[0] << 8) | buf[1];
    /* Calibration data end */

    return BMP180_OK;
}


BMP180_STATUS bmp180_set_oversampling(BMP180 *bmp180, const BMP180_OSS oss) {
    if (oss < BMP180_OSS_SINGLE || oss > BMP180_OSS_OCT)
        return BMP180_VALUE_ERR;
    bmp180->oss = oss;

    return BMP180_OK;
}


/**
 * @brief Calculates temperature using uncompensated temperature and calibration data
 *
 * @param BMP180 *: pointer to allocated and initialized structure
 * @return uint32_t real temperature
 */
static int32_t get_temp(BMP180 *bmp180) {
    int32_t x1 = ((bmp180->ut - bmp180->ac6) * bmp180->ac5) >> 15;
    int32_t x2 = (bmp180->mc << 11) / (x1 + bmp180->md);
    int32_t b5 = x1 + x2;
    return (b5 + 8) >> 4;
}


/**
 * @brief Calculates pressure using uncompensated temperature, uncompensated pressure
 *        and calibration data
 *
* @param BMP180 *: pointer to allocated and initialized structure
 * @return uint32_t real pressure
 */
static int32_t get_pressure(BMP180 *bmp180) {
    int32_t x1 = ((bmp180->ut - bmp180->ac6) * bmp180->ac5) >> 15;
    int32_t x2 = (bmp180->mc << 11) / (x1 + bmp180->md);
    int32_t b5 = x1 + x2;
    int32_t b6 = b5 - 4000;
    x1 = (bmp180->b2 * ((b6 * b6) >> 12)) >> 11;
    x2 = (bmp180->ac2 * b6) >> 11;
    int32_t x3 = x1 + x2;
    int32_t b3 = (((bmp180->ac1 * 4 + x3) << bmp180->oss) + 2) / 4;
    x1 = (bmp180->ac3 * b6) >> 13;
    x2 = (bmp180->b1 * ((b6 * b6) >> 12)) >> 16;
    x3 = ((x1 + x2) + 2) >> 2;
    uint32_t b4 = (bmp180->ac4 * (uint32_t)(x3 + 32768)) >> 15;
    uint32_t b7 = ((uint32_t)bmp180->up - b3) * (50000 >> bmp180->oss);
    int32_t p;
    if (b7 < 0x80000000) {
        p = (b7 * 2) / b4;
    } else {
        p = (b7 / b4) * 2;
    }
    x1 = (p >> 8) * (p >> 8);
    x1 = (x1 * 3038) >> 16;
    x2 = (-7357 * p) >> 16;
    return p + ((x1 + x2 + 3791) >> 4);
}


BMP180_STATUS bmp180_get_temperature(BMP180 *bmp180) {
    HAL_StatusTypeDef ret;
    const uint8_t reg = BMP180_TEMP_MEAS_VAL;
    uint8_t buf[2];

    ret = HAL_I2C_Mem_Write(
        bmp180->i2c_bus,
        (bmp180->addr) << 1,
        BMP180_CTRL_MEAS_REG,
        I2C_MEMADD_SIZE_8BIT,
        (uint8_t *) &reg,
        1,
        BMP180_HAL_I2C_TIMEOUT
    );
    if (ret != HAL_OK) return BMP180_WRITE_ERR;

    /* Wait for bit 5 reset in F4 */
    uint8_t retry = 0;
    do {
        ret = HAL_I2C_Mem_Read(
            bmp180->i2c_bus,
            (bmp180->addr) << 1,
            BMP180_CTRL_MEAS_REG,
            I2C_MEMADD_SIZE_8BIT,
            (uint8_t *) buf,
            1,
            BMP180_HAL_I2C_TIMEOUT
        );
        if (ret != HAL_OK) return BMP180_READ_ERR;
        HAL_Delay(BMP180_MEAS_TIMEOUT);
        if (retry > BMP180_MAX_RETRIES) return BMP180_TIMEOUT_ERR;
        retry++;
    } while (buf[0] & (1 << BMP180_SCO_BIT));

    ret = HAL_I2C_Mem_Read(
        bmp180->i2c_bus,
        (bmp180->addr) << 1,
        BMP180_OUT_MSB_REG,
        I2C_MEMADD_SIZE_8BIT,
        (uint8_t *) buf,
        2,
        BMP180_HAL_I2C_TIMEOUT
    );
    if (ret != HAL_OK) return BMP180_READ_ERR;
    bmp180->ut = (buf[0] << 8) | buf[1];
    bmp180->temperature = get_temp(bmp180);
    return BMP180_OK;
}


BMP180_STATUS bmp180_get_pressure(BMP180 *bmp180) {
    HAL_StatusTypeDef ret;
    uint8_t reg = (bmp180->oss << BMP180_OSS_LSB_BIT) | BMP180_PRES_MEAS_VAL;
    uint8_t buf[3];

    ret = HAL_I2C_Mem_Write(
        bmp180->i2c_bus,
        (bmp180->addr) << 1,
        BMP180_CTRL_MEAS_REG,
        I2C_MEMADD_SIZE_8BIT,
        (uint8_t *) &reg,
        1,
        BMP180_HAL_I2C_TIMEOUT
    );
    if (ret != HAL_OK) return BMP180_WRITE_ERR;

    /* Wait for bit 5 reset in F4 */
    uint8_t retry = 0;
    do {
        ret = HAL_I2C_Mem_Read(
            bmp180->i2c_bus,
            (bmp180->addr) << 1,
            BMP180_CTRL_MEAS_REG,
            I2C_MEMADD_SIZE_8BIT,
            (uint8_t *) buf,
            1,
            BMP180_HAL_I2C_TIMEOUT
        );
        if (ret != HAL_OK) return BMP180_READ_ERR;
        HAL_Delay(BMP180_MEAS_TIMEOUT);
        if (retry > BMP180_MAX_RETRIES) return BMP180_TIMEOUT_ERR;
        retry++;
    } while (buf[0] & (1 << BMP180_SCO_BIT));

    ret = HAL_I2C_Mem_Read(
        bmp180->i2c_bus,
        (bmp180->addr) << 1,
        BMP180_OUT_MSB_REG,
        I2C_MEMADD_SIZE_8BIT,
        (uint8_t *) buf,
        3,
        BMP180_HAL_I2C_TIMEOUT
    );
    if (ret != HAL_OK) return BMP180_READ_ERR;
    bmp180->up = ((buf[0] << 16) | (buf[1] << 8) | buf[2]) >> (8 - bmp180->oss);
    bmp180->pressure = get_pressure(bmp180);
    return BMP180_OK;
}


BMP180_STATUS bmp180_get_all(BMP180 *bmp180) {
    const BMP180_STATUS status_temperature = bmp180_get_temperature(bmp180);
    const BMP180_STATUS status_pressure = bmp180_get_pressure(bmp180);
    if (status_pressure != BMP180_OK || status_temperature != BMP180_OK) return BMP180_READ_ERR;
    return BMP180_OK;
}
