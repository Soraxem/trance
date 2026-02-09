# TRANCE USB-DMX

[Quickstart Maunual](quickstart-manual.pdf)

## Overview

The USB DMX Model is a device that has an USB plug that delivers dmx data instead of usb data. This makes the Device compatible with all DMX-over-USB devices in lighting technology.

The Company Steinigke showtechnic has some fixtures with such a Port. These Ports are originally designed for their WirelessDMX and QuickDMX devices, wich essentially are CRMX adapters. This Project is in now way affiliated with XRMC or Steinigke showtechnic.

## Usage

In The Configurations page the Artnet DMX-Address has to be set to 1, for the device to work correctly.

## Assembly

There only are smd components for this device. Here is what you need to assemble this device.

- PCBS
- Stencil
- All components from the BOM
- Tweezers
- Solder Paste
- a Plastic card
- Tissues
- SMD reflow oven or Hot Plate
- Soldering Iron
- Solder
- Masking Tape
- a Steady hand

Study the schematic and the components closely, just to be sure where and how to place each component. Then apply the Solder Paste with the Stencil, place the components and reflow the device.

## Firmware

The firmware is dependant on multible Librarys, wich are mentioned at the beginning of the file. Make sure to install the correct versions of these librarys. It also depends on the Trance library, wich is not Published in the Library Manager. To install this library, download the right version from github and put it into your arduino library Folder.

For flashing press the button on the device and plug the USB-C into the computer while keeping the button pressed. By this the microcontroller connects to the computer and expexts a software to be flashed. In arduino select the correct port and configuration, then upload the firmware.