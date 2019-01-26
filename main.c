
#define SYSCTL_RCGC2 							(*((volatile unsigned long *)0x400FE108))
#define GPIO_PORTF_AMSEL					(*((volatile unsigned long *)0x40025528))
#define GPIO_PORTF_PCTL						(*((volatile unsigned long *)0x4002552C))
#define GPIO_PORTF_DIR						(*((volatile unsigned long *)0x40025400))
#define GPIO_PORTF_AFESEL					(*((volatile unsigned long *)0x40025420))
#define GPIO_PORTF_DEN						(*((volatile unsigned long *)0x4002551C))
#define GPIO_PORTF_PUR						(*((volatile unsigned long *)0x40025510))
#define GPIO_PORTF_DATA						(*((volatile unsigned long *)0x400253FC))

#define PF1           (*((volatile unsigned long *)0x40025008)) //Red LED   --> VT
#define PF3           (*((volatile unsigned long *)0x40025020)) //Green LED --> Ready
#define PF4           (*((volatile unsigned long *)0x40025040)) //SW2    --> AS

void port_init(void)
{
	volatile unsigned long delay;
	SYSCTL_RCGC2 |= 0x00000020;
	delay = SYSCTL_RCGC2;
	GPIO_PORTF_AMSEL = 0x00;
	GPIO_PORTF_PCTL = 0x00000000;
	GPIO_PORTF_AFESEL = 0x00;
	GPIO_PORTF_DIR |= 0x0E;
	GPIO_PORTF_DIR &= (~0x10);
	GPIO_PORTF_DEN = 0x1E;
	GPIO_PORTF_PUR =0x10;
}

/*// #1 A subRoutine that waits for the switch to be pressed
void WaitForASLow(void){
	while(1){
		if(!(GPIO_PORTF_DATA & 0x10))break;
	}
	return;
}

// #2 A subRoutine that waits for the switch to be released 
void WaitForASHigh(void){
	while(1){
		if(GPIO_PORTF_DATA | 0xEF)break;
	}
	return;
}
*/
void WaitForASLow(void){
 while (PF4 != 0){}
 return;
}

// Subroutine reads AS input and waits for signal to be low
// If AS is already low, it returns right away
// If AS is currently high, it will wait until it to go low
// Inputs:  None
// Outputs: None
void WaitForASHigh(void){
 while (PF4 == 0){}
 return;
}
// #3 A subRoutine that sets VT high (PF1)
void SetVT(void){
	GPIO_PORTF_DATA |= 0x02;
}

// #4 A subRoutine that clears VT signal (PF1)
void ClearVT(void){
	GPIO_PORTF_DATA &= ~0x02;
}

// #5 A subRoutine that sets Ready signal high (PF3)
void SetReady(void){
	GPIO_PORTF_DATA |= 0x08;
}

// #6 A subRoutine that clears ready signal (PF3)
void ClearReady(void){
	GPIO_PORTF_DATA &= ~0x08;
}

// #7 A subRoutine for time delays
void Delay1ms(unsigned long msec){
	unsigned long i;
  while(msec > 0){
    i = 15333;  
    while(i > 0){
      i = i - 1;
    }
    msec = msec - 1; 
  }
}


int main(){
	port_init();
	while(1){
		SetReady();
		WaitForASLow();
		ClearReady();
		Delay1ms(10);
		WaitForASHigh();
		Delay1ms(250);
		SetVT();
		Delay1ms(250);
		ClearVT();
	}
}
