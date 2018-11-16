#include "stm32f10x.h"


/*fonction executée quand l'exception est levée
 (handler) */
 
 void EXTI9_5_IRQHandler(void) 
 {
	//remettre le compteur à 0
	TIM3->CNT = 0;
}




void girouette() 
{
	//on active les signaux d'horloge pour le GPIO A
	RCC->APB1ENR|=RCC_APB1ENR_TIM3EN;
	

	
	
	//configuration broche 6 gpio A en input
	GPIOA->CRL &= ~( (GPIO_CRL_MODE6_1) | (GPIO_CRL_MODE6_0));
	//config de la broche 6 en floating input
	GPIOA->CRL|=GPIO_CRL_CNF6_0;
	GPIOA->CRL&=~(GPIO_CRL_CNF6_1);
	
	//configuration broche 7 gpio A en input
	GPIOA->CRL &= ~( (GPIO_CRL_MODE7_1) | (GPIO_CRL_MODE7_0));
	//config de la broche 7 en floating input
	GPIOA->CRL|=GPIO_CRL_CNF7_0;
	GPIOA->CRL&=~(GPIO_CRL_CNF7_1);
	
	//SMS= ‘011’ (TIMx_SMCR register, both inputs are active on both rising and falling edges)
	TIM3->SMCR |= (TIM_SMCR_SMS_0);
	TIM3->SMCR |= (TIM_SMCR_SMS_1);
	
	
	// activer le compteur 
	TIM3->CR1 |= TIM_CR1_CEN; 
	
	// lier l'entrée de TI1
	TIM3->CCMR1 |= TIM_CCMR1_CC1S_0;
	TIM3->CCMR1 |= TIM_CCMR1_CC1S_1;
	
	// lier l'entrée de TI2
	TIM3->CCMR2 |= TIM_CCMR1_CC2S_0;
	TIM3->CCMR2 |= TIM_CCMR1_CC2S_1;
	
	// polarité non inversé pour TI1
	TIM3->CCER &= ~TIM_CCER_CC1P;
	TIM3->CCER &= ~TIM_CCER_CC1NP;
	
	TIM3->CCMR1 &= ~TIM_CCMR1_IC1F_0;
	TIM3->CCMR1 &= ~TIM_CCMR1_IC1F_1;
	TIM3->CCMR1 &= ~TIM_CCMR1_IC1F_2;
	TIM3->CCMR1 &= ~TIM_CCMR1_IC1F_3;
	
	// polarité non inversé pour TI2
	TIM3->CCER &= ~TIM_CCER_CC2P;
	TIM3->CCER &= ~TIM_CCER_CC2NP;
	
	TIM3->CCMR2 &= ~TIM_CCMR1_IC2F_0;
	TIM3->CCMR2 &= ~TIM_CCMR1_IC2F_1;
	TIM3->CCMR2 &= ~TIM_CCMR1_IC2F_2;
	TIM3->CCMR2 &= ~TIM_CCMR1_IC2F_3;
	
	TIM3->ARR = 1440;
	
	
	//configuration index	
	// Alternate Function IO clock enabled
	RCC->APB2ENR |=  RCC_APB2ENR_AFIOEN ; 
	
	// choisir broche PA5
	AFIO->EXTICR[2] |= AFIO_EXTICR2_EXTI5_PA;
	
	// masquer la ligne PA5
	EXTI->IMR |= EXTI_IMR_MR5;
	
	// choisir le front montant
	EXTI->RTSR |= EXTI_RTSR_TR5;
	
	//configuration broche 5 gpio A en input
	GPIOA->CRL &= ~( (GPIO_CRL_MODE5_1) | (GPIO_CRL_MODE5_0));
	//config de la broche 5 en floating input
	GPIOA->CRL|=GPIO_CRL_CNF5_0;
	GPIOA->CRL&=~(GPIO_CRL_CNF5_1);
	
	
	//on autorise l'interruption au niveau du coeur
	NVIC->ISER[0]|=NVIC_ISER_SETENA_23;
}



int main (void)
{	
	girouette();
	
	
	while(1)
    {  
	
    }

}
