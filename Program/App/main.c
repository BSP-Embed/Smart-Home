#include "main.h"

extern int8u lcdptr;

int16u mov_dura= 0;

int main(void)
{
	float temp;
	
	init();
	
	while (TRUE) {
		
		 set_sleep_mode(SLEEP_MODE_IDLE);
		 sleep_mode();
			
		if (Flags.Meas == TRUE) {
			
			temp = meastemp(1);
			
			if (temp <= LOW_THRESHOLD)
				fanoff();
				
			if (PIR_PPIN & _BV(PIR_SENS_PIN)) {
				lighton();
				lcdclrr(0);
				lcdws(" Human Detected");
				mov_dura = 0;
				Flags.PIR = 1;
			}
			
			if (Flags.PIR && temp >= HIGH_THRESHOLD)
				fanon();
			
			Flags.Meas = FALSE;
		}
			
		if (Flags.loadoff) {
			lcdclrr(0);
			lcdws("Energy Conserv'n");
			lightoff();
			fanoff();
			Flags.PIR = 0;
			Flags.loadoff = FALSE;
		}
	}
	return 0;
}
static void init(void)
{
	Flags.PIR = FALSE;
	Flags.Meas = FALSE;
	Flags.loadoff = FALSE;
	
	gpioinit();
	buzinit();
	ledinit();
	beep(2,100);
	lcdinit();
	adcinit();
	tmr1init();
	disptitl();
	beep(1,100);
	sei();
	StartTmr();

}
static void disptitl(void)
{
	lcdclr();
	lcdws("Energy Conserv'n");
	lcdr2();
	lcdws("  Temp:     'C");
}
static void tmr1init(void)
{
	TCNT1H   = 0xD3;
	TCNT1L   = 0x00;
	TIMSK   |= _BV(TOIE1);			//ENABLE OVERFLOW INTERRUPT
	TCCR1A   = 0x00;					
	TCCR1B  |= _BV(CS10) | _BV(CS11); /* PRESCALAR BY 16 */
}
/* overflows at every 100msec */
ISR(TIMER1_OVF_vect) 
{ 
	static int8u i, j;

	TCNT1H = 0xD3;
	TCNT1L = 0x00;
	
	if (Flags.Meas == FALSE && ++j >= 10) {
		Flags.Meas = TRUE;
		j = 0;
	}
	
	if (Flags.PIR && mov_dura++ >= 150)
		Flags.loadoff = 1;
		
	if (++i >= 50) 
		 i = 0;
			
	switch(i) {
		case 0: case 2: ledon(); break;
		case 1: case 3: ledoff(); break;
	} 
	/* put in sleep */
	
}
static float meastemp(int8u disp)
{
	float temp;
	char s[10];

	temp = adcget(0);

	temp *=  STEP_SIZE;
	temp /= LM35_MUL_FACTOR;
	
	if (disp == 1) {
		lcdptr = 0xc7;
		lcdws("     ");
		ftoa(temp,s);
		lcdptr = 0xc7;
		lcdws(s);
	}
	return temp;
}

static void gpioinit(void)
{
	RELAY_DDR |= _BV(LIGHT_PIN) | _BV(FAN_PIN);
	RELAY_PORT &= ~(_BV(LIGHT_PIN) | _BV(FAN_PIN));
	
	PIR_DDR &= ~_BV(PIR_SENS_PIN);
	PIR_PORT |= _BV(PIR_SENS_PIN);
	
}