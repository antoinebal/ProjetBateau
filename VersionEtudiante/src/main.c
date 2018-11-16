#include "stm32f10x.h"
#include "math.h"
int x,y,tangue;
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
/* pour améliorer la précision, on peut faire une fenêtre glissante
gardant en mémoire les n dernières mesures. En effet, un acclééromètre peut renvoyer
une grosse mesure pour une petite pente s'il y a eu un gros accoût .
Lancer le calcul toutes les x ms : pour cela régler une int sur deb d'un timer 
avec un petit arr */
void  accelerometre () {
//on active les signaux d'horloge pour le GPIO A
	RCC->APB1ENR|=RCC_APB1ENR_TIM3EN;
	//configuration broche 0 gpio C en input
	GPIOC->CRL &= ~( (GPIO_CRL_MODE0_1) | (GPIO_CRL_MODE0_0));
	//config de la broche 0 en analog input
	GPIOC->CRL&=~(GPIO_CRL_CNF0_0 | GPIO_CRL_CNF0_1);
	
	//configuration broche 1 gpio C en input
	GPIOC->CRL &= ~( (GPIO_CRL_MODE1_1) | (GPIO_CRL_MODE1_0));
	//config de la broche 1 en analog input
	GPIOC->CRL &=~(GPIO_CRL_CNF1_0 | GPIO_CRL_CNF1_1);
	
	}
void configure_adc(){

    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN; // validation horloge ADC1
    RCC->CFGR |= RCC_CFGR_ADCPRE_DIV6; // passage de l'horloge ADC1 à 12MHz
    ADC1->CR2|= ADC_CR2_ADON; // démarrage ADC1
		ADC1->CR2|=0x07<<17;
	  ADC1->CR2|=ADC_CR2_EXTTRIG;
    ADC1->SQR1&= ADC_SQR1_L; // fixe le nombre de conversion à 1
    ADC1->CR2 |= ADC_CR2_CAL; // dÈbut de la calibration
    while ((ADC1->CR2 & ADC_CR2_CAL)); // attente de la fin de la calibration


	
	

}
int convert_single(char channel){
	
		ADC1->SQR3= channel; // indique la voie à convertir
    ADC1->CR2 |= ADC_CR2_SWSTART; // lancement de la conversion
    while(!(ADC1->SR & ADC_SR_EOC) ) {} // attente de la fin de conversion
    ADC1->SR &= ~ADC_SR_EOC; // validation de la conversion
    return ADC1->DR & ~((0x0F) << 12); // retour de la conversion
}

int main (void)
{	

	girouette();
	accelerometre();
	configure_adc();
	
	 while(1)
	{	
		x = convert_single(10); // conversion
    y = convert_single(11); // conversion   
		tangue = sin(x)/cos(y);
	}
}
