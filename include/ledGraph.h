#ifndef _LEDGRAPH_H_
#define _LEDGRAPH_H_

/* Conditional extern "C" so we're safe to call from C++ files */
#ifdef __cplusplus
extern "C" {
#endif

#include <libmaple/libmaple.h>
#include "initialize.h"
#include <libmaple/gpio.h>
#include <libmaple/exti.h>
#include <libmaple/timer.h>

#define MAX_RELOAD ((1 << 16) - 1)
#define BATTERY_BUTTON_PORT GPIOC
#define BATTERY_BUTTON_PIN 13
#define BATTERY_BUTTON_EXTI_LINE AFIO_EXTI_13
#define BATTERY_BUTTON_EXTI_PORT AFIO_EXTI_PC

#define BATTERY_LED1_PORT GPIOB
#define BATTERY_LED2_PORT GPIOB
#define BATTERY_LED3_PORT GPIOB
#define BATTERY_LED4_PORT GPIOA
#define BATTERY_LED5_PORT GPIOA
#define BATTERY_LED6_PORT GPIOA
#define BATTERY_LED7_PORT GPIOA
#define BATTERY_LED8_PORT GPIOC
#define BATTERY_LED9_PORT GPIOA
#define BATTERY_LED10_PORT GPIOB

#define BATTERY_LED1_PIN 11
#define BATTERY_LED2_PIN 10
#define BATTERY_LED3_PIN 2
#define BATTERY_LED4_PIN 7
#define BATTERY_LED5_PIN 6
#define BATTERY_LED6_PIN 5
#define BATTERY_LED7_PIN 4
#define BATTERY_LED8_PIN 15
#define BATTERY_LED9_PIN 14
#define BATTERY_LED10_PIN 3

#define BATTERY_TIMER TIMER1
#define BATTERY_TIMER_CH TIMER_CH1
#define BATTERY_PERIOD 2000000