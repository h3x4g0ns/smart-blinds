//---------------------------------------------------
#include <ESP8266WiFi.h>
const char *ssid = "";
const char *password = "";
//---------------------------------------------------
#include <ESP8266WebServer.h>
ESP8266WebServer server(80);  //listen to port 80
//---------------------------------------------------

#include <Servo.h>
Servo myservo;

int pos = 0;

void setPosition() {
  Serial.println("New request for \"/\"");
  if(server.hasArg("pos") && server.arg("pos").toInt() >=0 && server.arg("pos").toInt() <=100){
    myservo.attach(2);
    myservo.write(((double)server.arg("pos").toInt()*180/100));
    delay(1000);
    myservo.detach();
    Serial.print("Setting servo pos to:");
    Serial.println(((double)server.arg("pos").toInt()*180/100));
    pos = server.arg("pos").toInt();
  }
  server.send ( 200, "application/json",  "{\"position\":\"" + String(pos) + "\"}"  );
  Serial.println("Displaying status");
}

void setup() {
  Serial.begin(115200); //start the serial output
  Serial.println();
  Serial.println("Starting up");
  
  myservo.write(pos);
  
  Serial.print("Connecting to ");
  Serial.println(ssid);
  // Print local IP address and start web server
  Serial.println("");
  //set mac address
  uint8_t mac[6] {0x5C, 0x26, 0x19, 0x40, 0x86, 0x60}; 
  wifi_set_macaddr(0, const_cast<uint8*>(mac));
  //disable ap advertising 
  WiFi.mode(WIFI_STA);
  //connect to wifi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  
  Serial.println("------------------------------");
  server.on ( "/", setPosition );
  server.begin(); //start the webserver
  Serial.println("Webserver started");

  Serial.println("Startup finished");
  Serial.println("****************");
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
  server.handleClient();  //process all the requests for the Webserver
}
