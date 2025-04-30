#include <WiFiS3.h>


// WiFi credentials
//char ssid[] = "Abuadas";
//char pass[] = "Moona123";
char ssid[] = "MyCoolServer";
char pass[] = "mypassword";




//IPAddress server(10, 0, 0, 115); // Server IP
IPAddress server(192, 168, 4, 1);
uint16_t port = 1234;
WiFiClient client;


// Ultrasonic Sensor Pins


const int trigPin = 9;
const int echoPin = 10;
float distance = 0;
const float thresholdCM = 100.0;


void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);


  Serial.print("Connecting to WiFi");
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    Serial.print(".");
    //delay(1000);
  }


  Serial.println("\nWiFi connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}


void loop() {
  // Measure distance
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);


  float duration = pulseIn(echoPin, HIGH);
  distance = (duration * 0.0343) / 2;


  //Serial.print("📏 Distance: ");
  //Serial.print(distance);
  //Serial.print(" cm");


  if (distance > 0 && distance < thresholdCM) {
    Serial.println("Detected something. Sending alert...");
    Serial.println("Take a picture");


    if (client.connect(server, port)) {
      client.println("detected something");


      // Wait for a response (timeout after 10s)
      unsigned long start = millis();
      while (!client.available() && millis() - start < 100000);


      if (client.available()) {
        String reply = client.readStringUntil('\n');
        Serial.print("Server replied: ");
        Serial.println(reply);
      } else {
        Serial.println("No response from server.");
      }


      client.stop();
      Serial.println("Connection closed.");
    } else {
      Serial.println("Failed to connect to server.");
    }


   // delay(3000); // Avoid spamming server
  }


  //delay(300); // General loop delay
}




