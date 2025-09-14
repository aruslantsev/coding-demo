/*
 * BMP180 temperature and pressure sensor.
 * Usually uses address 0x77.
 * 1. Allocate BMP180 structure
 * 2. Call bmp180_init(BMP180 *bmp180, I2C_HandleTypeDef *hi2c, uint16_t addr)
 *    to initialize BMP180 structure. This checks if sensor is present, resets it and
 *    fetches calibration data.
 * 3. Call bmp180_set_oversampling(BMP180 *bmp180, uint8_t oss)
 *    to change oversampling setting. 0 <= oss < 4
 * 4. Call bmp180_get_temperature(BMP180 *bmp180).
 *    This fills fields ut and temperature in BMP180 structure.
 * 5. Call bmp180_get_pressure(BMP180 *bmp180)
 *    Uses results of bmp180_get_temperature.
 *    This fills fields up and pressure in BMP180 structure.
 */

typedef enum {
    BMP180_OK = 0x00U,
    BMP180_NORESPONSE = 0x01U,
    BMP180_NOTFOUND = 0x02U,
    BMP180_RESET_ERR = 0x03U,
    BMP180_WRITE_ERR = 0x04U,
    BMP180_READ_ERR = 0x05U,
    BMP180_VALUE_ERR = 0x06U,
    BMP180_TIMEOUT_ERR = 0x07U
} BMP180_STATUS;

typedef struct {
    I2C_HandleTypeDef* i2c_bus;
    uint16_t addr; // 0x77
    /* Calibration data */
    int16_t ac1;
    int16_t ac2;
    int16_t ac3;
    uint16_t ac4;
    uint16_t ac5;
    uint16_t ac6;
    int16_t b1;
    int16_t b2;
    int16_t mb;
    int16_t mc;
    int16_t md;
    /* Oversampling setting: 0 <= oss < 4 */
    uint8_t oss;
    /* Uncompensated temperature and pressure */
    int32_t ut;
    int32_t up;
    /* Compensated temperature and pressure */
    int32_t temperature; // 0.1 deg. Celsius
    int32_t pressure; // Pa
} BMP180;

/*
 * Initializes structure with i2c handler and sensor address, checks chip id,
 * sends soft reset signal, fetches calibration data
 */
