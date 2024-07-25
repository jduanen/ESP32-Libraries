/********************************************************************************
 * 
 * Xiao Device On-Board LED Control Library
 *
 ********************************************************************************/

#include "OnBoardLED.h"


OnBoardLED::OnBoardLED(uint8_t ledPin) {
	_ledPin = ledPin;
	pinMode(_ledPin, OUTPUT);
	_mode = MONO_LED_MODE;
};

OnBoardLED::OnBoardLED(uint8_t redPin, uint8_t grnPin, uint8_t bluPin) {
	_redPin = redPin;
	_grnPin = grnPin;
	_bluPin = bluPin;
	pinMode(_redPin, OUTPUT);
	pinMode(_grnPin, OUTPUT);
	pinMode(_bluPin, OUTPUT);
	_mode = RGB_LED_MODE;
};

void OnBoardLED::on() {
	setColor(WHITE);
};

void OnBoardLED::off() {
	setColor(BLACK);
};

void OnBoardLED::setColor(LEDColors color) {
	switch (_mode) {
	case MONO_LED_MODE:
		digitalWrite(_ledPin, color);
		_ledState = (color != 0);
		break;
	case RGB_LED_MODE:
		digitalWrite(_redPin, (color & 0x01));
		digitalWrite(_grnPin, ((color >> 1) & 0x01));
		digitalWrite(_bluPin, ((color >> 2) & 0x01));
		_ledState = color;
		break;
	}
};
