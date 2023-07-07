#include "main.h"

#include "wifi.h"


const char *ssid = WLAN_SSID;
const char *password = WLAN_PASS;

WiFiUtilities *wifi;
OV2640 cam;


void resetDevice(void) {
    delay(100);
    WiFi.disconnect();
    esp_restart();
}

String usage(String str) {
    IPAddress ip = WiFi.localIP();
    str += "Browser Usage:\n";
    str += "    Image Stream: http://";
    str += ip.toString() + "\n";
    str += "    Single Image: http://";
    str += ip.toString() + "/jpg\n";
#ifdef ENABLE_RTSP_SERVER
    str += "RTSP Usage:\n";
    str += "    Image Stream: rtsp://";
    str += ip.toString();
    str += ":8554/mjpeg/1\n";
#endif
    return(str);
}

void setup() {
    Serial.begin(115200);
    while (!Serial) {
        delay(10);
    }
    delay(1000);
    Serial.println(String(APP_NAME) + ": BEGIN");

    Serial.println("\n\n##################################");
    Serial.printf("Internal Total heap %d, internal Free Heap %d\n", ESP.getHeapSize(), ESP.getFreeHeap());
    Serial.printf("SPIRam Total heap %d, SPIRam Free Heap %d\n", ESP.getPsramSize(), ESP.getFreePsram());
    Serial.printf("ChipRevision %d, Cpu Freq %d, SDK Version %s\n", ESP.getChipRevision(), ESP.getCpuFreqMHz(), ESP.getSdkVersion());
    Serial.printf("Flash Size %d, Flash Speed %d\n", ESP.getFlashChipSize(), ESP.getFlashChipSpeed());
    Serial.println("##################################\n\n");

    wifi = new WiFiUtilities(WIFI_STA, ssid, password, -1);

    //// TODO fix the library properly
    //// directly edited these files:
    //// .pio/libdeps/seeed_xiao_esp32s3/Micro-RTSP/src/OV2640.h
    //// .pio/libdeps/seeed_xiao_esp32s3/Micro-RTSP/src/OV2640.cpp
    cam.init(xiaoCam_config);

#ifdef ENABLE_WEB_SERVER
    initWebServer();
#endif

#ifdef ENABLE_RTSP_SERVER
    initRTSPServer();
#endif

    Serial.println(usage("\n"));

    Serial.println(String(APP_NAME) + ": READY");
}

void loop() {
    delay(100);
}
