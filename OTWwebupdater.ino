#include <ETH.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Update.h>

const char* host = "esp32";

IPAddress local_IP(192, 168, 0, 50);  // Set your desired unique IP here
IPAddress gateway(192, 168, 0, 1);    // Your network gateway
IPAddress subnet(255, 255, 255, 0);   // Subnet mask
IPAddress primaryDNS(8, 8, 8, 8);     // Primary DNS (Google DNS)
IPAddress secondaryDNS(8, 8, 4, 4);   // Secondary DNS (Google DNS)

WebServer server(80);

/*
 * Login page
 */
const char* loginIndex = "<form name='loginForm'> ... (same as before) </form>";

/*
 * Server Index Page
 */
const char* serverIndex = "<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script> ... (same as before)";

/*
 * Setup function
 */
void setup(void) {
  Serial.begin(115200);

  // Disable WiFi
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  btStop();

  // Initialize Ethernet with static IP
  if (!ETH.begin(1, 16, 23, 18, ETH_PHY_LAN8720, ETH_CLOCK_GPIO0_IN)) {  // phy_addr=1, power=Pin 16, mdc=Pin 23, mdio=Pin 18
    Serial.println("Failed to initialize Ethernet");
    return;
  }

  // Configure static IP address
  if (!ETH.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("Static IP configuration failed");
  }

  Serial.println("Starting Ethernet...");

  // Wait for Ethernet connection
  while (!ETH.linkUp()) {
    delay(1000);
    Serial.println("Waiting for Ethernet...");
  }

  Serial.print("Connected to Ethernet. IP address: ");
  Serial.println(ETH.localIP());

  /* Use MDNS for hostname resolution */
  if (!MDNS.begin(host)) { // http://esp32.local
    Serial.println("Error setting up MDNS responder!");
    while (1) {
      delay(1000);
    }
  }
  Serial.println("mDNS responder started");

  /* Serve pages */
  server.on("/", HTTP_GET, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", loginIndex);
  });
  server.on("/serverIndex", HTTP_GET, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", serverIndex);
  });

  /* Handle firmware update */
  server.on("/update", HTTP_POST, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
    ESP.restart();
  }, []() {
    HTTPUpload& upload = server.upload();
    if (upload.status == UPLOAD_FILE_START) {
      Serial.printf("Update: %s\n", upload.filename.c_str());
      if (!Update.begin(UPDATE_SIZE_UNKNOWN)) { // start with max available size
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_WRITE) {
      /* Flashing firmware to ESP */
      if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_END) {
      if (Update.end(true)) { // true to set the size to the current progress
        Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
      } else {
        Update.printError(Serial);
      }
    }
  });

  server.begin();
}

void loop(void) {
  server.handleClient();
  delay(1);
}
