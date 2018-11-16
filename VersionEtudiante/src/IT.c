#include "stm32f10x.h"
#include "tp2lib.h"

void (* ptrFonction4)(void);
void (* ptrFonction2)(void);
void (* ptrFonction3)(void);

void TIM3_IRQHandler(void) {
TIM3 -> SR &=~(TIM_SR_UIF);
TempoCPU();
(*ptrFonction3)();
}

void TIM2_IRQHandler(void) {
TIM2 -> SR &=~(TIM_SR_UIF);

(*ptrFonction2)();

}

void TIM4_IRQHandler(void) {
TIM4 -> SR &=~(TIM_SR_UIF);

(*ptrFonction4)();

}

//COMMENT
int TimerInitIT2(TIM_TypeDef *timer, int priority, void (*ptr)(void)){
		
		int retour;
    //on autorise localement l'interruption à se lever
    //(uie = update interrupt enabled)
    timer->DIER|=TIM_DIER_UIE;  
            
    if (timer == TIM3) {

    ptrFonction3 = ptr;
    //on fait en sorte que l'interruption puisse être
    //acceptée au niveau du <3
    NVIC->ISER[0] |= NVIC_ISER_SETENA_29;  
                                                                
            
    //on place le niveau de priorité à priority
    NVIC->IP[29] = (priority << 4);  

		//PAS BESOIN
    //on place l'it en attente
    //NVIC->ISPR[0] |= NVIC_ISPR_SETPEND_29;

    retour = 0;

} else if (timer == TIM2) {
		ptrFonction2 = ptr;
		
	    //on fait en sorte que l'interruption puisse être
    //acceptée au niveau du <3
    NVIC->ISER[0] |= NVIC_ISER_SETENA_28;  
                                                                
            
    //on place le niveau de priorité à priority
    NVIC->IP[28] = (priority << 4);  

		//PAS BESOIN
    //on place l'it en attente
    //NVIC->ISPR[0] |= NVIC_ISPR_SETPEND_28;
	
	  retour = 0;
}

    //} else {
    
//on doit remplir ce else avec les autres possibilités de timer
//}

return retour;
}


            
    

