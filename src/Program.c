#include <msp430.h>
#include <stddef.h>
#include <stdint.h>
#include "Display.h"
#include "Render.h"
#include "Liquid.h"
#include <stdlib.h>
#include <stdbool.h>

const unsigned char InitializationData[] = {
 DISPLAY_OFF,
 DISPLAY_SET_CONTRAST, 0xFF,
 DISPLAY_SET_ADDRESS_MODE, DISPLAY_ADDRESS_MODE_VERTICAL,
 DISPLAY_SET_COLUMN_ADDRESS, 0x00, 0x7F,
 DISPLAY_SET_PAGE_ADDRESS, 0x00, 0x07,
 DISPLAY_SET_START_LINE,
 DISPLAY_SET_SEGMENT_REMAP_OFF,
 DISPLAY_SET_MULTIPLEX_RATIO, DISPLAY_MULTIPLEX_RESET,
 DISPLAY_SET_SCAN_DIRECTION_NORMAL,
 DISPLAY_SET_OFFSET, 0x00,
 DISPLAY_SET_CHARGE_PUMP, DISPLAY_CHARGE_PUMP_ON,
 DISPLAY_SET_COM_PINS, DISPLAY_COM_ALTERNATIVE,
 DISPLAY_SET_CLOCK_DIVIDE, 0xF0,
 DISPLAY_SET_PRECHARGE_PERIOD, 0x1,
 DISPLAY_SET_DESELECT_LEVEL, 0x20,
 DISPLAY_SET_NORMAL,
 DISPLAY_SHOW_RAM,
 DISPLAY_ON
};


#define LIQUIDS_LENGTH (DISPLAY_WIDTH / 2 + 1)
LiquidPoint Liquids[LIQUIDS_LENGTH];


void DisableWatchdog()
{
    WDTCTL = WDTPW | WDTHOLD;
}

void SetClockSpeed()
{
    // Because we're working with real-time rendering, we want the fastest possible clock speeds.
    DCOCTL = CALDCO_16MHZ;
    BCSCTL1 = CALBC1_16MHZ;
}

void EnableI2C()
{
    // Sets I2C to use Pin 1.6 and 1.7
    P1SEL |= BIT6 | BIT7;
    P1SEL2 |= BIT6 | BIT7;

    // Disables USCI so we can configure it
    UCB0CTL1 = UCSWRST;

    UCB0CTL0 = UCMST     // We're the "master"
            |  UCMODE_3  // UART mode with automatic baud rate detection
            |  UCSYNC;  // Synchronous mode. (Required for USCI)

    UCB0CTL1 = UCSSEL_2 | UCSWRST; // USCI clock source select. These bits select the BRCLK source clock -- SMCLK

    UCB0BR0 = 16;
    UCB0I2CSA = 0x3C; // Set slave address
    UCB0CTL1 &= ~UCSWRST; // Enable USCI
}

void EnableSwitch2Input()
{
    P1DIR &= ~BIT3;
    P1OUT |= BIT3;
    P1REN |= BIT3;
    P1IE |= BIT3;
    P1IES &= ~BIT3;
    P1IFG &= ~BIT3;
}

bool TriggerSplash = false;

void main(void)
{
    DisableWatchdog();

    SetClockSpeed();

    EnableI2C();

    EnableSwitch2Input();


    BeginWrite();
    WriteCommands(InitializationData, 31);
    EndWrite();

    ResetDisplayRAM_Vertical();

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
