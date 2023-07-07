#include <Arduino.h>

#include <WebServer.h>
#include <WiFiClient.h>
#include <WiFiUtilities.h>

#include "OV2640.h"
//#include "OV2640Streamer.h"
//#include "CRtspSession.h"


#define VERBOSE         1
#define APP_NAME        "WiFiExample"
#define APP_VERSION     "1.0.0"

// N.B. Define at most one of these
#define ENABLE_WEB_SERVER  // Browser stream: "http://<ipa>", snapshot: "http://<ipa>/jpg"
//#define ENABLE_RTSP_SERVER // RTSP stream: rtsp://<ipa>:8554/mjpeg/1"
//// TODO add assert that both aren't defined


extern WiFiUtilities *wifi;
extern OV2640 cam;

void resetDevice(void);
String usage(String str);

void handleJpegStream(void);
void handleJpeg(void);
void handleNotFound();
void webTask(void *pvParameters);
void initWebServer(void);
void stopWebServer(void);

void initRTSP(void);
void stopRTSP(void);