BMP180_STATUS bmp180_init(BMP180* bmp180, I2C_HandleTypeDef* i2c_bus, uint16_t addr)
{
    bmp180->i2c_bus = i2c_bus;
    bmp180->addr = addr;
    bmp180->oss = 0;
    BMP180_STATUS ret;
    uint8_t buf[2];

    /* Check chip id */
    ret = HAL_I2C_Mem_Read(
        bmp180->i2c_bus,
        (bmp180->addr) << 1,
        0xD0,
        I2C_MEMADD_SIZE_8BIT,
        (uint8_t *) buf,
        1,
        100
    );
    if (ret != HAL_OK)
        return BMP180_NORESPONSE;
    if (buf[0] != 0b01010101)
        return BMP180_NOTFOUND;

    /* Soft reset */
    uint8_t reg = 0xB6;
    ret = HAL_I2C_Mem_Write(
        bmp180->i2c_bus,
        (bmp180->addr) << 1,
        0xE0,
        I2C_MEMADD_SIZE_8BIT,
        (uint8_t *) &reg,
        1,
        100
    );
    if (ret != HAL_OK)
        return BMP180_RESET_ERR;
    HAL_Delay(50);

    /* Calibration data */
    ret = HAL_I2C_Mem_Read(
        bmp180->i2c_bus,
        (bmp180->addr) << 1,
        0xAA,
        I2C_MEMADD_SIZE_8BIT,
        (uint8_t *) buf,
        2,
        100
    );
    if (ret != HAL_OK)
        return BMP180_READ_ERR;
    bmp180->ac1 = (buf[0] << 8) | buf[1];

    ret = HAL_I2C_Mem_Read(
        bmp180->i2c_bus,
        (bmp180->addr) << 1,
        0xAC,
        I2C_MEMADD_SIZE_8BIT,
        (uint8_t *) buf,
        2,
        100
    );
    if (ret != HAL_OK)
        return BMP180_READ_ERR;
    bmp180->ac2 = (buf[0] << 8) | buf[1];

    ret = HAL_I2C_Mem_Read(
        bmp180->i2c_bus,
        (bmp180->addr) << 1,
        0xAE,
        I2C_MEMADD_SIZE_8BIT,
        (uint8_t *) buf,
        2,
        100
    );
    if (ret != HAL_OK)
        return BMP180_READ_ERR;
    bmp180->ac3 = (buf[0] << 8) | buf[1];

    ret = HAL_I2C_Mem_Read(
        bmp180->i2c_bus,
        (bmp180->addr) << 1,
        0xB0,
        I2C_MEMADD_SIZE_8BIT,
        (uint8_t *) buf,
        2,
        100
    );
    if (ret != HAL_OK)
        return BMP180_READ_ERR;
    bmp180->ac4 = (buf[0] << 8) | buf[1];

    ret = HAL_I2C_Mem_Read(
        bmp180->i2c_bus,
        (bmp180->addr) << 1,
        0xB2,
        I2C_MEMADD_SIZE_8BIT,
        (uint8_t *) buf,
        2,
        100
    );
    if (ret != HAL_OK)
        return BMP180_READ_ERR;
    bmp180->ac5 = (buf[0] << 8) | buf[1];

    ret = HAL_I2C_Mem_Read(
        bmp180->i2c_bus,
        (bmp180->addr) << 1,
        0xB4,
        I2C_MEMADD_SIZE_8BIT,
        (uint8_t *) buf,
        2,
        100
    );
    if (ret != HAL_OK)
        return BMP180_READ_ERR;
    bmp180->ac6 = (buf[0] << 8) | buf[1];

    ret = HAL_I2C_Mem_Read(
        bmp180->i2c_bus,
        (bmp180->addr) << 1,
        0xB6,
        I2C_MEMADD_SIZE_8BIT,
        (uint8_t *) buf,
        2,
        100
    );
    if (ret != HAL_OK)
        return BMP180_READ_ERR;
    bmp180->b1 = (buf[0] << 8) | buf[1];

    ret = HAL_I2C_Mem_Read(
        bmp180->i2c_bus,
        (bmp180->addr) << 1,
        0xB8,
        I2C_MEMADD_SIZE_8BIT,
        (uint8_t *) buf,
        2,
        100
    );
    if (ret != HAL_OK)
        return BMP180_READ_ERR;
    bmp180->b2 = (buf[0] << 8) | buf[1];

    ret = HAL_I2C_Mem_Read(
        bmp180->i2c_bus,
        (bmp180->addr) << 1,
        0xBA,
        I2C_MEMADD_SIZE_8BIT,
        (uint8_t *) buf,
        2,
        100
    );
    if (ret != HAL_OK)
        return BMP180_READ_ERR;
    bmp180->mb = (buf[0] << 8) | buf[1];

    ret = HAL_I2C_Mem_Read(
        bmp180->i2c_bus,
        (bmp180->addr) << 1,
        0xBC,
        I2C_MEMADD_SIZE_8BIT,
        (uint8_t *) buf,
        2,
        100
    );
    if (ret != HAL_OK)
        return BMP180_READ_ERR;
    bmp180->mc = (buf[0] << 8) | buf[1];

    ret = HAL_I2C_Mem_Read(
        bmp180->i2c_bus,
        (bmp180->addr) << 1,
        0xBE,
        I2C_MEMADD_SIZE_8BIT,
        (uint8_t *) buf,
        2,
        100
    );
    if (ret != HAL_OK)
        return BMP180_READ_ERR;
    bmp180->md = (buf[0] << 8) | buf[1];
    /* Calibration data end */

    return BMP180_OK;
}

/*
 * Changes oversampling setting
 */
BMP180_STATUS bmp180_set_oversampling(BMP180* bmp180, uint8_t oss)
{
    if (oss < 0 || oss > 3)
        return BMP180_VALUE_ERR;
    bmp180->oss = oss;
    return BMP180_OK;
}

/*
 * Calculates temperature using uncompensated temperature and calibration data
 */
