

# Trance

Live Lighting control via the network stack.

## Minmal Product

Here is the definition of the minimum viable product for this project in form of a list.

- E1.31
- Button WPS
- Status LED
- Rest endpoint
  - setting hostname
  - setting address
  - setting unverse

## LED States

- Red -> System
- Blue -> Wifi
  - Pulsing -> AP mode
  - Blinking -> Disconnected
  - Static -> Connected
- Green -> Communication
  - Blinking -> Stopped
  - Static -> Connected

## Product Variations

Same Housing, different components.

| Name     | Input | Network | Output     | Development    |
| -------- | ----- | ------- | ---------- | -------------- |
| Devboard | USB-C | IPEX    | PINS       | 1. Prototype   |
|          | USB-C | Wifi    | 1x SPI LED | -              |
|          | USB-C | Wifi    | 1x PWM LED | -              |
|          | USB-C | Wifi    | 1x DMX     | Schematic      |
|          | USB-C | ETH     | 1x SPI LED | -              |
|          | USB-C | ETH     | 1x PWM LED | -              |
|          | USB-C | ETH     | 1x DMX     | Parts Research |

Each variation uses the same "Devboard / Mainboard" by this production is simpler. Optimisation on the shield work with any board, and optimisations on the board work with any shield. This ensures ease of use ans modularity.

> I have to rethink the Mainboard design philiosophy, because it makes everything more expensive to create at home, and realistically the different hardwares will, one way ore the other, need to have different firmwares. It is important to have lots of oppertunities with the hardware, and then extend the functionalities with the software.

## Software Structure

The Software needs 3 Layers.

- Input
- Data
- Output

The Output is defined by the Hardware and can't be "updated" but the Input is based on the software and can be extended. The Data layer is the unified Language between these two. So the input can be extended with updates for example, adding mqtt support, extending rest interface. So the Input is the part of the software that is the same on all trance devices, and it can be updated witout having to reimplement Hardware drivers. The Data Layer is the interface inbetween. So the Input has put its Inputs into the the data format, wich is known, but the Input module does not Know what the Output is. Then the Output takes the data and writes it to the Output.

## Eurolite Quix DMX

Eurolite put USB-A Ports on some of their new devices. These ports aren't really USB but are a DMX interface utilizing the USB-A connector. Now the plan is to create a USB-A Stick, that can controll this.

But currently i am working on a internal wireless retrofit for the Eurolite LED 7C-7. But this is someting i dont think lots of people want to to do their devices, so i also want to someday create a USB-A Stick.

## Vision

Centralized Provision and setup of trance-family controllers. Inspired by Unifi or Node red.

Custom Protocol as artnet replacement, wich is livetime, but not just digitalize the dmx format.

Integration with popular open and propriatary smart home systems.