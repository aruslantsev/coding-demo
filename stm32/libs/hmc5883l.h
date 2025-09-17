/*
 * HMC5883L compass.
 * Usually uses address 0x1E.
 * 1. Allocate HMC5883L structure
 * 2. Call hmc5883l_init(HMC5883L *bmp180, I2C_HandleTypeDef *hi2c, uint16_t addr)
 *    to initialize HMC5883L structure. This checks if compass is present.
 * 3. Set gain, rate, mode, etc...
 * 4. Call bmp180_get_temperature(BMP180 *bmp180).
 *    This fills fields ut and temperature in BMP180 structure.
 * 5. Call bmp180_get_pressure(BMP180 *bmp180)
 *    Uses results of bmp180_get_temperature.
 *    This fills fields up and pressure in BMP180 structure.
 */

typedef enum {
    HMC5883L_OK             = 0x00U,
    HMC5883L_NORESPONSE     = 0x01U,
    HMC5883L_NOTFOUND       = 0x02U,
    HMC5883L_WRITE_ERR      = 0x03U,
    HMC5883L_READ_ERR       = 0x04U,
    HMC5883L_VALUE_ERR      = 0x05U,
    HMC5883L_TIMEOUT_ERR    = 0x06U
} HMC5883L_STATUS;


typedef struct {
    I2C_HandleTypeDef *i2c_bus;
    uint16_t addr; // 0x1E
    int16_t x;
    int16_t y;
    int16_t z;
} HMC5883L;


static HMC5883L_STATUS hmc5883l_get_cra(HMC5883L *hmc5883l, uint8_t *cra) {
    HAL_StatusTypeDef ret;
    ret = HAL_I2C_Mem_Read(
        hmc5883l->i2c_bus,
        (hmc5883l->addr) << 1,
        0x00,
        I2C_MEMADD_SIZE_8BIT,
        (uint8_t *) cra,
        1,
        100
    );
    if (ret != HAL_OK)
        return HMC5883L_READ_ERR;
    return HMC5883L_OK;
}


static HMC5883L_STATUS hmc5883l_set_cra(HMC5883L *hmc5883l, uint8_t cra) {
    HAL_StatusTypeDef ret;
    ret = HAL_I2C_Mem_Write(
        hmc5883l->i2c_bus,
        (hmc5883l->addr) << 1,
        0x00,
        I2C_MEMADD_SIZE_8BIT,
        (uint8_t *) &cra,
        1,
        100
    );
    if (ret != HAL_OK)
        return HMC5883L_WRITE_ERR;
    return HMC5883L_OK;
}


static HMC5883L_STATUS hmc5883l_get_crb(HMC5883L *hmc5883l, uint8_t *crb) {
    HAL_StatusTypeDef ret;
    ret = HAL_I2C_Mem_Read(
        hmc5883l->i2c_bus,
        (hmc5883l->addr) << 1,
        0x01,
        I2C_MEMADD_SIZE_8BIT,
        (uint8_t *) crb,
        1,
        100
    );
    if (ret != HAL_OK)
        return HMC5883L_READ_ERR;
    return HMC5883L_OK;
}


static HMC5883L_STATUS hmc5883l_set_cra(HMC5883L *hmc5883l, uint8_t cra) {
    HAL_StatusTypeDef ret;
    ret = HAL_I2C_Mem_Write(
        hmc5883l->i2c_bus,
        (hmc5883l->addr) << 1,
        0x00,
        I2C_MEMADD_SIZE_8BIT,
        (uint8_t *) &cra,
        1,
        100
    );
    if (ret != HAL_OK)
        return HMC5883L_WRITE_ERR;
    return HMC5883L_OK;
}


