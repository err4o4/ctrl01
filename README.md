# CTRL 01

![hero](https://github.com/err4o4/ctrl01/blob/master/STL/hero.png?raw=true)

ESP32-based [LilyGO T-Display](https://www.lilygo.cc/products/lilygo%C2%AE-ttgo-t-display-1-14-inch-lcd-esp32-control-board) media controller for Apple products. Supports any number of buttons (lib: OneButton), pixel LEDs (lib: FastLed), and a display(lib: TFT_eSPI). The code can be adapted to utilize one or all of these features.

## Apple Media Service

This device displays current playback information from an iPhone and supports playback controls such as next/previous track, play/pause, and volume adjustment. It also includes deep sleep mode to conserve battery power.

[Apple Documentation.](https://developer.apple.com/library/archive/documentation/CoreBluetooth/Reference/AppleMediaService_Reference/Specification/Specification.html#//apple_ref/doc/uid/TP40014716-CH1-SW7)

Special thanks to the following examples. As far as I can tell, this hasn't been implemented using the [Arduino ESP32 board library](https://github.com/espressif/arduino-esp32) before.

- [James Hudson's Apple Notification Center Service](https://github.com/Smartphone-Companions/ESP32-ANCS-Notifications/blob/master/src/ancs_ble_client.cpp) This example finally showed how to solicit services using the Arduino ESP32 library, which is not built-in.
- [John Park's Adafruit Guide to the Apple Media Service Display](https://learn.adafruit.com/now-playing-bluetooth-apple-media-service-display/apple-music-service) This was the first working example I could find. Written in CircutPython.
- [Moddable's Apple Media Service example](https://github.com/Moddable-OpenSource/moddable/blob/6d7f33f8f318663bd5ba8d6ca6536443d42e68ea/modules/network/ble/ams-client/amsclient.js#L129) This was another good example, written in javascript.
- [Del Sol Clock](https://github.com/Marcus10110/DelSolClock) Part of ESP32 code for AMS taken from this guy.