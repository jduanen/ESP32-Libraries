/***************************************************************************
 *
 * WiFi Support Utilities Library
 * 
 ***************************************************************************/

//// TODO decide if I want to use char* instead of String
//// TODO add option to fallback on given (well-known?) AP mode if connect in STA mode times out
//// TODO make it possible to fail to connect (in any mode) and still go on
//// TODO allow config of network -- i.e., localIPAddr, GW, subnet, dnsAddr

#include "WiFiUtilities.h"

// Either try to connect in WIFI_STA mode, or offer connections in WIFI_AP mode.
// waitForConnect:
//  <0: wait indefinitely
//   0: return immediately (no wait)
//  >0: wait that many msec and then fail (reset the system)
//// FIXME figure out how to fail an constructor properly
//// FIXME use logging instead of prints here
WiFiUtilities::WiFiUtilities(wifi_mode_t mode, const String& ssid, const String& password, uint32_t waitForConnect) {
    uint32_t startTime = millis();
    _mode = mode;
    _ssid = ssid;
    _password = password;

    WiFi.macAddress(_mac);
    _macAddr = String(_mac[0], HEX) + ":" +
               String(_mac[1], HEX) + ":" +
               String(_mac[2], HEX) + ":" +
               String(_mac[3], HEX) + ":" +
               String(_mac[4], HEX) + ":" +
               String(_mac[5], HEX);

    WiFi.mode(_mode);
    switch (_mode) {
    case WIFI_STA:
        if (!WiFi.begin(_ssid, _password)) {
            Serial.println("ERROR: failed to start WiFi, rebooting...");
            _systemReset();
        }
        //// TODO WiFi.config(...)
        if (waitForConnect == 0) {
            Serial.print("Station mode WiFi connection in progress");
            Serial.println(", MAC address: " + _macAddr);
        } else {
            Serial.println("Connecting to WiFi in Station mode...");
            while (WiFi.status() != WL_CONNECTED) {
                Serial.println("    WiFi status: " + wiFiStatusToString(WiFi.status()));
                if (waitForConnect > 0) {
                    if ((millis() - startTime) > waitForConnect) {
                        //// FIXME do this better
                        Serial.println("rebooting...");
                        _systemReset();
                    }
                }
                delay(1000);
            }
            Serial.println("WiFi STA connection status: " + wiFiStatusToString(WiFi.status()));
            //assert((_ssid == WiFi.SSID()), "ERROR: not connected to correct SSID" + String(_ssid) + " != " + String(WiFi.SSID()));
            _ipAddr = WiFi.localIP();
            Serial.print("\nWiFi in Station Mode: ");
            Serial.print("IP address: " + _ipAddr.toString());
            Serial.print(", MAC address: " + _macAddr);
            Serial.print(", Connected to " + _ssid);
            Serial.println(", RSSI: " + String(WiFi.RSSI()));
        }
        break;
    case WIFI_AP:
        if (!WiFi.softAP(_ssid, _password)) {
            Serial.println("ERROR: failed to start WiFi, rebooting...");
            _systemReset();
        }
        //// TODO WiFi.softAP(...)
        _ipAddr = WiFi.softAPIP();
        if (waitForConnect == 0) {
            Serial.println("AP mode WiFi ready for connection");
        } else {
            Serial.println("Waiting for WiFi connection to the AP...");
            while (WiFi.status() != WL_CONNECTED) {
                Serial.println("    WiFi AP connection status: " + wiFiStatusToString(WiFi.status()));
                if (waitForConnect > 0) {
                    if ((millis() - startTime) > waitForConnect) {
                        // if connection wait times out, just return
                        break;
                    }
                }
                delay(50);
            }
        }
        Serial.println("\nWiFi Access Point Mode: ");
        Serial.print("AP SSID: " + _ssid);
        Serial.print(", AP IP Address: " + _ipAddr.toString());
        Serial.print(", AP MAC address: " + _macAddr);
        Serial.println(", RSSI: " + String(WiFi.RSSI()));
        break;
    default:
        Serial.println("ERROR: unhandled WiFi mode -- " + wiFiModeToString(_mode));
        break;
    }
};

String WiFiUtilities::rot47(String str) {
    String outStr = "";
    char oldChr, newChr;

    for (int i = 0; (i < str.length()); i++) {
        oldChr = str.charAt(i);
        if ((oldChr >= '!') && (oldChr <= 'O')) {
            newChr = ((oldChr + 47) % 127);
        } else {
            if ((oldChr >= 'P') && (oldChr <= '~')) {
                newChr = ((oldChr - 47) % 127);
            } else {
                newChr = oldChr;
            }
        }
        outStr.concat(newChr);
    }
    return(outStr);
};

String WiFiUtilities::wiFiStatusToString(wl_status_t status) {
    String s = "WIFI_UNKNOWN: " + String(status);
    switch (status) {
        case WL_NO_SHIELD: s = "WIFI_NO_SHIELD"; break;
        case WL_IDLE_STATUS: s = "WIFI_IDLE_STATUS"; break;
        case WL_NO_SSID_AVAIL: s = "WIFI_NO_SSID_AVAIL"; break;
        case WL_SCAN_COMPLETED: s = "WIFI_SCAN_COMPLETED"; break;
        case WL_CONNECTED: s = "WIFI_CONNECTED"; break;
        case WL_CONNECT_FAILED: s = "WIFI_CONNECT_FAILED"; break;
        case WL_CONNECTION_LOST: s = "WIFI_CONNECTION_LOST"; break;
        case WL_DISCONNECTED: s = "WIFI_DISCONNECTED"; break;
    }
    return(s);
};

String WiFiUtilities::wiFiModeToString(wifi_mode_t mode) {
    return String(_wifiModes[mode]);
};

wifi_mode_t WiFiUtilities::getWiFiMode() {
   return (_mode);
};

wl_status_t WiFiUtilities::getWiFiStatus() {
    return(WiFi.status());
};