static HMC5883L_STATUS hmc5883l_get_mr(HMC5883L *hmc5883l, uint8_t *mr) {
    HAL_StatusTypeDef ret;
    ret = HAL_I2C_Mem_Read(
        hmc5883l->i2c_bus,
        (hmc5883l->addr) << 1,
        0x02,
        I2C_MEMADD_SIZE_8BIT,
        (uint8_t *) mr,
        1,
        100
    );
    if (ret != HAL_OK)
        return HMC5883L_READ_ERR;
    return HMC5883L_OK;
}


static HMC5883L_STATUS hmc5883l_set_mr(HMC5883L *hmc5883l, uint8_t mr) {
    HAL_StatusTypeDef ret;
    ret = HAL_I2C_Mem_Write(
        hmc5883l->i2c_bus,
        (hmc5883l->addr) << 1,
        0x02,
        I2C_MEMADD_SIZE_8BIT,
        (uint8_t *) &mr,
        1,
        100
    );
    if (ret != HAL_OK)
        return HMC5883L_WRITE_ERR;
    return HMC5883L_OK;
}


static HMC5883L_STATUS hmc5883l_get_sr(HMC5883L *hmc5883l, uint8_t *sr) {
    HAL_StatusTypeDef ret;
    ret = HAL_I2C_Mem_Read(
        hmc5883l->i2c_bus,
        (hmc5883l->addr) << 1,
        0x09,
        I2C_MEMADD_SIZE_8BIT,
        (uint8_t *) sr,
        1,
        100
    );
    if (ret != HAL_OK)
        return HMC5883L_READ_ERR;
    return HMC5883L_OK;
}


HMC5883L_STATUS hmc5883l_reset_mr7(HMC5883L *hmc5883l) {
    HMC5883L_STATUS ret;
    uint8_t mr;
    ret = hmc5883l_get_mr(hmc5883l, &mr);
    if (ret != HMC5883L_OK)
        return ret;
    mr = mr & 0b01111111;
    ret = hmc5883l_set_mr(hmc5883l, mr);
    if (ret != HMC5883L_OK)
        return ret;
    return HMC5883L_OK;
}


HMC5883L_STATUS hmc5883l_get_mr7(HMC5883L *hmc5883l, uint8_t *mr7_is_set) {
    HMC5883L_STATUS ret;
    uint8_t mr;
    ret = hmc5883l_get_mr(hmc5883l, &mr);
    if (ret != HMC5883L_OK)
        return ret;
    *mr7_is_set = (mr & 0b10000000) != 0? 1: 0;
    return HMC5883L_OK;
}


HMC5883L_STATUS hmc5883l_init(HMC5883L *hmc5883l, I2C_HandleTypeDef *i2c_bus, uint16_t addr) {
    hmc5883l->i2c_bus = i2c_bus;
    hmc5883l->addr = addr;
    HAL_StatusTypeDef ret;
    uint8_t buf[3];

    /* Check identification registers */
    ret = HAL_I2C_Mem_Read(
        hmc5883l->i2c_bus,
        (hmc5883l->addr) << 1,
        0x0A,
        I2C_MEMADD_SIZE_8BIT,
        (uint8_t *) buf,
        3,
        100
    );
    if (ret != HAL_OK)
        return HMC5883L_NORESPONSE;
    if (buf[0] != 0b01001000 || buf[1] != 0b00110100 || buf[2] != 0b00110011)
        return HMC5883L_NOTFOUND;

    uint8_t reg;
    ret = hmc5883l_get_cra(hmc5883l, &reg);
    if (ret != HMC5883L_OK)
        return ret;
    reg = reg & 0b01111111;
    ret = hmc5883l_set_cra(hmc5883l, reg);
    if (ret != HMC5883L_OK)
        return ret;

    ret = hmc5883l_get_crb(hmc5883l, &reg);
    if (ret != HMC5883L_OK)
        return ret;
    reg = reg & 0b11100000;
    ret = hmc5883l_set_crb(hmc5883l, reg);
    if (ret != HMC5883L_OK)
        return ret;

    ret = hmc5883l_get_mr(hmc5883l, &reg);
    if (ret != HMC5883L_OK)
        return ret;
    reg = reg & 0b10000011;
    ret = hmc5883l_set_mr(hmc5883l, reg);
    if (ret != HMC5883L_OK)
        return ret;

    ret = hmc5883l_reset_mr7(hmc5883l);
    if (ret != HMC5883L_OK)
        return ret;

    return HMC5883L_OK;
}


