#ifndef MAIN_H
#define MAIN_H

/* comment the below line for release */
//#define TESTING

#include "includes.h"

#ifdef TESTING
	#define PhNum	 "9980237552"
#else
	#define PhNum	 "9008590320"
#endif

#define RELAY_DDR				DDRC
#define RELAY_PORT				PORTC
#define LIGHT_PIN				PC2
#define FAN_PIN					PC3

#define PIR_DDR					DDRD
#define PIR_PORT				PORTD
#define PIR_PPIN				PIND
#define PIR_SENS_PIN			PD2


/* DEFINE CONSTANT */
#define LM35_MUL_FACTOR		0.01			/* 10mv/degree */
#define STEP_SIZE			0.00488			/* 5v / 1024 */

#define HIGH_THRESHOLD		35				/* in degrees */
#define LOW_THRESHOLD		30


struct  {
	volatile int8u PIR:1;
	volatile int8u Meas:1;
	volatile int8u loadoff:1;
	
}Flags;


#define lighton()			RELAY_PORT |= _BV(LIGHT_PIN)
#define lightoff()			RELAY_PORT	&= ~_BV(LIGHT_PIN)

#define fanon()				RELAY_PORT |= _BV(FAN_PIN)
#define fanoff()			RELAY_PORT	&= ~_BV(FAN_PIN)


//DEFINE MACROS
#define StartTmr()			TCCR0  	|= _BV(CS01)
#define StopTmr()			TCCR0  	&= ~_BV(CS01)


//FUNCTION PROTOTYPES
static void		 init		(void);
static void 	 disptitl 	(void);
static void 	tmr1init	(void);
static float	meastemp	(int8u disp);
static void		gpioinit	(void);
#endif
