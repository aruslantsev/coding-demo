/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

// Button 1. Bits:  78 | HEX: 0x00053A9B9F40729E5370
// Button 2. Bits:  78 | HEX: 0x00042F539F40729E53E0
// Button 3. Bits:  78 | HEX: 0x0005020D9F40729E53B0
// Button 4. Bits:  78 | HEX: 0x000FDBF59F40729E53D0

const uint8_t REMOTE_BUTTON_1[10] = {0x00, 0x05, 0x3A, 0x9B, 0x9F, 0x40, 0x72, 0x9E, 0x53, 0x70};
const uint8_t REMOTE_BUTTON_2[10] = {0x00, 0x04, 0x2F, 0x53, 0x9F, 0x40, 0x72, 0x9E, 0x53, 0xE0};
const uint8_t REMOTE_BUTTON_3[10] = {0x00, 0x05, 0x02, 0x0D, 0x9F, 0x40, 0x72, 0x9E, 0x53, 0xB0};
const uint8_t REMOTE_BUTTON_4[10] = {0x00, 0x0F, 0xDB, 0xF5, 0x9F, 0x40, 0x72, 0x9E, 0x53, 0xD0};

#define TX_PORT       GPIOB
#define TX_PIN        GPIO_PIN_11

// Длительность элементарного импульса в микросекундах (300-400 мкс)
#define TE_US         350

#define PACKET_REPEATS 5

static inline void delay_us(uint32_t us) {
    uint32_t count = us * (SystemCoreClock / 1000000U) / 6U;
    while (count--) {
        __NOP();
    }
}

static void send_bit_1(void) {
    HAL_GPIO_WritePin(TX_PORT, TX_PIN, GPIO_PIN_SET);
    delay_us(TE_US * 2);
    HAL_GPIO_WritePin(TX_PORT, TX_PIN, GPIO_PIN_RESET);
    delay_us(TE_US);
}

static void send_bit_0(void) {
    HAL_GPIO_WritePin(TX_PORT, TX_PIN, GPIO_PIN_SET);
    delay_us(TE_US);
    HAL_GPIO_WritePin(TX_PORT, TX_PIN, GPIO_PIN_RESET);
    delay_us(TE_US * 2);
}

static void send_preamble(void) {
    HAL_GPIO_WritePin(TX_PORT, TX_PIN, GPIO_PIN_SET);
    delay_us(TE_US);
    HAL_GPIO_WritePin(TX_PORT, TX_PIN, GPIO_PIN_RESET);
    delay_us(TE_US * 30); // Межкадровый интервал
}

static void send_packet_78bit(const uint8_t *data) {
    send_preamble();

    for (int i = 0; i < 78; i++) {
        uint8_t byte_idx = i / 8;
        uint8_t bit_idx  = 7 - (i % 8); // Начиная со старшего бита (MSB first)

        if ((data[byte_idx] >> bit_idx) & 0x01) {
            send_bit_1();
        } else {
            send_bit_0();
        }
    }

    // Принудительно сбрасываем пин в 0 по окончании пакета
    HAL_GPIO_WritePin(TX_PORT, TX_PIN, GPIO_PIN_RESET);
}

void Transmit_Remote_Signal(void) {
    for (int repeat = 0; repeat < PACKET_REPEATS; repeat++) {
        send_packet_78bit(REMOTE_BUTTON_1);

        // Межпакетная пауза ~12 мс (аналогично оригинальному пульту)
        HAL_Delay(12);
    }
}

void Enter_Standby_Mode(void) {
    // 1. Включаем тактирование блока PWR
    __HAL_RCC_PWR_CLK_ENABLE();

    // 2. Очищаем ВСЕ флаги питания (Wakeup flag & Standby flag)
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU | PWR_FLAG_SB);

    // 3. Активируем пин пробуждения PA0 (WKUP1)
    HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1);

    // 4. Сбрасываем пин передатчика перед сном
    HAL_GPIO_WritePin(TX_PORT, TX_PIN, GPIO_PIN_RESET);

    // 5. Ждем, пока пользователь отпустит кнопку на PA0!
    // (Если кнопка нажата, PA0 = 1. Ждем, пока станет 0, чтобы не "зациклить" просыпание)
    while (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_SET) {
        HAL_Delay(10);
    }

    // 6. Повторно очищаем флаги после отпускания кнопки
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);

    // 7. Входим в Standby
    HAL_PWR_EnterSTANDBYMode();
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */

  Transmit_Remote_Signal(); // 4. СРАЗУ отправляем пачку из 5 пакетов

  HAL_Delay(10);         // 5. Небольшая задержка
  Enter_Standby_Mode();  // 6. Снова "выключаемся" в Standby (2–3 мкА)

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    // Transmit_Remote_Signal();
    //
    // // Пауза 1 секунда между пачками
    // HAL_Delay(100);

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_RESET);

  /*Configure GPIO pin : PA0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PB11 */
  GPIO_InitStruct.Pin = GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