HMC5883L_STATUS hmc5883l_get_number_of_samples(HMC5883L *hmc5883l, uint8_t *number_of_samples) {
    uint8_t cra;
    HMC5883L_STATUS ret;
    ret = hmc5883l_get_cra(hmc5883l, &cra);
    if (ret != HMC5883L_OK)
        return ret;
    *number_of_samples = (cra & 0b01100000) >> 5;
    return HMC5883L_OK;
}


HMC5883L_STATUS hmc5883l_set_number_of_samples(HMC5883L *hmc5883l, uint8_t number_of_samples) {
    uint8_t cra;
    HMC5883L_STATUS ret;
    if (number_of_samples < 0 || number_of_samples > 3)
        return HMC5883L_VALUE_ERR;
    ret = hmc5883l_get_cra(hmc5883l, &cra);
    if (ret != HMC5883L_OK)
        return ret;
    cra = (cra & 0b10011111) | (number_of_samples << 5);
    ret = hmc5883l_set_cra(hmc5883l, cra);
    if (ret != HMC5883L_OK)
        return ret;
    return HMC5883L_OK;
}


HMC5883L_STATUS hmc5883l_get_output_rate(HMC5883L *hmc5883l, uint8_t *rate) {
    uint8_t cra;
    HMC5883L_STATUS ret;
    ret = hmc5883l_get_cra(hmc5883l, &cra);
    if (ret != HMC5883L_OK)
        return ret;
    *rate = (cra & 0b00011100) >> 2;
    return HMC5883L_OK;
}


HMC5883L_STATUS hmc5883l_set_output_rate(HMC5883L *hmc5883l, uint8_t rate) {
    uint8_t cra;
    HMC5883L_STATUS ret;
    if (rate < 0 || rate > 6)  // 7 is not used
        return HMC5883L_VALUE_ERR;
    ret = hmc5883l_get_cra(hmc5883l, &cra);
    if (ret != HMC5883L_OK)
        return ret;
    cra = (cra & 0b11100011) | (rate << 2);
    ret = hmc5883l_set_cra(hmc5883l, cra);
    if (ret != HMC5883L_OK)
        return ret;
    return HMC5883L_OK;
}


HMC5883L_STATUS hmc5883l_get_measurement_configutation(HMC5883L *hmc5883l, uint8_t *configuration) {
    uint8_t cra;
    HMC5883L_STATUS ret;
    ret = hmc5883l_get_cra(hmc5883l, &cra);
    if (ret != HMC5883L_OK)
        return ret;
    *configuration = (cra & 0b00000011);
    return HMC5883L_OK;
}


HMC5883L_STATUS hmc5883l_set_measurement_configutation(HMC5883L *hmc5883l, uint8_t configuration) {
    uint8_t cra;
    HMC5883L_STATUS ret;
    if (configuration < 0 || configuration > 2)  // 3 is not used
        return HMC5883L_VALUE_ERR;
    ret = hmc5883l_get_cra(hmc5883l, &cra);
    if (ret != HMC5883L_OK)
        return ret;
    cra = (cra & 0b11111100) | configuration;
    ret = hmc5883l_set_cra(hmc5883l, cra);
    if (ret != HMC5883L_OK)
        return ret;
    return HMC5883L_OK;
}


HMC5883L_STATUS hmc5883l_get_gain(HMC5883L *hmc5883l, uint8_t *gain) {
    uint8_t crb;
        HMC5883L_STATUS ret;
        ret = hmc5883l_get_crb(hmc5883l, &crb);
        if (ret != HMC5883L_OK)
            return ret;
        *gain = (crb & 0b11100000) >> 5;
    return HMC5883L_OK;
}


