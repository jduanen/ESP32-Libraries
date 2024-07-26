/********************************************************************************
 * 
 * Xiao Device On-Board LED Control Library
 *
 ********************************************************************************/

#include "OnBoardLED.h"


OnBoardLED::OnBoardLED(uint8_t ledPin) {
	_ledPin = ledPin;

	pinMode(_ledPin, OUTPUT);

	setColor(BLACK);

	_mode = MONO_LED_MODE;
};

OnBoardLED::OnBoardLED(uint8_t neoLedPwrPin, uint8_t neoLedCmdPin) {
	_pwrPin = neoLedPwrPin;
	_cmdPin = neoLedCmdPin;

	pinMode(_pwrPin, OUTPUT);
	pinMode(_cmdPin, OUTPUT);

////	#ifdef XIAO_RP2040  //// FIXME
	_neoPixel = new Adafruit_NeoPixel(1, _cmdPin, NEO_GBR + NEO_KHZ800);
	digitalWrite(_pwrPin, HIGH);
	_neoPixel->clear();

	_mode = NEO_PIXEL_MODE;
}

OnBoardLED::OnBoardLED(uint8_t redPin, uint8_t grnPin, uint8_t bluPin) {
	_redPin = redPin;
	_grnPin = grnPin;
	_bluPin = bluPin;

	pinMode(_redPin, OUTPUT);
	pinMode(_grnPin, OUTPUT);
	pinMode(_bluPin, OUTPUT);

	setColor(BLACK);

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
		_ledColor = (color == 0) ? BLACK : WHITE;
		break;
	case RGB_LED_MODE:
		digitalWrite(_redPin, ((~color >> 2) & 0x01));
		digitalWrite(_grnPin, ((~color >> 1) & 0x01));
		digitalWrite(_bluPin, (~color & 0x01));
		_ledColor = color;
		break;
	case NEO_PIXEL_MODE:
		//// TODO add brightness control
		uint32_t R = (color & 0x01) ? 0x00001F : 0x000000;
		uint32_t G = ((color >> 1) & 0x01) ? 0x001F00 : 0x000000;
		uint32_t B = ((color >> 2) & 0x01) ? 0x1F0000 : 0x000000;
		_neoPixel->setPixelColor(0, (R | G | B));
		_neoPixel->show();
		break;
	}
	_ledColor = color;
};

LEDColors OnBoardLED::currentColor() {
	return _ledColor;
}
