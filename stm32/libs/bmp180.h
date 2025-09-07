typedef enum {
	BMP180_OK				= 0x00U,
	BMP180_NORESPONSE		= 0x01U,
	BMP180_NOTFOUND			= 0x02U,
	BMP180_RESET_ERR 		= 0x03U,
	BMP180_WRITE_ERR		= 0x04U,
	BMP180_READ_ERR			= 0x05U,
	BMP180_VALUE_ERR		= 0x06U
} BMP180_STATUS;


typedef struct {
	I2C_HandleTypeDef	*i2c_bus;
	uint16_t			addr;
	int16_t				ac1;
	int16_t				ac2;
	int16_t				ac3;
	uint16_t			ac4;
	uint16_t			ac5;
	uint16_t			ac6;
	int16_t				b1;
	int16_t				b2;
	int16_t				mb;
	int16_t				mc;
	int16_t				md;
	uint8_t				oss;
	int32_t				ut;
	int32_t				up;
	double				temperature;
	double				pressure;
} BMP180;


HAL_StatusTypeDef bmp180_init(BMP180 *bmp180, I2C_HandleTypeDef *i2c_bus, uint16_t addr) {
	bmp180->i2c_bus = i2c_bus;
	bmp180->addr = addr;
	bmp180->oss = 0;
	HAL_StatusTypeDef ret;
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
	if (ret != HAL_OK) return BMP180_NORESPONSE;
	if (buf[0] != 0b01010101) return BMP180_NOTFOUND;

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
	if (ret != HAL_OK) return BMP180_RESET_ERR;
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
	if (ret != HAL_OK) return ret;
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
	if (ret != HAL_OK) return BMP180_READ_ERR;
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
	if (ret != HAL_OK) return BMP180_READ_ERR;
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
	if (ret != HAL_OK) return BMP180_READ_ERR;
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
	if (ret != HAL_OK) return BMP180_READ_ERR;
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
	if (ret != HAL_OK) return BMP180_READ_ERR;
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
	if (ret != HAL_OK) return BMP180_READ_ERR;
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
	if (ret != HAL_OK) return BMP180_READ_ERR;
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
	if (ret != HAL_OK) return BMP180_READ_ERR;
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
	if (ret != HAL_OK) return BMP180_READ_ERR;
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
	if (ret != HAL_OK) return BMP180_READ_ERR;
	bmp180->md = (buf[0] << 8) | buf[1];
	/* Calibration data end */

	return BMP180_OK;
}


HAL_StatusTypeDef bmp180_set_oversampling(BMP180 *bmp180, uint8_t oss) {
	if (oss < 0 || oss > 3) return BMP180_VALUE_ERR;
	bmp180->oss = oss;
	return HAL_OK;
}


HAL_StatusTypeDef bmp180_get_temperature(BMP180 *bmp180) {
	HAL_StatusTypeDef ret;
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
	if (ret != HAL_OK) return BMP180_WRITE_ERR;

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
		if (ret != HAL_OK) return BMP180_READ_ERR;
		HAL_Delay(5);
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
	if (ret != HAL_OK) return BMP180_READ_ERR;
	bmp180->ut = (buf[0] << 8) | buf[1];
	return BMP180_OK;
}


HAL_StatusTypeDef bmp180_get_pressure(BMP180 *bmp180) {
	HAL_StatusTypeDef ret;
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
	if (ret != HAL_OK) return BMP180_WRITE_ERR;

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
		if (ret != HAL_OK) return BMP180_READ_ERR;
		HAL_Delay(5);
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
	if (ret != HAL_OK) return BMP180_READ_ERR;
	bmp180->up = ((buf[0] << 16) | (buf[1] << 8) | buf[2]) >> (8 - bmp180->oss);
	return BMP180_OK;
}
