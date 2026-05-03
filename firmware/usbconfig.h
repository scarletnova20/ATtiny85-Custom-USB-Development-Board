/* ==========================================================================
 * usbconfig.h — V-USB configuration for ATtiny85 Custom USB Dev Board
 *
 * Pin mapping (derived from the KiCad schematic):
 *   PB3  (pin 2)  → USB D-  (68 Ω series R2, 3.6 V Zener D1, 1.5 kΩ pull-up R4)
 *   PB4  (pin 5)  → USB D+  (68 Ω series R1, 3.6 V Zener D2)
 *   PB1  (pin 12) → LED     (330 Ω series R3)
 *   PB0  (pin 11) → GPIO    (directly on J1)
 *   PB2  (pin 14) → GPIO    (directly on J1)
 *   PB5  (pin 1)  → RESET
 *
 * Clock: 16.5 MHz PLL (internal oscillator, no external crystal)
 * ========================================================================== */

#ifndef __usbconfig_h_included__
#define __usbconfig_h_included__

/* ---------------------------- Hardware Config ---------------------------- */

/* USB D- is on Port B, bit 3 (directly accessible for INT0 on ATtiny85) */
#define USB_CFG_IOPORTNAME      B
#define USB_CFG_DMINUS_BIT      3

/* USB D+ is on Port B, bit 4 — must support interrupt (or use pin-change) */
#define USB_CFG_DPLUS_BIT       4

/* Clock speed in Hz — ATtiny85 PLL at 16.5 MHz */
#define USB_CFG_CLOCK_KHZ       16500

/* Use USB_COUNT_SOF to track bus activity (optional, saves power) */
#define USB_CFG_CHECK_CRC       0

/* ----------------------- Interrupt Configuration ------------------------ */

/*
 * ATtiny85 uses PCINT for USB — we use pin-change interrupt on D+.
 * V-USB needs these macros to route the interrupt correctly.
 */
#define USB_INTR_CFG            PCMSK
#define USB_INTR_CFG_SET        (1 << USB_CFG_DPLUS_BIT)
#define USB_INTR_CFG_CLR        0
#define USB_INTR_ENABLE         GIMSK
#define USB_INTR_ENABLE_BIT     PCIE
#define USB_INTR_PENDING        GIFR
#define USB_INTR_PENDING_BIT    PCIF
#define USB_INTR_VECTOR         PCINT0_vect

/* --------------------- Functional Configuration ------------------------- */

#define USB_CFG_HAVE_INTRIN_ENDPOINT    1   /* EP1 IN for interrupt data   */
#define USB_CFG_HAVE_INTRIN_ENDPOINT3   0
#define USB_CFG_EP3_NUMBER              3
#define USB_CFG_IMPLEMENT_HALT          0
#define USB_CFG_SUPPRESS_INTR_CODE      0
#define USB_CFG_INTR_POLL_INTERVAL      100 /* ms between interrupt polls  */
#define USB_CFG_IS_SELF_POWERED         0   /* bus-powered from USB VBUS   */
#define USB_CFG_MAX_BUS_POWER           50  /* max 50 mA                   */
#define USB_CFG_IMPLEMENT_FN_WRITE      1
#define USB_CFG_IMPLEMENT_FN_READ       0
#define USB_CFG_IMPLEMENT_FN_WRITEOUT   0
#define USB_CFG_HAVE_FLOWCONTROL        0
#define USB_CFG_LONG_TRANSFERS          0
#define USB_CFG_DRIVER_FLASH_PAGE       0

#define USB_COUNT_SOF                   0
#define USB_CFG_CHECK_DATA_TOGGLING     0

/* ---------------------- Device Description (IDs) ------------------------ */

/* Use the shared V-USB VID/PID for HID devices (obdev.at free-use IDs) */
#define USB_CFG_VENDOR_ID       0xc0, 0x16  /* 0x16c0 — Van Ooijen Technische Informatica */
#define USB_CFG_DEVICE_ID       0xdc, 0x05  /* 0x05dc — obdev shared HID            */
#define USB_CFG_DEVICE_VERSION  0x00, 0x01  /* version 1.0                           */
#define USB_CFG_VENDOR_NAME     'A','T','t','i','n','y','8','5','U','S','B'
#define USB_CFG_VENDOR_NAME_LEN 11
#define USB_CFG_DEVICE_NAME     'L','E','D',' ','C','o','n','t','r','o','l'
#define USB_CFG_DEVICE_NAME_LEN 11
#define USB_CFG_SERIAL_NUMBER_LEN 0

/* ---------------------- Device Class ------------------------------------ */

#define USB_CFG_DEVICE_CLASS        0       /* class is specified at interface level */
#define USB_CFG_DEVICE_SUBCLASS     0
#define USB_CFG_INTERFACE_CLASS     3       /* HID                                  */
#define USB_CFG_INTERFACE_SUBCLASS  0       /* no boot interface                    */
#define USB_CFG_INTERFACE_PROTOCOL  0       /* no protocol                          */
#define USB_CFG_HID_REPORT_DESCRIPTOR_LENGTH 22

/* ---------------------- Optional Features ------------------------------- */

#define USB_CFG_DESCR_PROPS_DEVICE                  0
#define USB_CFG_DESCR_PROPS_CONFIGURATION           0
#define USB_CFG_DESCR_PROPS_STRINGS                 0
#define USB_CFG_DESCR_PROPS_STRING_0                0
#define USB_CFG_DESCR_PROPS_STRING_VENDOR           0
#define USB_CFG_DESCR_PROPS_STRING_PRODUCT          0
#define USB_CFG_DESCR_PROPS_STRING_SERIAL_NUMBER    0
#define USB_CFG_DESCR_PROPS_HID                     0
#define USB_CFG_DESCR_PROPS_HID_REPORT              0
#define USB_CFG_DESCR_PROPS_UNKNOWN                 0

#endif /* __usbconfig_h_included__ */
