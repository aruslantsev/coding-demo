/*
 * HMC5883L compass.
 * Usually uses address 0x1E.
 * 1. Allocate HMC5883L structure
 * 2. Call hmc5883l_init(HMC5883L *hmc5883l, I2C_HandleTypeDef *hi2c, uint16_t addr)
 *    to initialize HMC5883L structure. This checks if compass is present.
 * 3. Set gain, rate, mode, etc...
 * 4. Call hmc5883l_single_measurement(HMC5883L *hmc5883l)
 *    or hmc5883l_continuous_measurement(HMC5883L *hmc5883l)
 * 5. Read fields x, y, z of hmc5883l
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


/*
 * Copies Configuration register A (CRA) to uint8_t *
 */
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


/*
 * Copies specified uint8_t value to Configuration register A (CRA)
 */
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


/*
 * Copies Configuration register B (CRB) to uint8_t *
 */
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


/*
 * Copies specified uint8_t value to Configuration register B (CRB)
 */
static HMC5883L_STATUS hmc5883l_set_crb(HMC5883L *hmc5883l, uint8_t crb) {
    HAL_StatusTypeDef ret;
    ret = HAL_I2C_Mem_Write(
        hmc5883l->i2c_bus,
        (hmc5883l->addr) << 1,
        0x01,
        I2C_MEMADD_SIZE_8BIT,
        (uint8_t *) &crb,
        1,
        100
    );
    if (ret != HAL_OK)
        return HMC5883L_WRITE_ERR;
    return HMC5883L_OK;
}


/*
 * Copies Mode register (MR) to uint8_t *
 */
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


/*
 * Copies specified uint8_t value to Mode register (MR)
 */
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


/*
 * Copies Status register (SR) to uint8_t *
 */
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


/*
 * Sets MSB of MR to 0
 */
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


/*
 * Copies MSB of MR to mr7_is_set
 */
HMC5883L_STATUS hmc5883l_get_mr7(HMC5883L *hmc5883l, uint8_t *mr7_is_set) {
    HMC5883L_STATUS ret;
    uint8_t mr;
    ret = hmc5883l_get_mr(hmc5883l, &mr);
    if (ret != HMC5883L_OK)
        return ret;
    *mr7_is_set = (mr & 0b10000000) != 0? 1: 0;
    return HMC5883L_OK;
}


/*
 * Initializes structure with bus handler and address, checks if compass is present.
 * Resets some bits of registers
 */
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

    /* Reset bits in registers, see datasheet */
    HMC5883L_STATUS cret;
    uint8_t reg;
    cret = hmc5883l_get_cra(hmc5883l, &reg);
    if (cret != HMC5883L_OK)
        return cret;
    reg = reg & 0b01111111;
    cret = hmc5883l_set_cra(hmc5883l, reg);
    if (cret != HMC5883L_OK)
        return cret;

    cret = hmc5883l_get_crb(hmc5883l, &reg);
    if (cret != HMC5883L_OK)
        return cret;
    reg = reg & 0b11100000;
    cret = hmc5883l_set_crb(hmc5883l, reg);
    if (cret != HMC5883L_OK)
        return cret;

    ret = hmc5883l_get_mr(hmc5883l, &reg);
    if (cret != HMC5883L_OK)
        return cret;
    reg = reg & 0b10000011;
    cret = hmc5883l_set_mr(hmc5883l, reg);
    if (cret != HMC5883L_OK)
        return cret;

    cret = hmc5883l_reset_mr7(hmc5883l);
    if (cret != HMC5883L_OK)
        return cret;

    return HMC5883L_OK;
}


/*
 * Get averaged number of samples per measurement
 * 0U = 1, 1U, 2U = 4, 3U = 8
 */
HMC5883L_STATUS hmc5883l_get_number_of_samples(HMC5883L *hmc5883l, uint8_t *number_of_samples) {
    uint8_t cra;
    HMC5883L_STATUS ret;
    ret = hmc5883l_get_cra(hmc5883l, &cra);
    if (ret != HMC5883L_OK)
        return ret;
    *number_of_samples = (cra & 0b01100000) >> 5;
    return HMC5883L_OK;
}


/*
 * Set averaged number of samples per measurement
 * 0U = 1, 1U, 2U = 4, 3U = 8 (default)
 */
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


/*
 * Gets data output rate
 * 0U = 0.75Hz, 1U = 1.5Hz, 2U = 3Hz, 3U = 7.5Hz, 4U = 15Hz (default),
 * 5U = 30Hz, 6U = 75Hz, 7U NOT USED
 */
HMC5883L_STATUS hmc5883l_get_output_rate(HMC5883L *hmc5883l, uint8_t *rate) {
    uint8_t cra;
    HMC5883L_STATUS ret;
    ret = hmc5883l_get_cra(hmc5883l, &cra);
    if (ret != HMC5883L_OK)
        return ret;
    *rate = (cra & 0b00011100) >> 2;
    return HMC5883L_OK;
}


/*
 * Sets data output rate
 * 0U = 0.75Hz, 1U = 1.5Hz, 2U = 3Hz, 3U = 7.5Hz, 4U = 15Hz (default),
 * 5U = 30Hz, 6U = 75Hz, 7U NOT USED
 */
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


/*
 * Gets measurement configuration
 * 0U = Normal (default), 1U = positive bias, 2U = negative bias, 3U NOT USED
 */
HMC5883L_STATUS hmc5883l_get_measurement_configuration(HMC5883L *hmc5883l, uint8_t *configuration) {
    uint8_t cra;
    HMC5883L_STATUS ret;
    ret = hmc5883l_get_cra(hmc5883l, &cra);
    if (ret != HMC5883L_OK)
        return ret;
    *configuration = (cra & 0b00000011);
    return HMC5883L_OK;
}


