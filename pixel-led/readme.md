# Trance PIXEL-LED



## Overview

Pixel LED is simply a Trance model that can control a spi led strip. It is designed to be directly plugged into a wall charger. It features an infrared sensor so it could be integrated with infrared remote controls.

## Sponsorship

[PCBWay](pcbway.com) was so kind to sponsor pcb production of revision 1.0 in development of this device. I quickly wanted to review their services here.

All the pcbs i recieved were up to specification and of good quality. I was suprized an honored to recieve such a sponsoring for such a small project. I admire how PCBWay invests in OpenSource electronics, especially with their project sharing platform.

## Remote

Currently there is only one Remote implemented in the Code. This remote is shipped with these [WS8212 Globe stringlights](aliexpress.com/item/1005009844838768.html), and normally interfaces with that controller.

There are firmware versions with and without the infrared Remote.

## Firmware

The firmware is dependant on multible Librarys, wich are mentioned at the beginning of the firmware source file. Make sure to install the correct versions of these librarys. It also depends on the Trance library, wich is not Published in the Library Manager. To install this library, download the right version from github and put it into your arduino library Folder.

For flashing press the button on the device and plug the USB plug into the computer while keeping the button pressed. By this the microcontroller connects to the computer and expexts a software to be flashed. In arduino select the correct port and configuration, then upload the firmware.
