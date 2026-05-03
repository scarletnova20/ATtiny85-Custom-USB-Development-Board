#!/usr/bin/env python3
"""
host_led_control.py — Host-side USB control for the ATtiny85 LED board.

Sends vendor-specific USB control requests to set / get / toggle the
onboard LED connected to PB1.

Requirements:
    pip install pyusb

Usage:
    python host_led_control.py on
    python host_led_control.py off
    python host_led_control.py toggle
    python host_led_control.py status
"""

import sys
import usb.core
import usb.util

# Shared V-USB IDs (obdev.at free-use VID/PID for HID devices)
VID = 0x16C0
PID = 0x05DC

# Custom vendor request codes (must match firmware main.c)
USB_REQ_SET_LED = 1
USB_REQ_GET_LED = 2

# USB control request type: vendor, device-to-host / host-to-device
CTRL_OUT = usb.util.CTRL_TYPE_VENDOR | usb.util.CTRL_RECIPIENT_DEVICE | usb.util.CTRL_OUT
CTRL_IN  = usb.util.CTRL_TYPE_VENDOR | usb.util.CTRL_RECIPIENT_DEVICE | usb.util.CTRL_IN


def find_device():
    """Locate the ATtiny85 USB LED controller on the bus."""
    dev = usb.core.find(idVendor=VID, idProduct=PID)
    if dev is None:
        print("ERROR: ATtiny85 USB device not found.")
        print("       • Is the board plugged in?")
        print("       • Were fuses set correctly (make fuse)?")
        sys.exit(1)
    return dev


def set_led(dev, value):
    """Send SET_LED request. value: 0=OFF, 1=ON, 2=TOGGLE."""
    dev.ctrl_transfer(CTRL_OUT, USB_REQ_SET_LED, value, 0, [])
    states = {0: "OFF", 1: "ON", 2: "TOGGLED"}
    print(f"LED → {states.get(value, '???')}")


def get_led(dev):
    """Read current LED state."""
    result = dev.ctrl_transfer(CTRL_IN, USB_REQ_GET_LED, 0, 0, 1)
    state = result[0] if len(result) else 0
    print(f"LED state: {'ON' if state else 'OFF'}")
    return state


def main():
    if len(sys.argv) < 2:
        print("Usage: python host_led_control.py [on|off|toggle|status]")
        sys.exit(1)

    cmd = sys.argv[1].lower()
    dev = find_device()

    if cmd == "on":
        set_led(dev, 1)
    elif cmd == "off":
        set_led(dev, 0)
    elif cmd == "toggle":
        set_led(dev, 2)
    elif cmd == "status":
        get_led(dev)
    else:
        print(f"Unknown command: {cmd}")
        print("Valid commands: on, off, toggle, status")
        sys.exit(1)


if __name__ == "__main__":
    main()
