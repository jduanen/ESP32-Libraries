#include "main.h"


WebServer webServer(80);

TaskHandle_t webTaskHandler;

boolean stopWeb = false;


#ifdef ENABLE_WEB_SERVER
/*
 * Handle image stream requests
 * Returns a response and runs in a loop to update web content
 *  each time a new image is available.
 */
void handleJpegStream(void) {
    WiFiClient client = webServer.client();
    String response = "HTTP/1.1 200 OK\r\n";
    response += "Content-Type: multipart/x-mixed-replace; boundary=frame\r\n\r\n";
    webServer.sendContent(response);

    while (true) {
        cam.run();
        if (!client.connected()) {
            break;
        }
        response = "--frame\r\n";
        response += "Content-Type: image/jpeg\r\n\r\n";
        webServer.sendContent(response);

        client.write((char *)cam.getfb(), cam.getSize());
        webServer.sendContent("\r\n");
        if (!client.connected()) {
            break;
        }
    }
}

/*
 * Handle single image requests
 * Gets image from camera and send it to caller
 */
void handleJpeg(void) {
    WiFiClient client = webServer.client();

    cam.run();
    if (!client.connected()) {
        return;
    }
    String response = "HTTP/1.1 200 OK\r\n";
    response += "Content-disposition: inline; filename=capture.jpg\r\n";
    response += "Content-type: image/jpeg\r\n\r\n";
    webServer.sendContent(response);
    client.write((char *)cam.getfb(), cam.getSize());
}

void handleNotFound() {
    String message = "";
    message += "WebServer is running!\n";
#ifdef ENABLE_RTSP_SERVER
    message += "RTSP Server is running!\n";
#endif
    message += "\n";

    message += "Unhandled Request:\n";
    message += "    URI:       ";
    message += webServer.uri() + "\n";
    message += "    Method:    ";
    message += (webServer.method() == HTTP_GET) ? "GET\n" : "POST\n";
    message += "    Arguments: ";
    message += webServer.args();
    message += "\n\n";

    message = usage(message);
    message += "\n";

    webServer.send(200, "text/plain", message);
}
#endif

void webTask(void *pvParameters) {
    webServer.on("/", HTTP_GET, handleJpegStream);
    webServer.on("/jpg", HTTP_GET, handleJpeg);
    webServer.onNotFound(handleNotFound);
    webServer.begin();
    while (true) {
#ifdef ENABLE_WEB_SERVER
	    webServer.handleClient();
#endif
	    if (stopWeb) {
	    	webServer.close();
	    	vTaskDelete(NULL);
	    }
	    delay(100);
    }
}

void initWebServer() {
#ifdef ENABLE_WEB_SERVER
	xTaskCreate(webTask, "WEB", 4096, NULL, 1, &webTaskHandler);
	if (webTaskHandler == NULL) {
		Serial.println("ERROR: failed to create WebServer task");
	} else {
		Serial.println("WebServer task running");
	}
#endif
}

void stopWebServer() {
    stopWeb = true;
}
