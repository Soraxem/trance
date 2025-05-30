

# Trance

Live Lighting control via the network stack.

## Minmal Product

Here is the definition of the minimum viable product for this project in form of a list.

- E1.31
- Button WPS
- Rest endpoint
  - setting hostname
  - setting address
  - setting unverse

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

## Vision

Centralized Provision and setup of trance-family controllers. Inspired by Unifi or Node red.

Custom Protocol as artnet replacement, wich is livetime, but not just digitalize the dmx format.

Integration with popular open and propriatary smart home systems.