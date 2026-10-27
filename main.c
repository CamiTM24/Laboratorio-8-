/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * Universidad del Valle
  * Camila Tello 24404
  * Laboratorio 8
  * Electronica Digital 2
  *
  * @file
  * @brief
  *
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

/* Dibujo de cada letra: 5 columnas y 7 filas. */
typedef struct
{
  char letra;
  uint8_t columnas[5];
} TFT_Letra;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* Dimensiones de la pantalla vertical. */
#define TFT_ANCHO        240
#define TFT_ALTO         320

/* Colores RGB565. */
#define CELESTE          0x7EFC
#define NEGRO            0x0000
#define ROJO             0xF800

/* Girar 180 grados respecto a la primera version. */
#define TFT_ORIENTACION  0x88

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;

/* USER CODE BEGIN PV */

/* Caracteres necesarios para el titulo y los 72 BPM. */
static const TFT_Letra fuente[] = {
  {' ', {0x00, 0x00, 0x00, 0x00, 0x00}},
  {'2', {0x42, 0x61, 0x51, 0x49, 0x46}},
  {'7', {0x01, 0x71, 0x09, 0x05, 0x03}},
  {'A', {0x7E, 0x11, 0x11, 0x11, 0x7E}},
  {'B', {0x7F, 0x49, 0x49, 0x49, 0x36}},
  {'C', {0x3E, 0x41, 0x41, 0x41, 0x22}},
  {'D', {0x7F, 0x41, 0x41, 0x22, 0x1C}},
  {'I', {0x00, 0x41, 0x7F, 0x41, 0x00}},
  {'M', {0x7F, 0x02, 0x0C, 0x02, 0x7F}},
  {'N', {0x7F, 0x04, 0x08, 0x10, 0x7F}},
  {'O', {0x3E, 0x41, 0x41, 0x41, 0x3E}},
  {'P', {0x7F, 0x09, 0x09, 0x09, 0x06}},
  {'R', {0x7F, 0x09, 0x19, 0x29, 0x46}},
  {'T', {0x01, 0x01, 0x7F, 0x01, 0x01}}
};

/* Corazon 8x8
   El fondo celeste */
static const uint16_t corazon[8][8] = {
  {CELESTE, ROJO,    ROJO,    CELESTE, CELESTE, ROJO,    ROJO,    CELESTE},
  {ROJO,    ROJO,    ROJO,    ROJO,    ROJO,    ROJO,    ROJO,    ROJO},
  {ROJO,    ROJO,    ROJO,    ROJO,    ROJO,    ROJO,    ROJO,    ROJO},
  {ROJO,    ROJO,    ROJO,    ROJO,    ROJO,    ROJO,    ROJO,    ROJO},
  {CELESTE, ROJO,    ROJO,    ROJO,    ROJO,    ROJO,    ROJO,    CELESTE},
  {CELESTE, CELESTE, ROJO,    ROJO,    ROJO,    ROJO,    CELESTE, CELESTE},
  {CELESTE, CELESTE, CELESTE, ROJO,    ROJO,    CELESTE, CELESTE, CELESTE},
  {CELESTE, CELESTE, CELESTE, ROJO,    ROJO,    CELESTE, CELESTE, CELESTE}
};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
/* USER CODE BEGIN PFP */

static void TFT_Enviar(uint8_t *datos, uint16_t cantidad);
static void TFT_Comando(uint8_t comando);
static void TFT_Datos(const uint8_t *datos, uint16_t cantidad);
static void TFT_Registro(uint8_t comando,
                         const uint8_t *datos,
                         uint16_t cantidad);
static void TFT_Init(void);
static void TFT_Ventana(uint16_t x1, uint16_t y1,
                        uint16_t x2, uint16_t y2);
static void TFT_Relleno(uint16_t x, uint16_t y,
                        uint16_t ancho, uint16_t alto,
                        uint16_t color);
static void TFT_Texto(uint16_t x, uint16_t y,
                      const char *texto, uint8_t escala,
                      uint16_t color);