/*
 * Sets measurement configuration
 * 0U = Normal (default), 1U = positive bias, 2U = negative bias, 3U NOT USED
 */
HMC5883L_STATUS hmc5883l_set_measurement_configuration(HMC5883L *hmc5883l, uint8_t configuration) {
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


/*
 * Sets gain
 * Value        Recommended sensor  Gain        Digital resolution  Output range
                field range         LSb/Gauss   mG/LSb
 * 0U           +-0.88Ga            1370        0.73                0xF800-0x07FF (-2048-2047)
 * 1U (default) +-1.3               1090        0.92                0xF800-0x07FF (-2048-2047)
 * 2U           +-1.9               820         1.22                0xF800-0x07FF (-2048-2047)
 * 3U           +-2.5               660         1.52                0xF800-0x07FF (-2048-2047)
 * 4U           +-4.0               440         2.27                0xF800-0x07FF (-2048-2047)
 * 5U           +-4.7               390         2.56                0xF800-0x07FF (-2048-2047)
 * 6U           +-5.6               330         3.03                0xF800-0x07FF (-2048-2047)
 * 7U           +-8.1               230         4.35                0xF800-0x07FF (-2048-2047)
 */
HMC5883L_STATUS hmc5883l_get_gain(HMC5883L *hmc5883l, uint8_t *gain) {
    uint8_t crb;
        HMC5883L_STATUS ret;
        ret = hmc5883l_get_crb(hmc5883l, &crb);
        if (ret != HMC5883L_OK)
            return ret;
        *gain = (crb & 0b11100000) >> 5;
    return HMC5883L_OK;
}


/*
 * Gets gain
 * Value        Recommended sensor  Gain        Digital resolution  Output range
                field range         LSb/Gauss   mG/LSb
 * 0U           +-0.88Ga            1370        0.73                0xF800-0x07FF (-2048-2047)
 * 1U (default) +-1.3               1090        0.92                0xF800-0x07FF (-2048-2047)
 * 2U           +-1.9               820         1.22                0xF800-0x07FF (-2048-2047)
 * 3U           +-2.5               660         1.52                0xF800-0x07FF (-2048-2047)
 * 4U           +-4.0               440         2.27                0xF800-0x07FF (-2048-2047)
 * 5U           +-4.7               390         2.56                0xF800-0x07FF (-2048-2047)
 * 6U           +-5.6               330         3.03                0xF800-0x07FF (-2048-2047)
 * 7U           +-8.1               230         4.35                0xF800-0x07FF (-2048-2047)
 */
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


/*
 * Gets operating mode
 * 0U = Continuous measurement, 1U = Single measurement (default), 2U, 3U = Idle
 */
HMC5883L_STATUS hmc5883l_get_operating_mode(HMC5883L *hmc5883l, uint8_t *mode) {
    uint8_t mr;
        HMC5883L_STATUS ret;
        ret = hmc5883l_get_mr(hmc5883l, &mr);
        if (ret != HMC5883L_OK)
            return ret;
        *mode = (mr & 0b00000011);
    return HMC5883L_OK;
}


/*
 * Sets operating mode
 * 0U = Continuous measurement, 1U = Single measurement (default), 2U, 3U = Idle
 */
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


/*
 * Reads data from registers, places data in x, y, z fields
 */
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
    hmc5883l->x = ((buf[0] << 8) | buf[1]);
    hmc5883l->y = ((buf[2] << 8) | buf[3]);
    hmc5883l->z = ((buf[4] << 8) | buf[4]);

    return HMC5883L_OK;
}


/*
 * Checks if ready bit set in status register
 */
HMC5883L_STATUS hmc5883l_is_ready(HMC5883L *hmc5883l, uint8_t *is_ready) {
    uint8_t sr;
    HMC5883L_STATUS ret;
    ret = hmc5883l_get_sr(hmc5883l, &sr);
    if (ret != HMC5883L_OK)
        return ret;
    *is_ready = (sr & 0b00000001) != 0? 1: 0;
    return HMC5883L_OK;
}


/*
 * Checks if locked bit set in status register
 */
HMC5883L_STATUS hmc5883l_is_locked(HMC5883L *hmc5883l, uint8_t *is_locked) {
    uint8_t sr;
    HMC5883L_STATUS ret;
    ret = hmc5883l_get_sr(hmc5883l, &sr);
    if (ret != HMC5883L_OK)
        return ret;
    *is_locked = (sr & 0b00000010) != 0? 1: 0;
    return HMC5883L_OK;
}


/*
 * Waits for ready bit in status register
 */
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


/*
 * Performs single measurement
 */
HMC5883L_STATUS hmc5883l_single_measurement(HMC5883L *hmc5883l) {
    HMC5883L_STATUS ret;
    ret = hmc5883l_set_operating_mode(hmc5883l, 0U);
    if (ret != HMC5883L_OK)
        return ret;
    ret = hmc5883l_wait_ready(hmc5883l, 100);
    if (ret != HMC5883L_OK)
        return ret;
    ret = hmc5883l_read(hmc5883l);
    if (ret != HMC5883L_OK)
        return ret;
    return HMC5883L_OK;
}

/*
 * Performs continuous measurement
 * Make sure to set mode to continuous measurement
 */
HMC5883L_STATUS hmc5883l_continuous_measurement(HMC5883L *hmc5883l) {
    HMC5883L_STATUS ret;
    ret = hmc5883l_wait_ready(hmc5883l, 100);
    if (ret != HMC5883L_OK)
        return ret;
    ret = hmc5883l_read(hmc5883l);
    if (ret != HMC5883L_OK)
        return ret;
    return HMC5883L_OK;
}
