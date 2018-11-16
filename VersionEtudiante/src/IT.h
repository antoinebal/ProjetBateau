#include "stm32f10x.h"
#include "tp2lib.h"


int TimerInitIT2(TIM_TypeDef *timer, int priority, void (*ptr)(void));
