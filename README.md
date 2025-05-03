# trance

I was not so happy with the state and variety of wireless dmx solutions. So i wanted to create my own. By creating my own adapter i relized that there also arent lots of options for direct wireless dmx led controllers. And so the Project trance is borne.

## MVP

Here is the definition of the minimum viable product for this project in form of a list.

- E1.31
- Button WPS
- Rest endpoint
  - setting hostname
  - setting address
  - setting unverse

## Product Palette

Same Housing, different components.

| Name | Input | Network | Output     |
|------|-------|---------|------------|
|      | USB-C | Wifi    | 1x SPI LED |
|      | USB-C | Wifi    | 1x PWM LED |
|      | USB-C | Wifi    | 1x DMX     |
|      | USB-C | ETH     | 1x SPI LED |
|      | USB-C | ETH     | 1x PWM LED |
|      | USB-C | ETH     | 1x DMX     |

## Vision

Centralized Provision and setup of trance-family controllers. Inspired by Unifi or Node red.

Custom Protocol as artnet replacement, wich is livetime, but not just digitalize the dmx format.

Integration with popular open and propriatary smart home systems.