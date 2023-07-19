/*
 * GPIO.h
 */

#ifndef GPIO_GPIO_H_
#define GPIO_GPIO_H_

#define SW_1        GPIO_PIN_0          //ON
#define SW_2        GPIO_PIN_1          //OFF
#define SW1_Int     GPIO_INT_PIN_0
#define SW2_Int     GPIO_INT_PIN_1

/*
 * Port L:  Pin - batt
 *
 * Port H:  Pin0 - pv
 *          Pin1 - boost
 *          Pin2 - load
 *          Pin3 - biDc
 */

/* GPIO Function */
extern void Config_LED(void);
extern void Config_SW(void);
extern void Config_GPIO(void);

extern void LED_control(int, bool);
//extern void ssr_all(bool);
extern void ssr_control(uint8_t, bool);

#endif /* GPIO_GPIO_H_ */
