#include "stm32f4xx.h"

int main(void) {
    // init HW
    RCC->AHB1ENR |= (1 << 2); // GPIOC en
    GPIOC->MODER |= (1 << (13*2));
    GPIOC->OTYPER |= ( 1 << 13 ); // PC13-LED


    // should get here if no enough memory to create idle task

    for(;;) {
        GPIOC->ODR ^= (1 << 13);
        for(uint32_t i = 0; i < 500000; i++);
    }

    return 0;
}
