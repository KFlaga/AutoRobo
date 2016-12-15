
#ifndef PINS_H_
#define PINS_H_

#include "stm32f4xx_gpio.h"

#define SetPin(port, pin) (port)->BSRRL |= (pin)
#define ResetPin(port, pin) (port)->BSRRH |= (pin)

#define CheckPinHigh(port, pin) (port)->IDR & pin
#define CheckPinLow(port, pin) !((port)->IDR & pin)

// Output = MODER(01)
#define InitPinOutput(port, pinnum) (port)->MODER &= ~((uint32_t)(3<<((pinnum)*2))); \
	(port)->MODER |= ((uint32_t)(1<<((pinnum)*2)))
// Input = MODER(00)
#define InitPinInput(port, pinnum) (port)->MODER &= ~((uint32_t)(3<<((pinnum)*2)));
// Analog = MODER(11)
#define InitPinAnalog(port, pinnum) (port)->MODER |= ((uint32_t)(3<<((pinnum)*2)))
// AFIO = MODER(10)
#define InitPinAfio(port, pinnum)	(port)->MODER &= ~((uint32_t)(3<<((pinnum)*2))); \
	(port)->MODER |= ((uint32_t)(2<<((pinnum)*2)))
// LowSpeed = OSPEEDR(00)
#define InitPinLowSpeed(port, pinnum) (port)->OSPEEDR &= ~((uint32_t)(3<<((pinnum)*2)));
// MedSpeed = OSPEEDR(01)
#define InitPinMedSpeed(port, pinnum) (port)->OSPEEDR &= ~((uint32_t)(3<<((pinnum)*2))); \
	(port)->OSPEEDR |= ((uint32_t)(1<<((pinnum)*2)))
// HighSpeed = OSPEEDR(10)
#define InitPinHighSpeed(port, pinnum) (port)->OSPEEDR &= ~((uint32_t)(3<<((pinnum)*2))); \
	(port)->OSPEEDR |= ((uint32_t)(2<<((pinnum)*2)))
// Pullup-Pulldown enable = OTYPER(0)
#define InitPinPUPD(port, pinnum) (port)->OTYPER &= ~((uint16_t)(1<<(pinnum)));
// Open-drain = OTYPER(1)
#define InitPinOpenDrain(port, pinnum) (port)->OTYPER |= (uint16_t)(1<<(pinnum));
// No pu-pd = PUPDR(00)
#define InitPinNoPUPD(port, pinnum) (port)->PUPDR &= ~((uint32_t)(3<<((pinnum)*2)));
// Pullup = PUPDR(01)
#define InitPinPullUp(port, pinnum) (port)->PUPDR &= ~((uint32_t)(3<<((pinnum)*2))); \
	(port)->PUPDR |= ((uint32_t)(1<<((pinnum)*2)))
// Pulldown = PUPDR(10)
#define InitPinPullDown(port, pinnum) (port)->PUPDR &= ~((uint32_t)(3<<((pinnum)*2))); \
	(port)->PUPDR |= ((uint32_t)(2<<((pinnum)*2)))

void InitIO();
void ResetIO();

/*
 * Motor control pins/ports
 */
#define MOTO_Port_FL GPIOB
#define MOTO_Port_FR GPIOA
#define MOTO_Port_BL GPIOC
#define MOTO_Port_BR GPIOC
// PWM pins
#define MOTO_Pin_PWM_FL GPIO_Pin_12 // PB12
#define MOTO_PinNum_PWM_FL 12
#define MOTO_Pin_PWM_FR GPIO_Pin_5	// PA0 -> PA5
#define MOTO_PinNum_PWM_FR 5
#define MOTO_Pin_PWM_BL GPIO_Pin_8	// PC8
#define MOTO_PinNum_PWM_BL 8
#define MOTO_Pin_PWM_BR GPIO_Pin_10	// PC10
#define MOTO_PinNum_PWM_BR 10
// In1 pin -> forward on low = CCW
#define MOTO_Pin_IN1_FL GPIO_Pin_8	// PB8
#define MOTO_PinNum_IN1_FL 8
#define MOTO_Pin_IN1_FR GPIO_Pin_6	// PA1 -> PA6
#define MOTO_PinNum_IN1_FR 6
#define MOTO_Pin_IN1_BL GPIO_Pin_5	// PC5
#define MOTO_PinNum_IN1_BL 5
#define MOTO_Pin_IN1_BR GPIO_Pin_11	// PC11
#define MOTO_PinNum_IN1_BR 11
// In2 pin -> backward on low = CW
#define MOTO_Pin_IN2_FL GPIO_Pin_9	// PB9
#define MOTO_PinNum_IN2_FL 9
#define MOTO_Pin_IN2_FR GPIO_Pin_7	// PA4 -> PA7
#define MOTO_PinNum_IN2_FR 7
#define MOTO_Pin_IN2_BL GPIO_Pin_6	// PC6
#define MOTO_PinNum_IN2_BL 6
#define MOTO_Pin_IN2_BR GPIO_Pin_12	// PC12
#define MOTO_PinNum_IN2_BR 12
// Standby in common for left/right
#define MotoPort_Stby_L GPIOC	// PC9
#define MotoPin_Stby_L GPIO_Pin_9
#define MOTO_PinNum_Stby_L 9
#define MotoPort_Stby_R GPIOD	// PD2
#define MotoPin_Stby_R GPIO_Pin_2
#define MOTO_PinNum_Stby_R 2

