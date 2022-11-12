#include "stm32f4xx.h"

#include "picoRTOSConfig.h"
#include "picoRTOS.h"

#include "main.h"

#define BLINK_PERIOD PICORTOS_DELAY_SEC(1)
static void blinkMain(void *priv) {
    GPIOC->MODER |= (1 << (13*2));
    GPIOC->OTYPER |= ( 1 << 13 ); // PC13-LED    
    for(;;) {
        GPIOC->ODR ^= (1 << 13);
        picoRTOS_sleep(BLINK_PERIOD);
    }
}

#define TOGGLE_PERIOD PICORTOS_DELAY_MSEC(500)
static void pinToggleMain(void *priv) {
    GPIOC->MODER |= (1 << (14*2));
    GPIOC->OTYPER |= ( 1 << 14 ); // PC14
    for(;;) {
        GPIOC->ODR ^= (1 << 14);
        picoRTOS_sleep(TOGGLE_PERIOD);
    }
}

int main(void) {
    // init HW
    RCC->AHB1ENR |= (1 << 2); // GPIOC en
    
    picoRTOS_init();
    struct picoRTOS_task task;
    static picoRTOS_stack_t stack0[CONFIG_DEFAULT_STACK_COUNT];
    static picoRTOS_stack_t stack1[CONFIG_DEFAULT_STACK_COUNT];

    picoRTOS_task_init(&task, blinkMain, NULL, stack0, (size_t)CONFIG_DEFAULT_STACK_COUNT);
    picoRTOS_add_task(&task, (picoRTOS_priority_t)TASK_BLINK_PRIO);
    picoRTOS_task_init(&task, pinToggleMain, NULL, stack1, (size_t)CONFIG_DEFAULT_STACK_COUNT);
    picoRTOS_add_task(&task, (picoRTOS_priority_t)TASK_PIN_PRIO);

    picoRTOS_start();
    // should get here
    return 0;
}