int32_t get_temp(BMP180* bmp180)
{
    int32_t x1 = ((bmp180->ut - bmp180->ac6) * bmp180->ac5) >> 15;
    int32_t x2 = (bmp180->mc << 11) / (x1 + bmp180->md);
    int32_t b5 = x1 + x2;
    return (b5 + 8) >> 4;
}

/*
 * Calculates pressure using uncompensated temperature, uncompensated pressure
 * and calibration data
 */
int32_t get_pressure(BMP180* bmp180)
{
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
    if (b7 < 0x80000000)
        p = (b7 * 2) / b4;
    else
        p = (b7 / b4) * 2;
    x1 = (p >> 8) * (p >> 8);
    x1 = (x1 * 3038) >> 16;
    x2 = (-7357 * p) >> 16;
    return p + ((x1 + x2 + 3791) >> 4);
}

/*
 * Reads uncompensated temperature and calculates temperature in 0.1 degrees Celsius
 */
BMP180_STATUS bmp180_get_temperature(BMP180* bmp180)
{
	BMP180_STATUS ret;
    uint8_t reg = 1 << 5 | 0b01110;
    uint8_t buf[2];

    ret = HAL_I2C_Mem_Write(
        bmp180->i2c_bus,
        (bmp180->addr) << 1,
        0xF4,
        I2C_MEMADD_SIZE_8BIT,
        (uint8_t *) &reg,
        1,
        100
    );
    if (ret != HAL_OK)
        return BMP180_WRITE_ERR;

    /* Wait for bit 5 reset in F4 */
    uint8_t retry = 0;
    do {
        ret = HAL_I2C_Mem_Read(
            bmp180->i2c_bus,
            (bmp180->addr) << 1,
            0xF4,
            I2C_MEMADD_SIZE_8BIT,
            (uint8_t *) buf,
            1,
            100
        );
        if (ret != HAL_OK)
            return BMP180_READ_ERR;
        HAL_Delay(5);
        if (retry >= 128)
            return BMP180_TIMEOUT_ERR;
        retry++;
    } while (buf[0] & (1 << 5));

    ret = HAL_I2C_Mem_Read(
        bmp180->i2c_bus,
        (bmp180->addr) << 1,
        0xF6,
        I2C_MEMADD_SIZE_8BIT,
        (uint8_t *) buf,
        2,
        100
    );
    if (ret != HAL_OK)
        return BMP180_READ_ERR;
    bmp180->ut = (buf[0] << 8) | buf[1];
    bmp180->temperature = get_temp(bmp180);
    return BMP180_OK;
}

/*
 * Reads uncompensated pressure and calculates pressure in Pa
 */
BMP180_STATUS bmp180_get_pressure(BMP180* bmp180)
{
	BMP180_STATUS ret;
    uint8_t reg = bmp180->oss << 6 | 1 << 5 | 0b10100;
    uint8_t buf[3];

    ret = HAL_I2C_Mem_Write(
        bmp180->i2c_bus,
        (bmp180->addr) << 1,
        0xF4,
        I2C_MEMADD_SIZE_8BIT,
        (uint8_t *) &reg,
        1,
        100
    );
    if (ret != HAL_OK)
        return BMP180_WRITE_ERR;

    /* Wait for bit 5 reset in F4 */
    uint8_t retry = 0;
    do {
        ret = HAL_I2C_Mem_Read(
            bmp180->i2c_bus,
            (bmp180->addr) << 1,
            0xF4,
            I2C_MEMADD_SIZE_8BIT,
            (uint8_t *) buf,
            1,
            100
        );
        if (ret != HAL_OK)
            return BMP180_READ_ERR;
        HAL_Delay(5);
        if (retry >= 128)
            return BMP180_TIMEOUT_ERR;
        retry++;
    } while (buf[0] & (1 << 5));

    ret = HAL_I2C_Mem_Read(
        bmp180->i2c_bus,
        (bmp180->addr) << 1,
        0xF6,
        I2C_MEMADD_SIZE_8BIT,
        (uint8_t *) buf,
        3,
        100
    );
    if (ret != HAL_OK)
        return BMP180_READ_ERR;
    bmp180->up = ((buf[0] << 16) | (buf[1] << 8) | buf[2]) >> (8 - bmp180->oss);
    bmp180->pressure = get_pressure(bmp180);
    return BMP180_OK;
}
