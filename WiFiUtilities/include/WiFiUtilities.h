/***************************************************************************
 *
 * WiFi Support Utilities Library
 * 
 ***************************************************************************/

#pragma once

#include <WiFi.h>
//// TODO make these conditional on ESP32
//#include <WiFiClient.h>
#include <WiFiAP.h>


#define DEF_WIFI_CONNECT_RETRIES    8


class WiFiUtilities {
public:
	WiFiUtilities(wifi_mode_t mode, const String& ssid, const String& password, uint32_t waitForConnect);

	String rot47(String str);
	String wiFiModeToString(wifi_mode_t mode);
	String wiFiStatusToString(wl_status_t status);
	wifi_mode_t getWiFiMode();
    wl_status_t getWiFiStatus();

protected:
	String _ssid;
	String _password;
	wifi_mode_t _mode;
    IPAddress _ipAddr;
    byte _mac[6];
    String _macAddr;
	const char _wifiModes[4][4] = {"OFF", "STA", "A_P", "APS"};

    void(*_systemReset)(void) = 0;
};
