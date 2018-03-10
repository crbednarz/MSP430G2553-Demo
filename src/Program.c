#include <msp430.h>
#include "Display.h"
#include "World.h"
#include "Render.h"
#include <stdbool.h>
#include <stdlib.h>


#define LIQUIDS_LENGTH (DISPLAY_WIDTH / 2 + 1)
static World ActiveWorld;


static void DisableWatchdog()
{
	WDTCTL = WDTPW | WDTHOLD;
}


static void EnableSwitch2Input()
{
	P1DIR &= ~BIT3;
	P1OUT |= BIT3;
	P1REN |= BIT3;
	P1IE |= BIT3;
	P1IES &= ~BIT3;
	P1IFG &= ~BIT3;
}

static bool TriggerSplash = false;


void main(void)
{
	DisableWatchdog();

	EnableSwitch2Input();

	InitializeDisplay();

	InitializeWorld(&ActiveWorld);

	int time = 0;

	while (true)
	{
		time++;

		StepWorld(&ActiveWorld);

		RenderWorld(&ActiveWorld);

		if (TriggerSplash)
		{
			TriggerSplash = false;
			srand(time);
			unsigned int x = (rand() % (LIQUIDS_LENGTH - 8)) + 4;
			ActiveWorld.Liquids[x].Y = 50 * 4;
			ActiveWorld.Liquids[x + 1].Y = 50 * 4;
			ActiveWorld.Liquids[x - 1].Y = 50 * 4;
			ActiveWorld.Liquids[x + 2].Y = 45 * 4;
			ActiveWorld.Liquids[x - 2].Y = 45 * 4;
			ActiveWorld.Liquids[x + 3].Y = 40 * 4;
			ActiveWorld.Liquids[x - 3].Y = 40 * 4;
		}

		__delay_cycles(20000);
	}
}


// USCI_B0 Data ISR
#pragma vector = USCIAB0TX_VECTOR
__interrupt void USCIAB0TX_ISR(void){
	IFG2 &= ~UCB0TXIFG;
	__bic_SR_register_on_exit(LPM3_bits);
}

#pragma vector = PORT1_VECTOR
__interrupt void PORT1_ISR(void)
{
	TriggerSplash = true;
	P1IFG &= ~BIT3;
}