/*
 * Encoders pins/ports
 */
#define ENC_Enable_Port	GPIOB // PB15
#define ENC_Enable_Pin GPIO_Pin_15
#define ENC_Enable_PinNum 15
#define ENC_Port_FL	GPIOB	// PB1
#define ENC_Pin_FL GPIO_Pin_1
#define ENC_PinNum_FL 1
#define ENC_Port_FR	GPIOA // PA4
#define ENC_Pin_FR GPIO_Pin_4
#define ENC_PinNum_FR 4
#define ENC_Port_BL	GPIOB // PB2
#define ENC_Pin_BL GPIO_Pin_2
#define ENC_PinNum_BL 2
#define ENC_Port_BR	GPIOA // PA1
#define ENC_Pin_BR GPIO_Pin_1
#define ENC_PinNum_BR 1

/*
 *  Button multiplexer :
 *  Address : A0 A1 EH
 *  EN enables inputs 0-3 on low, 4-7 on high
 */
#define BUT_MUX_A0_Port GPIOC // PC1
#define BUT_MUX_A0 GPIO_Pin_1
#define BUT_MUX_A0Num 1
#define BUT_MUX_A1_Port GPIOC // PC2
#define BUT_MUX_A1 GPIO_Pin_2
#define BUT_MUX_A1Num 2
#define BUT_MUX_EN_Port GPIOC // PC3
#define BUT_MUX_EN GPIO_Pin_3
#define BUT_MUX_ENNum 3
#define BUT_MUX_Read_Port GPIOC // PC0
#define BUT_MUX_Read GPIO_Pin_0
#define BUT_MUX_ReadNum 0

/*
 * LCD pins/ports
 */
#define LCD_E_Port GPIOC 	// PC4
#define LCD_E_Pin GPIO_Pin_4
#define LCD_E_PinNum 4
#define LCD_RS_Port GPIOB 	// PB10
#define LCD_RS_Pin GPIO_Pin_10
#define LCD_RS_PinNum 10
//#define LCD_RW_Port GPIOC 	// PC7
//#define LCD_RW_Pin GPIO_Pin_7
//#define LCD_D0_Port GPIOA 	// PA3
//#define LCD_D0_Pin GPIO_Pin_3
//#define LCD_D1_Port GPIOA 	// PA2
//#define LCD_D1_Pin GPIO_Pin_2
//#define LCD_D2_Port GPIOA 	// PA10
//#define LCD_D2_Pin GPIO_Pin_10
//#define LCD_D3_Port	GPIOB 	// PB3
//#define LCD_D3_Pin GPIO_Pin_3
#define LCD_D4_Port GPIOB 	// PB4
#define LCD_D4_Pin GPIO_Pin_4
#define LCD_D4_PinNum 4
#define LCD_D5_Port GPIOB 	// PB5
#define LCD_D5_Pin GPIO_Pin_5
#define LCD_D5_PinNum 5
#define LCD_D6_Port GPIOB 	// PB3
#define LCD_D6_Pin GPIO_Pin_3
#define LCD_D6_PinNum 3
#define LCD_D7_Port GPIOA 	// PA10
#define LCD_D7_Pin GPIO_Pin_10
#define LCD_D7_PinNum 10

/*
 * SPI pins
 */
#define SPI_MOSI_Port GPIOA 	// PA7
#define SPI_MOSI_Pin GPIO_Pin_7
#define SPI_MISO_Port GPIOA		// PA6
#define SPI_MISO_Pin GPIO_Pin_6
#define SPI_SCK_Port GPIOA		// PA5
#define SPI_SCK_Pin GPIO_Pin_5
#define SPI_CS_Port GPIOB		// PB6
#define SPI_CS_Pin GPIO_Pin_6

#endif