static void TFT_Corazon(uint16_t x, uint16_t y);
static void Mostrar_Monitor(void);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */

  /* Inicializar y dibujar la pantalla una sola vez. */
  TFT_Init();
  Mostrar_Monitor();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

    /* El diseño permanece fijo en pantalla. */
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

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
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(TFT_DC_GPIO_Port, TFT_DC_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(TFT_RST_GPIO_Port, TFT_RST_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(TF_SC_GPIO_Port, TF_SC_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : USART_TX_Pin USART_RX_Pin */
  GPIO_InitStruct.Pin = USART_TX_Pin|USART_RX_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : TFT_DC_Pin */
  GPIO_InitStruct.Pin = TFT_DC_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(TFT_DC_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : TFT_RST_Pin */
  GPIO_InitStruct.Pin = TFT_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(TFT_RST_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : TF_SC_Pin */
  GPIO_InitStruct.Pin = TF_SC_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(TF_SC_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* Enviar bytes a la pantalla utilizando SPI1. */
static void TFT_Enviar(uint8_t *datos, uint16_t cantidad)
{
  if (HAL_SPI_Transmit(&hspi1, datos, cantidad, 1000) != HAL_OK)
  {
    Error_Handler();
  }
}

/* DC bajo indica que enviamos una instruccion.
   CS bajo selecciona la pantalla. */
static void TFT_Comando(uint8_t comando)
{
  HAL_GPIO_WritePin(TF_SC_GPIO_Port, TF_SC_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(TFT_DC_GPIO_Port, TFT_DC_Pin, GPIO_PIN_RESET);

  TFT_Enviar(&comando, 1);

  HAL_GPIO_WritePin(TF_SC_GPIO_Port, TF_SC_Pin, GPIO_PIN_SET);
}

/* DC alto indica que enviamos parametros o colores. */
static void TFT_Datos(const uint8_t *datos, uint16_t cantidad)
{
  HAL_GPIO_WritePin(TF_SC_GPIO_Port, TF_SC_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(TFT_DC_GPIO_Port, TFT_DC_Pin, GPIO_PIN_SET);

  TFT_Enviar((uint8_t *)datos, cantidad);

  HAL_GPIO_WritePin(TF_SC_GPIO_Port, TF_SC_Pin, GPIO_PIN_SET);
}

/* Enviar una instruccion seguida de sus parametros. */
static void TFT_Registro(uint8_t comando,
                         const uint8_t *datos,
                         uint16_t cantidad)
{
  TFT_Comando(comando);

  if (cantidad > 0)
  {
    TFT_Datos(datos, cantidad);
  }
}

/* Preparar el controlador ILI9341. */
static void TFT_Init(void)
{
  HAL_GPIO_WritePin(TF_SC_GPIO_Port, TF_SC_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(TFT_DC_GPIO_Port, TFT_DC_Pin, GPIO_PIN_SET);

  /* Reiniciar la pantalla mediante su pin RESET. */
  HAL_GPIO_WritePin(TFT_RST_GPIO_Port, TFT_RST_Pin, GPIO_PIN_SET);
  HAL_Delay(10);
  HAL_GPIO_WritePin(TFT_RST_GPIO_Port, TFT_RST_Pin, GPIO_PIN_RESET);
  HAL_Delay(20);
  HAL_GPIO_WritePin(TFT_RST_GPIO_Port, TFT_RST_Pin, GPIO_PIN_SET);
  HAL_Delay(150);

  /* Reinicio por software. */
  TFT_Comando(0x01);
  HAL_Delay(150);

  /* Ajustes internos del controlador. */
  const uint8_t powerB[] = {0x00, 0xC1, 0x30};
  const uint8_t powerSeq[] = {0x64, 0x03, 0x12, 0x81};
  const uint8_t timingA[] = {0x85, 0x00, 0x78};
  const uint8_t powerA[] = {0x39, 0x2C, 0x00, 0x34, 0x02};
  const uint8_t pump[] = {0x20};
  const uint8_t timingB[] = {0x00, 0x00};

  const uint8_t power1[] = {0x23};
  const uint8_t power2[] = {0x10};
  const uint8_t vcom1[] = {0x3E, 0x28};
  const uint8_t vcom2[] = {0x86};

  /* Orientacion corregida y colores de 16 bits. */
  const uint8_t orientacion[] = {TFT_ORIENTACION};
  const uint8_t formato[] = {0x55};

  const uint8_t frame[] = {0x00, 0x18};
  const uint8_t display[] = {0x08, 0x82, 0x27};
  const uint8_t gammaOff[] = {0x00};
  const uint8_t gammaSel[] = {0x01};

  /* Ajustes de los tonos de color. */
  const uint8_t gammaP[] = {
    0x0F, 0x31, 0x2B, 0x0C, 0x0E,
    0x08, 0x4E, 0xF1, 0x37, 0x07,
    0x10, 0x03, 0x0E, 0x09, 0x00
  };

  const uint8_t gammaN[] = {
    0x00, 0x0E, 0x14, 0x03, 0x11,
    0x07, 0x31, 0xC1, 0x48, 0x08,
    0x0F, 0x0C, 0x31, 0x36, 0x0F
  };

  TFT_Registro(0xCF, powerB, sizeof(powerB));
  TFT_Registro(0xED, powerSeq, sizeof(powerSeq));
  TFT_Registro(0xE8, timingA, sizeof(timingA));
  TFT_Registro(0xCB, powerA, sizeof(powerA));
  TFT_Registro(0xF7, pump, sizeof(pump));
  TFT_Registro(0xEA, timingB, sizeof(timingB));

  TFT_Registro(0xC0, power1, sizeof(power1));
  TFT_Registro(0xC1, power2, sizeof(power2));
  TFT_Registro(0xC5, vcom1, sizeof(vcom1));
  TFT_Registro(0xC7, vcom2, sizeof(vcom2));

  TFT_Registro(0x36, orientacion, sizeof(orientacion));
  TFT_Registro(0x3A, formato, sizeof(formato));

  TFT_Registro(0xB1, frame, sizeof(frame));
  TFT_Registro(0xB6, display, sizeof(display));

  TFT_Registro(0xF2, gammaOff, sizeof(gammaOff));
  TFT_Registro(0x26, gammaSel, sizeof(gammaSel));
  TFT_Registro(0xE0, gammaP, sizeof(gammaP));
  TFT_Registro(0xE1, gammaN, sizeof(gammaN));

  /* Salir del reposo y activar la imagen. */
  TFT_Comando(0x11);
  HAL_Delay(150);
  TFT_Comando(0x29);
  HAL_Delay(20);
}

/* Seleccionar el area donde se pintaran los pixeles. */
static void TFT_Ventana(uint16_t x1, uint16_t y1,
                        uint16_t x2, uint16_t y2)
{
  uint8_t datos[4];

  /* Columnas inicial y final. */
  datos[0] = x1 >> 8;
  datos[1] = x1 & 0xFF;
  datos[2] = x2 >> 8;
  datos[3] = x2 & 0xFF;
  TFT_Registro(0x2A, datos, 4);

  /* Filas inicial y final. */
  datos[0] = y1 >> 8;
  datos[1] = y1 & 0xFF;
  datos[2] = y2 >> 8;
  datos[3] = y2 & 0xFF;
  TFT_Registro(0x2B, datos, 4);

  /* Iniciar escritura de pixeles. */
  TFT_Comando(0x2C);
}

/* Dibujar un rectangulo de un color.
   x, y: posicion.
   ancho, alto: tamaño en pixeles. */
static void TFT_Relleno(uint16_t x, uint16_t y,
                        uint16_t ancho, uint16_t alto,
                        uint16_t color)
{
  uint8_t buffer[128];
  uint32_t restantes;

  /* Evitar dibujar fuera de la pantalla. */
  if (x >= TFT_ANCHO || y >= TFT_ALTO ||
      ancho == 0 || alto == 0)
  {
    return;
  }

  if (ancho > TFT_ANCHO - x)
  {
    ancho = TFT_ANCHO - x;
  }

  if (alto > TFT_ALTO - y)
  {
    alto = TFT_ALTO - y;
  }

  TFT_Ventana(x, y, x + ancho - 1, y + alto - 1);

  /* Cada pixel ocupa dos bytes. */
  for (uint16_t i = 0; i < sizeof(buffer); i += 2)
  {
    buffer[i] = color >> 8;
    buffer[i + 1] = color & 0xFF;
  }

  restantes = (uint32_t)ancho * alto;

  HAL_GPIO_WritePin(TF_SC_GPIO_Port, TF_SC_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(TFT_DC_GPIO_Port, TFT_DC_Pin, GPIO_PIN_SET);

  /* Enviar bloques de hasta 64 pixeles. */
  while (restantes > 0)
  {
    uint16_t pixeles;

    if (restantes > 64)
    {
      pixeles = 64;
    }
    else
    {
      pixeles = (uint16_t)restantes;
    }

    TFT_Enviar(buffer, pixeles * 2);
    restantes -= pixeles;
  }

  HAL_GPIO_WritePin(TF_SC_GPIO_Port, TF_SC_Pin, GPIO_PIN_SET);
}

/* Dibujar texto con el tamaño y color seleccionados.
   escala amplifica cada punto de la letra. */
static void TFT_Texto(uint16_t x, uint16_t y,
                      const char *texto, uint8_t escala,
                      uint16_t color)
{
  while (*texto != '\0')
  {
    const uint8_t *columnas = fuente[0].columnas;

    /* Buscar el dibujo del caracter actual. */
    for (uint32_t i = 0;
         i < sizeof(fuente) / sizeof(fuente[0]); i++)
    {
      if (fuente[i].letra == *texto)
      {
        columnas = fuente[i].columnas;
        break;
      }
    }

    /* Pintar los puntos que tienen un bit en 1. */
    for (uint8_t col = 0; col < 5; col++)
    {
      for (uint8_t fila = 0; fila < 7; fila++)
      {
        if (columnas[col] & (1U << fila))
        {
          TFT_Relleno(x + col * escala,
                      y + fila * escala,
                      escala, escala, color);
        }
      }
    }

    /* Avanzar al siguiente caracter. */
    x += 6 * escala;
    texto++;
  }
}

/* Dibujar el corazon original.
   Cada punto del arreglo se amplia a 6x6 pixeles.
   El tamaño final es de 48x48 pixeles. */
static void TFT_Corazon(uint16_t x, uint16_t y)
{
  for (uint8_t fila = 0; fila < 8; fila++)
  {
    for (uint8_t col = 0; col < 8; col++)
    {
      TFT_Relleno(x + col * 6,
                  y + fila * 6,
                  6, 6,
                  corazon[fila][col]);
    }
  }
}

/* Organizar el diseño completo de la pantalla. */
static void Mostrar_Monitor(void)
{
	 /* Fondo celeste. */
	  TFT_Relleno(0, 0, TFT_ANCHO, TFT_ALTO, CELESTE);

	  /* Marco negro. */
	  TFT_Relleno(5, 5, 230, 2, NEGRO);
	  TFT_Relleno(5, 313, 230, 2, NEGRO);
	  TFT_Relleno(5, 5, 2, 310, NEGRO);
	  TFT_Relleno(233, 5, 2, 310, NEGRO);

	  /* Titulo en dos lineas. */
	  TFT_Texto(36, 25, "MONITOR", 4, NEGRO);
	  TFT_Texto(24, 65, "CARDIACO", 4, NEGRO);

	  /* Estrellita izquierda: brillo de cuatro puntas. */
	  TFT_Relleno(78, 101, 2, 12, NEGRO);
	  TFT_Relleno(73, 106, 12, 2, NEGRO);
	  TFT_Relleno(76, 104, 6, 6, NEGRO);

	  /* Estrellita central, un poco mas arriba. */
	  TFT_Relleno(119, 91, 2, 14, NEGRO);
	  TFT_Relleno(113, 97, 14, 2, NEGRO);
	  TFT_Relleno(117, 95, 6, 6, NEGRO);

	  /* Estrellita derecha. */
	  TFT_Relleno(160, 101, 2, 12, NEGRO);
	  TFT_Relleno(155, 106, 12, 2, NEGRO);
	  TFT_Relleno(158, 104, 6, 6, NEGRO);

	  /* Corazon rojo centrado. */
	  TFT_Corazon(96, 115);

	  /* Valor fijo y unidad. */
	  TFT_Texto(30, 180, "72", 9, NEGRO);
	  TFT_Texto(150, 209, "BPM", 3, NEGRO);
	}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */

  /* Detener el programa si ocurre un error. */
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
