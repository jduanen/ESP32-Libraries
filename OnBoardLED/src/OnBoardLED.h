/********************************************************************************
 * 
 * Xiao Device On-Board LED Control Library
 *
 ********************************************************************************/

#pragma once

#include <stdint.h>


#define MONO_LED_MODE	0
#define RGB_LED_MODE	1


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
	OnBoardLED(uint8_t redPin, uint8_t grnPin, uint8_t bluPin);

	void on();
	void off();
	void setColor(LEDColors color);
protected:
	uint8_t _mode;
	uint8_t _ledPin;
	uint8_t _redPin, _grnPin, bluPin;
	LEDColors _ledState = BLACK;
};
