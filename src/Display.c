#include "Display.h"
#include "Render.h"

static const unsigned char InitializationData[] = {
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

static void SetClockSpeed()
{
    // Because we're working with real-time rendering, we want the fastest possible clock speeds.
    DCOCTL = CALDCO_16MHZ;
    BCSCTL1 = CALBC1_16MHZ;
}

static void EnableI2C()
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

void InitializeDisplay()
{
    SetClockSpeed();

    EnableI2C();
    BeginWrite();
    WriteCommands(InitializationData, 31);
    EndWrite();

    ResetDisplayRAM_Vertical();
}
