[This on Youtube](https://www.youtube.com/playlist?list=PL4cNBOFjqRCpjGmj9xtUntssc8VHCmmZN) | [This on Makertube](https://makertube.net/w/p/su6X6o1RSBtH18CKgF7tRq) | [This on PCBWAY](https://www.pcbway.com/project/member/shareproject/?bmbno=57956445-373F-42)

# Trance

The Trance Device Family are open hardware devices that allow life wireless lighting control via ArtNet 4. These devices mainly have applications in portable, mobile, and flexible lighting solutions.

## Control

Some lighting controllers do not implement ArtNet 4 with udp unicasting. UDP unicasting is essential for wireless lighting, because most WiFi routers ratelimit and occasionally hold back multicasted UDP packets. Unicasted UDP packets do not get ratelimited and are therefore the way to go.

If your controller does not implement UDP multicasting as per ArtNet 4 Specification, you can use my translation layer called artman. Github Repo here.

## Software

To ensure a flexible and easy software, there is a Arduino library, that implements the communication side of the software. By updating this Library with more Protocols or more reliable networking, we can expand the functionallity of every trance controller, despite hardware differences. This Library also allows for easy hardware development for DIY solutions.

You can find the Library here on github.

## Hardware

This project also features custom hardware that is more compact than just using a ESP32 devboard.

### USB-DMX

This device interfaces with the Eurolite Wireless DMX port on some of the newer Eurolite devices.

### PIXEL-LED

This is a led controller that can interface with the common spi-leds like ws8212. It can directly be plugged in to the usb port of a wall adapter.

## License

This Project is licensed under CERN-OHL-P
