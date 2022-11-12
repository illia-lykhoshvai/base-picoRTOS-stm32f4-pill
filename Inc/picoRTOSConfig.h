#ifndef PICORTOSCONFIG_H
#define PICORTOSCONFIG_H

#include "stm32f4xx.h"

/* CLOCKS */
#define CONFIG_SYSCLK_HZ        SystemCoreClock
#define CONFIG_TICK_HZ          1000

/* TASKS */
#define CONFIG_TASK_COUNT       2
#define TASK_BLINK_PRIO         0
#define TASK_PIN_PRIO           1

/* STACK */
#define CONFIG_DEFAULT_STACK_COUNT 128

/* VTABLE */
#define CONFIG_ARCH_ARM_MOVE_VTABLE_TO_RAM

/* IPCs */
#define CONFIG_DEADLOCK_COUNT 1000

#endif
