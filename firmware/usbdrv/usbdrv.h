/* ==========================================================================
 * usbdrv.h — V-USB driver header (minimal subset for ATtiny85)
 *
 * This is a STUB header providing declarations needed by the firmware.
 * For a real build, replace this directory with the full V-USB library from:
 *   https://github.com/obdev/v-usb
 *
 * License: GNU GPL v2 (or commercial license from obdev.at)
 * ========================================================================== */

#ifndef __usbdrv_h_included__
#define __usbdrv_h_included__

#include <avr/pgmspace.h>
#include <stdint.h>
#include "../usbconfig.h"

/* ---- Data types -------------------------------------------------------- */

typedef union {
    uint16_t word;
    uint8_t  bytes[2];
} usbWord_t;

typedef struct usbRequest {
    uint8_t     bmRequestType;
    uint8_t     bRequest;
    usbWord_t   wValue;
    usbWord_t   wIndex;
    usbWord_t   wLength;
} usbRequest_t;

/* ---- Return type for usbFunctionSetup ---------------------------------- */
typedef uint8_t usbMsgLen_t;

/* ---- Pointer to reply data --------------------------------------------- */
extern uint8_t *usbMsgPtr;

/* ---- Public API -------------------------------------------------------- */

extern void usbInit(void);
extern void usbPoll(void);

/* These macros manipulate the D- pull-up (1.5 kΩ via R4 on this board).
 * On ATtiny85 the pull-up is hardwired, so connect/disconnect are no-ops
 * in hardware, but V-USB uses them for re-enumeration timing.             */
#define usbDeviceConnect()      ((void)0)
#define usbDeviceDisconnect()   ((void)0)

/* ---- Callback prototypes (implemented in main.c) ----------------------- */

extern usbMsgLen_t usbFunctionSetup(uint8_t data[8]);

#if USB_CFG_IMPLEMENT_FN_WRITE
extern uint8_t usbFunctionWrite(uint8_t *data, uint8_t len);
#endif

#if USB_CFG_IMPLEMENT_FN_READ
extern uint8_t usbFunctionRead(uint8_t *data, uint8_t len);
#endif

/* ---- HID report descriptor (defined in main.c) ------------------------- */
extern PROGMEM const char usbHidReportDescriptor[];

#endif /* __usbdrv_h_included__ */