HMC5883L_STATUS hmc5883l_set_gain(HMC5883L *hmc5883l, uint8_t gain) {
    uint8_t crb;
        HMC5883L_STATUS ret;
        if (gain < 0 || gain > 7)
            return HMC5883L_VALUE_ERR;
        ret = hmc5883l_get_crb(hmc5883l, &crb);
        if (ret != HMC5883L_OK)
            return ret;
        crb = (crb & 0b00011111) | (gain << 5);
        ret = hmc5883l_set_crb(hmc5883l, crb);
        if (ret != HMC5883L_OK)
            return ret;
    return HMC5883L_OK;
}


HMC5883L_STATUS hmc5883l_get_operating_mode(HMC5883L *hmc5883l, uint8_t *mode) {
    uint8_t mr;
        HMC5883L_STATUS ret;
        ret = hmc5883l_get_mr(hmc5883l, &mr);
        if (ret != HMC5883L_OK)
            return ret;
        *mode = (mr & 0b00000011);
    return HMC5883L_OK;
}


HMC5883L_STATUS hmc5883l_set_operating_mode(HMC5883L *hmc5883l, uint8_t mode) {
    uint8_t mr;
        HMC5883L_STATUS ret;
        if (mode < 0 || mode > 3)
            return HMC5883L_VALUE_ERR;
        ret = hmc5883l_get_mr(hmc5883l, &mr);
        if (ret != HMC5883L_OK)
            return ret;
        mr = (mr & 0b11111100) | mode;
        ret = hmc5883l_set_mr(hmc5883l, mr);
        if (ret != HMC5883L_OK)
            return ret;
    return HMC5883L_OK;
}


HMC5883L_STATUS hmc5883l_read(HMC5883L* hmc5883l) {
    uint8_t buf[6];
    HAL_StatusTypeDef ret;

    ret = HAL_I2C_Mem_Read(
        hmc5883l->i2c_bus,
        (hmc5883l->addr) << 1,
        0x04,
        I2C_MEMADD_SIZE_8BIT,
        (uint8_t *) buf,
        6,
        100
    );
    if (ret != HAL_OK)
        return HMC5883L_READ_ERR;
    hmc5883l->x = ~((buf[0] << 8) | buf[1]);
    hmc5883l->y = ~((buf[2] << 8) | buf[3]);
    hmc5883l->z = ~((buf[4] << 8) | buf[4]);

    return HMC5883L_OK;
}


HMC5883L_STATUS hmc5883l_is_ready(HMC5883L *hmc5883l, uint8_t *is_ready) {
    uint8_t sr;
    HMC5883L_STATUS ret;
    ret = hmc5883l_get_sr(hmc5883l, &sr);
    if (ret != HMC5883L_OK)
        return ret;
    *is_ready = (sr & 0b00000001) != 0? 1: 0;
    return HMC5883L_OK;
}


HMC5883L_STATUS hmc5883l_is_locked(HMC5883L *hmc5883l, uint8_t *is_locked) {
    uint8_t sr;
    HMC5883L_STATUS ret;
    ret = hmc5883l_get_sr(hmc5883l, &sr);
    if (ret != HMC5883L_OK)
        return ret;
    *is_locked = (sr & 0b00000010) != 0? 1: 0;
    return HMC5883L_OK;
}


HMC5883L_STATUS hmc5883l_wait_ready(HMC5883L *hmc5883l, uint8_t timeout_ms) {
    HMC5883L_STATUS ret;
    uint8_t is_ready = 0, dt = 1;
    uint8_t curr_time = 0;
    do {
        ret = hmc5883l_is_ready(hmc5883l, &is_ready);
        if (ret != HMC5883L_OK)
            return ret;
        HAL_Delay(dt);
        curr_time += dt;
        if (curr_time >= timeout_ms)
            return HMC5883L_TIMEOUT_ERR;
    } while (!is_ready);
    return HMC5883L_OK;
}
