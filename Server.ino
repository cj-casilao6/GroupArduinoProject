#include <WiFiS3.h>


// making this arduino act as a wifi router (access point) for other arduinos to connect to
// WiFi credentials
char ssid[] = "MyCoolServer"; //username for router
char pass[] = "mypassword"; // password for router




WiFiServer server(1234); // server port number
int status = WL_IDLE_STATUS;




// Variables handled by client
bool isDetected = false;
bool isArmed = false; //make false
String getWaterLevel = "0";




void setup() {
  Serial.begin(9600);
  while (!Serial);




  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("No WiFi module found!");
    while (true);
  }




  Serial.println("Starting Access Point...");




  // set static IP for AP
  WiFi.config(IPAddress(192, 168, 4, 1));




  // Start AP mode
  status = WiFi.beginAP(ssid, pass);
  if (status != WL_AP_LISTENING) {
    Serial.println("Failed to start Access Point");
    while (true);
  }




  Serial.println("Access Point started!");
  printWiFiStatus();




  server.begin();
}




void loop() {
  WiFiClient client = server.available();
  if (client) {
    Serial.println("Client connected!");


    while (client.connected() && client.available()) {
      //delay(5);
        String message = client.readStringUntil('\n');
        message.trim();
        Serial.print("Received: ");
        Serial.println(message);




        if (message == "detected something") {
          Serial.println("isDetected set to TRUE");
          client.println("Status updated to true");
          isDetected = true;




        } else if (message == "is something detected?") {
          Serial.print("isDetected is: ");
          Serial.println(isDetected ? "TRUE" : "FALSE");
          client.println(isDetected ? "isDetected is set to True" : "isDetected is set to False");
          //if(isDetected) isDetected = false; //added this to reset detection sensor






        } else if (message == "GET_WATER_LEVEL") {
          Serial.print("Water level: ");
          Serial.println(getWaterLevel);
          client.println(getWaterLevel);




        }else if (message == "System is armed!") {
          isArmed = true;
          if(isDetected) isDetected = false; //added this to reset detection sensor
          Serial.print("isArmed is set to true!");
          client.println("isArmed is set to true!");




        } else if(message == "Is System armed?") {
            Serial.print("isArmed is: ");
            Serial.println(isArmed ? "TRUE" : "FALSE");
            client.println(isArmed ? "isArmed is set to True" : "isArmed is set to False");
            if(isArmed) {
              isArmed = false; //added this to reset keypad
            }


        }
        else if(message.startsWith("Water level: ")){
          getWaterLevel = message.substring(13);
          Serial.println(getWaterLevel);
          client.println(getWaterLevel); //fix


        }
        else if(message.startsWith("Send Water level: ")){
          getWaterLevel = message.substring(18);
          Serial.println(getWaterLevel);
          //client.println(getWaterLevel); //fix


        }


        else {
          client.println("Unknown command");
        }




        client.flush();
        //delay(50);
       // break;
    }




    Serial.println("go again");
    //client.stop();
  }
  //Serial.println("finished request");
}




void printWiFiStatus() {
  Serial.print("AP SSID: ");
  Serial.println(WiFi.SSID());




  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);




  //Serial.print("Clients connected: ");
  //Serial.println(WiFi.apClientCount());
}















