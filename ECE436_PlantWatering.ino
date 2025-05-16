#include <WiFi.h>
#include <NetworkClient.h>
#include <WebServer.h>
#include "arduino_secrets.h"
#include "control.h"
#include "login.h"
#include "webpage.h"
#include <ArduinoJson.h>
#include "Pump_Relay.h"
#include "OLED_Debug.h"
#include "PlantSensors.h"
#include <Preferences.h>

Preferences preferences;

const char *ssid = SECRET_SSID;
const char *pass = SECRET_PASS;

String username = "";
String password = "";

WebServer server(80);

bool manualMode = 0;
int prevWateringTime = 0;
bool manualCircMode = 0;
int prevCircTime = 0;

// Stop button is attached to PIN 0 (IO0)
#define BTN_STOP_ALARM 0
int count = 0;

//Check if header is present and correct
int is_authentified() {
  Serial.println("Enter is_authentified");
  if (server.hasHeader("Cookie")) {
    Serial.print("Found cookie: ");
    String cookie = server.header("Cookie");
    Serial.println(cookie);
    if (cookie.indexOf("ESPSESSIONID=1") != -1) {
      Serial.println("Authentication Successful (admin)");
      return 1;
    }
  }
  Serial.println("Authentication Failed");
  return 0;
}

//logout
void handleLogout() {
  if (server.hasHeader("Cookie")) {
    Serial.print("Found cookie: ");
    String cookie = server.header("Cookie");
    Serial.println(cookie);
  }
  Serial.println("Disconnection");
  server.sendHeader("Location", "/webpage.html");
  server.sendHeader("Cache-Control", "no-cache");
  server.sendHeader("Set-Cookie", "ESPSESSIONID=0");
  server.send(301);
  return;
}

//login
void handleLogin() {
  String msg;
  if (server.hasHeader("Cookie")) {
    Serial.print("Found cookie: ");
    String cookie = server.header("Cookie");
    Serial.println(cookie);
  }
  if (server.hasArg("username") && server.hasArg("password")) {
    if (server.arg("username") == username && server.arg("password") == password) {
      server.sendHeader("Location", "/control.html");
      server.sendHeader("Cache-Control", "no-cache");
      server.sendHeader("Set-Cookie", "ESPSESSIONID=1");
      server.send(301);
      Serial.println("Log in Successful");
      return;
    }
    msg = "Wrong username/password! try again.";
    Serial.println("Log in Failed");
  }
  // redirect to login page
  server.sendHeader("Location", "/login.html");
  server.sendHeader("Cache-Control", "no-cache");
  server.sendHeader("Set-Cookie", "ESPSESSIONID=0");
  server.send(301);
}

void handleModes() {
  Serial.println("Have Modes");
  String payload = server.arg("plain");
  Serial.println(payload);
  if (payload == "Manual") {
    if (getWatererOnTime() > 0) {
      prevWateringTime = getWatererOnTime();
    }
    setWatererOnTime(0);
    manualMode = true;
  } else if (payload == "Quiet") {
    if (getBubblerOnTime() > 0) {
      prevCircTime = getBubblerOnTime();
    }
    setBubblerOnTime(0);
    manualCircMode = true;
  } else {
    // assume Auto
    if (prevWateringTime > 0) {
      setWatererOnTime(prevWateringTime);
    }
    if (prevCircTime > 0) {
      setBubblerOnTime(prevCircTime);
    }
    manualCircMode = false;
    manualMode = false;
  }
  if ((server.header("Accept").indexOf("html") >= 0) ||
        (server.header("Accept").indexOf("*/*") >= 0)) {
    server.send(200, "text/plain", "true");
  } else if (server.header("Accept").indexOf("json") >= 0) {
    server.send(200, "application/json", "{\"status\": 200}");
  }
}

void handlePlantType() {
  Serial.println("Have Plant Type");
  String payload = server.arg("plain");
  Serial.println(payload);
  if (payload == "Succulent"){
    Serial.println("Is a succulent");
    setMinSoilMoisture(30);
  }
  else if (payload == "Cacti"){
    Serial.println("Is a Cacti");
    setMinSoilMoisture(20);
  }
  else{
    setMinSoilMoisture(30);
  }
  if ((server.header("Accept").indexOf("html") >= 0) ||
        (server.header("Accept").indexOf("*/*") >= 0)) {
    server.send(200, "text/plain", "true");
  } else if (server.header("Accept").indexOf("json") >= 0) {
    server.send(200, "application/json", "{\"status\": 200}");
  }
}

