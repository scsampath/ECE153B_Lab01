/*
 * ECE 153B - Winter 2022
 *
 * Name(s): Sachen Sampath, Ryan Chau
 * Section:	Thursday
 * Lab: 1A
 */
#include "stm32l476xx.h"
#include "stdbool.h"

void Init(){
    // Enable HSI
    RCC->CR |= ((uint32_t)RCC_CR_HSION);
    while ( (RCC->CR & (uint32_t) RCC_CR_HSIRDY) == 0 );

    // Select HSI as system clock source
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
    RCC->CFGR |= (uint32_t)RCC_CFGR_SW_HSI;
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) == 0 );

    // Enable GPIO Clock
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
		RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;

    // Initialize Green LED
	
		// Set the mode of PA5 to Output (01)
    GPIOA->MODER &= ~(3UL<<10); //clear mode bits
		GPIOA->MODER |= 1UL<<10; // Set mode to output
	
		// Set the output type of PA 5 as Push-Pull (0,reset)
		GPIOA->OTYPER &= ~(1UL<<5);
	
		// Set PA5 to No Pull-Up, No Pull-Down (00,reset)
		GPIOA->PUPDR &= ~(3UL<<10);
	
		// Initialize User Button
		
		// Set mode of pin PC13 to Input (00)
    GPIOC->MODER &= ~(3UL<<26); //clear mode bits
		
		// Set PC13 to no pull-up and no pull-down (00,reset)
		GPIOC->PUPDR &= ~(3UL<<26);
}

int main(void){
    // Initialization
    Init();

    // Polling to Check for User Button Presses
		
		// Default, 0 is an even number so led should be set
		GPIOA->ODR |= 1UL<<5;
	
		bool previousButtonIsPressed = false;
		bool isBlinking = false;
	  int counter = 0;
		
		//polling loop
		while(1){
			bool buttonIsPressed = !(GPIOC->IDR>>13)&1;
			if(buttonIsPressed && !previousButtonIsPressed){
				isBlinking = !isBlinking;
				if(!isBlinking){
					GPIOA->ODR |= 1UL<<5;
				}
			}
			previousButtonIsPressed = buttonIsPressed;
			if(isBlinking){
				if(counter >= 100000){
					counter = 0;
					GPIOA->ODR ^= 1UL<<5;
				}
				counter += 1;
			}
		}
    return 0;
}
