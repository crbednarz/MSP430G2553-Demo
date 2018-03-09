#include <msp430.h>
#include <stddef.h>
#include <stdint.h>
#include "Display.h"
#include "Render.h"
#include "Liquid.h"
#include <stdlib.h>
#include <stdbool.h>


#define LIQUIDS_LENGTH (DISPLAY_WIDTH / 2 + 1)
static LiquidPoint Liquids[LIQUIDS_LENGTH];


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

    InitializeLiquidLevel(Liquids, LIQUIDS_LENGTH, 30 * 4);

    int time = 0;
    while (1)
    {
        RenderLiquids(Liquids, LIQUIDS_LENGTH);

        StepLiquids(Liquids, LIQUIDS_LENGTH);
        time++;
        if (TriggerSplash)
        {
            TriggerSplash = false;
            srand(time);
            unsigned int x = (rand() % (LIQUIDS_LENGTH - 8)) + 4;
            Liquids[x].Y = 50 * 4;
            Liquids[x + 1].Y = 50 * 4;
            Liquids[x - 1].Y = 50 * 4;
            Liquids[x + 2].Y = 45 * 4;
            Liquids[x - 2].Y = 45 * 4;
            Liquids[x + 3].Y = 40 * 4;
            Liquids[x - 3].Y = 40 * 4;
        }
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