void handleReset() {
  Serial.println("Have Reset");
  String payload = server.arg("plain");
  Serial.println(payload);
  setBubblerOnTime(20);         // input in seconds, this is 20  sec
  setBubblerOffTime(3*60*60);   // input in seconds, this is 3   hrs
  setWatererOnTime(20);         // input in seconds, this is 20  sec
  setWatererOffTime(12*60*60);  // input in seconds, this is 12  hrs
  if ((server.header("Accept").indexOf("html") >= 0) ||
        (server.header("Accept").indexOf("*/*") >= 0)) {
    server.send(200, "text/plain", "true");
  } else if (server.header("Accept").indexOf("json") >= 0) {
    server.send(200, "application/json", "{\"status\": 200}");
  }
}

void handleSetWaterings() {
  Serial.println("Have Watering set");
  String payload = server.arg("plain");
  Serial.println(payload);
  int space = payload.indexOf(" ");
  int time1 = payload.substring(0, space).toInt();
  int time2 = payload.substring(space + 1).toInt();
  setWatererOnTime(time1);
  setWatererOffTime(time2);
  if ((server.header("Accept").indexOf("html") >= 0) ||
        (server.header("Accept").indexOf("*/*") >= 0)) {
    server.send(200, "text/plain", "true");
  } else if (server.header("Accept").indexOf("json") >= 0) {
    server.send(200, "application/json", "{\"status\": 200}");
  }
}

void handleCirculation() {
  Serial.println("Have Circ");
  String payload = server.arg("plain");
  Serial.println(payload);
  if (payload == "on") {
    prevCircTime = getBubblerOnTime();
    setBubblerOnTime(0);
    manualCircMode = true;
  } else {
    setBubblerOnTime(prevCircTime);
    manualCircMode = false;
  }
  if ((server.header("Accept").indexOf("html") >= 0) ||
        (server.header("Accept").indexOf("*/*") >= 0)) {
    server.send(200, "text/plain", "true");
  } else if (server.header("Accept").indexOf("json") >= 0) {
    server.send(200, "application/json", "{\"status\": 200}");
  }
}

void handleManual() {
  Serial.println("Have Manual");
  String payload = server.arg("plain");
  Serial.println(payload);
  if (payload == "on") {
    prevWateringTime = getWatererOnTime();
    setWatererOnTime(0);
    manualMode = true;
  } else {
    if (prevWateringTime > 0) {
      setWatererOnTime(prevWateringTime);
    }
    manualMode = false;
  }
  if ((server.header("Accept").indexOf("html") >= 0) ||
        (server.header("Accept").indexOf("*/*") >= 0)) {
    server.send(200, "text/plain", "true");
  } else if (server.header("Accept").indexOf("json") >= 0) {
    server.send(200, "application/json", "{\"status\": 200}");
  }
}

void handleSetCreds() {
  Serial.println("Have Set Creds");
  String payload = server.arg("plain");
  Serial.println(payload);
  int space = payload.indexOf(" ");
  username = payload.substring(0, space);
  password = payload.substring(space + 1);
  preferences.putString("ssid", username.c_str());
  preferences.putString("pass", password.c_str());
  preferences.end();
  if ((server.header("Accept").indexOf("html") >= 0) ||
        (server.header("Accept").indexOf("*/*") >= 0)) {
    server.send(200, "text/plain", "true");
  } else if (server.header("Accept").indexOf("json") >= 0) {
    server.send(200, "application/json", "{\"status\": 200}");
  }
}

void handleCredentials() {
  Serial.println("Check Creds");
  if ((server.header("Accept").indexOf("html") >= 0) ||
        (server.header("Accept").indexOf("*/*") >= 0)) {
    server.send(200, "text/plain", (username == "" || password == "") ? "false" : "true");
  } else if (server.header("Accept").indexOf("json") >= 0) {
    if (username == "" || password == "") {
      server.send(200, "application/json", "{\"hasCredentials\": \"false\"}");
    } else {
      server.send(200, "application/json", "{\"hasCredentials\": \"true\"}");
    }
  }
}

