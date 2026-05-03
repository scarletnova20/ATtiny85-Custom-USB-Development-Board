/* ==========================================================================
 * main.c — ATtiny85 USB LED Controller (V-USB)
 *
 * A minimal USB HID device that lets a host application toggle the onboard
 * LED connected to PB1 via a 330 Ω resistor (R3).
 *
 * Custom HID vendor requests:
 *   bRequest = 1  →  SET LED state  (wValue.low: 0 = OFF, 1 = ON, 2 = TOGGLE)
 *   bRequest = 2  →  GET LED state  (returns 1 byte: 0 or 1)
 *
 * Build:
 *   make            — compile
 *   make flash      — compile & upload via USBasp
 *   make fuse       — set fuses for 16.5 MHz PLL
 *
 * ========================================================================== */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <util/delay.h>

#include "usbdrv/usbdrv.h"

/* ----------------------------- Pin Mapping ------------------------------ */

#define LED_PORT    PORTB
#define LED_DDR     DDRB
#define LED_PIN     PB1          /* schematic: PB1 → R3(330Ω) → LED → GND */

/* ----------------------------- USB Requests ----------------------------- */

#define USB_REQ_SET_LED   1
#define USB_REQ_GET_LED   2

/* ------------------------------- Globals -------------------------------- */

static uint8_t ledState = 0;     /* current LED state: 0 = off, 1 = on     */
static uint8_t replyBuf[1];      /* buffer for GET_LED reply               */

/* -------------------- HID Report Descriptor (22 bytes) ------------------- */

PROGMEM const char usbHidReportDescriptor[22] = {
    0x06, 0x00, 0xff,            /* USAGE_PAGE (Vendor Defined)            */
    0x09, 0x01,                  /* USAGE (Vendor Usage 1)                 */
    0xa1, 0x01,                  /* COLLECTION (Application)               */
    0x15, 0x00,                  /*   LOGICAL_MINIMUM (0)                  */
    0x26, 0xff, 0x00,            /*   LOGICAL_MAXIMUM (255)                */
    0x75, 0x08,                  /*   REPORT_SIZE (8)                      */
    0x95, 0x01,                  /*   REPORT_COUNT (1)                     */
    0x09, 0x00,                  /*   USAGE (Undefined)                    */
    0xb2, 0x02, 0x01,            /*   FEATURE (Data,Var,Abs,Buf)           */
    0xc0                         /* END_COLLECTION                         */
};

/* -------------------- V-USB Setup Request Handler ----------------------- */

usbMsgLen_t usbFunctionSetup(uint8_t data[8])
{
    usbRequest_t *rq = (void *)data;

    switch (rq->bRequest) {

    case USB_REQ_SET_LED:
        /* wValue.low: 0 = OFF, 1 = ON, 2 = TOGGLE */
        if (rq->wValue.bytes[0] == 2) {
            ledState ^= 1;
        } else {
            ledState = rq->wValue.bytes[0] & 1;
        }
        if (ledState) {
            LED_PORT |= (1 << LED_PIN);
        } else {
            LED_PORT &= ~(1 << LED_PIN);
        }
        return 0;                /* no data phase */

    case USB_REQ_GET_LED:
        replyBuf[0] = ledState;
        usbMsgPtr = replyBuf;
        return 1;                /* send 1 byte */

    default:
        return 0;
    }
}

/* -------------------- Calibrate Internal Oscillator --------------------- */

/* V-USB can auto-calibrate the RC oscillator from USB SOF timing.
 * This helper nudges OSCCAL to keep the clock within USB tolerance. */
static void calibrateOscillator(void)
{
    /* The V-USB library provides usbMeasureFrameLength() when
     * USB_CFG_HAVE_MEASURE_FRAME_LENGTH is set.  For simplicity we
     * rely on the factory-calibrated 16.5 MHz PLL here.              */
}

/* ============================= Main Loop ================================ */

int main(void)
{
    uint8_t i;

    /* --- watchdog: 1 s timeout, used as periodic wakeup --- */
    wdt_enable(WDTO_1S);

    /* --- LED pin as output, start OFF --- */
    LED_DDR  |= (1 << LED_PIN);
    LED_PORT &= ~(1 << LED_PIN);

    /* --- V-USB init --- */
    usbInit();
    usbDeviceDisconnect();       /* force re-enumeration */
    i = 0;
    while (--i) {
        wdt_reset();
        _delay_ms(1);            /* ~255 ms disconnect pulse */
    }
    usbDeviceConnect();

    sei();                       /* enable global interrupts */

    /* --- Startup blink: 3 quick flashes to confirm the MCU is alive --- */
    for (i = 0; i < 6; i++) {
        LED_PORT ^= (1 << LED_PIN);
        _delay_ms(80);
    }
    LED_PORT &= ~(1 << LED_PIN);
    ledState = 0;

    /* --- Main loop ---- */
    for (;;) {
        wdt_reset();
        usbPoll();               /* V-USB housekeeping */
    }

    return 0;
}
