/********************************************************************************
 * 
 * Xiao Device On-Board LED Control Library
 * 
 * Xiao RP2040 has a power LED (Red), a user-programmable RGB LED, and a NEOPixel.
 *   LED_BUILTIN = 17
 *   PIN_LED_R = 17
 *   PIN_LED_G = 16
 *   PIN_LED_B = 25
 *   NeoPixel:
 *     - PIN_NEOPIXEL   = 12  // command pin
 *     - NEOPIXEL_POWER = 11  // power pin
 *
 * These are defined in pins_arduino.h
 *
 ********************************************************************************/

#pragma once

#include <stdint.h>

#include <Adafruit_NeoPixel.h>


#define MONO_LED_MODE	0
#define RGB_LED_MODE	1
#define NEO_PIXEL_MODE	2


typedef enum LEDColors_e {
	BLACK   = 0x00,
	RED     = 0x01,
	GREEN   = 0x02,
	YELLOW  = 0x03,
	BLUE    = 0x04,
	MAGENTA = 0x05,
	CYAN    = 0x06,
	WHITE   = 0xFF
} LEDColors;


class OnBoardLED {
public:
	OnBoardLED(uint8_t ledPin);
	OnBoardLED(uint8_t neoLedPwrPin, uint8_t neoLedCmdPin);
	OnBoardLED(uint8_t redPin, uint8_t grnPin, uint8_t bluPin);

	void on();
	void off();
	void setColor(LEDColors color);

	LEDColors currentColor();
protected:
	uint8_t _mode;
	uint8_t _ledPin;
	uint8_t _redPin, _grnPin, _bluPin;
	uint8_t _pwrPin, _cmdPin;
	int8_t _brightness = 0x7F;
	LEDColors _ledColor = BLACK;
	Adafruit_NeoPixel *_neoPixel;
};