void handleVariables() {
  Serial.println("Have Check Creds");
  if ((server.header("Accept").indexOf("html") >= 0) ||
      (server.header("Accept").indexOf("*/*") >= 0)) {
    char logOutput[40];
    snprintf(logOutput, 40, "%d seconds, %d seconds, %.2f, %d", 
      getBubblerTimerTrigger(), 
      (getBubblerRelayState()) ? getBubblerOffTime() - getBubblerTimerTrigger() : 0, 
      returnHumid(), 
      getSoilMoisture());
    server.send(200, "text/plain", logOutput);
  } else if (server.header("Accept").indexOf("json") >= 0) {
    char logOutput[100];
    snprintf(logOutput, 100, "{\"LastWatered\": %d, \"NextWater\": %d, \"Humidity\": %.2f, \"SoilMoisture\": %d}", 
      getBubblerTimerTrigger(), 
      (getBubblerRelayState()) ? getBubblerOffTime() - getBubblerTimerTrigger() : 0, 
      returnHumid(), 
      getSoilMoisture());
    server.send(200, "application/json", logOutput);
  }
}

// Control page can be accessed only if authentication is ok
void handleControl() {
  Serial.println("Enter handleControl");
  String header;
  if (!is_authentified()) { 
    server.sendHeader("Location", "/login.html");
    server.sendHeader("Cache-Control", "no-cache");
    server.send(301);
    return;
  }
  server.send(200, "text/html", control);
}

// Login Page
void handleServeLogin() {
  server.send(200, "text/html", login);
}

// Main Page
void handleRoot() {
  server.send(200, "text/html", webpage);
}

// Error Page
void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setup(void) {
  int counter = 0;
  Serial.begin(115200);
  preferences.begin("credentials", false); 
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  Serial.println("");

  pinMode(BTN_STOP_ALARM, INPUT);

  setupRelay(19, 20);
  // setBubblerOnTime(20);         // input in seconds, this is 20  sec
  // setBubblerOffTime(3*60*60);   // input in seconds, this is 3   hrs
  // setWatererOnTime(20);         // input in seconds, this is 20  sec
  // setWatererOffTime(12*60*60);  // input in seconds, this is 12  hrs
  OLED_setup();
  DHT_setup(6);
  setupSensorPins(7,5);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    counter++;
    if(counter >= 60){ //after 30 seconds timeout - reset board
      ESP.restart();
    }
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/",                handleRoot);
  server.on("/webpage.html",    handleRoot);
  server.on("/login.html",      handleServeLogin);
  server.on("/control.html",    handleControl);
  server.on("/login",           handleLogin);
  server.on("/logout",          handleLogout);
  server.on("/modes",           handleModes);
  server.on("/plant_type",      handlePlantType);
  server.on("/reset",           handleReset);
  server.on("/setWaterings",    handleSetWaterings);
  server.on("/manual",          handleManual);
  server.on("/circ",            handleCirculation);
  server.on("/setUIDPASS",      handleSetCreds);
  server.on("/hasCredentials",  handleCredentials);
  server.on("/variables",       handleVariables);
  server.on("/inline", []() {
    server.send(200, "text/plain", "this works without need of authentication");
  });

  server.onNotFound(handleNotFound);
  //here the list of headers to be recorded
  const char *headerkeys[] = {"User-Agent", "Cookie", "Accept", "Accept-Encoding"};
  size_t headerkeyssize = sizeof(headerkeys) / sizeof(char *);
  //ask server to track these headers
  server.collectHeaders(headerkeys, headerkeyssize);
  server.begin();
  username = preferences.getString("ssid", "");
  password = preferences.getString("pass", "");
  delay(2000);
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
  delay(2);  //allow the cpu to switch to other tasks
  // If button is pressed
  if (digitalRead(BTN_STOP_ALARM) == LOW) {
    // endRelayTimer();
    delay(5); // debounce
    // setBubblerOnTime(getBubblerOnTime() + 3);
    Serial.println(getBubblerOnTime());
    Serial.println(getBubblerOffTime());
    Serial.println(getWatererOnTime());
    Serial.println(getWatererOffTime());
    // Serial.println("Temp: " + String(returnTemp()));
    // Serial.println("Humid: " + String(returnHumid()));
    // int soilValue = getSoilMoisture();
    // int waterValue = getWaterLevel();
    // Serial.print("Soil Moisture Value: ");
    // Serial.println(soilValue);
    // Serial.print("Water level Value: ");
    // Serial.println(waterValue);
    // showText(0, 0, String(getBubblerTimerTrigger())); // count
    // readDHT();
    Serial.println(manualMode);
    Serial.println(prevWateringTime);
    Serial.println(manualCircMode);
    Serial.println(prevCircTime);
    while (digitalRead(BTN_STOP_ALARM) == LOW);
  }
  showText(0, 0, String(getWatererTimerTrigger())); // count
}
