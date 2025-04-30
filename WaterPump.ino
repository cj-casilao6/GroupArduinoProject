#include <WiFiS3.h> 

// Pin assignments 
const int waterSensorPin = A0; // Analog pin for water level sensor 
const int relayPin = 4;        // Digital pin for dual channel relay 
const int buzzPin = 7;         // Digital pin for buzzer 

// WiFi credentials 
char ssid[] = "MyCoolServer"; 
char pass[] = "mypassword"; 

// Server settings 
IPAddress server(192, 168, 4, 1); // Server IP 
uint16_t port = 1234;             // Server port 
WiFiClient client; 

// Pump control variables 
const unsigned int buzzInterval = 200;           // Buzzer on/off every .2 secs
const unsigned int waterThreshold = 100;         // Minimum water level reading
const unsigned long waterCheckInterval = 1000;   // Interval for water level checking
const unsigned long serverInterval = 10000;      // Interval for server communication
const unsigned long pumpDuration = 4000;         // Activate pump for 4 secs

// Variables to be updated
unsigned long lastWaterCheck = 0; 
unsigned long prevMillis = 0; 
unsigned int currentWaterLevel = 0;
unsigned int pumpWaterLevel = 0; 

const unsigned long serialDelay = 2000; // 2 second delay for serial monitor


void setup() { 
  // Initialize pins as output
  pinMode(relayPin, OUTPUT); 
  pinMode(buzzPin, OUTPUT); 

  digitalWrite(relayPin, LOW);  // Ensure relay is off on startup
  
  Serial.begin(9600); 

  // Allow time for serial monitor to boot
  unsigned long startMillis = millis();
  while (millis() - startMillis < serialDelay) {}

  // Connect to WiFi
  Serial.println("Connecting to WiFi..."); 
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) { 
    Serial.println(".");
  } 

  // WiFi details
  Serial.println("\nWiFi connected!"); 
  Serial.print("IP Address: "); 
  Serial.println(WiFi.localIP()); 
}


void loop() { 
  unsigned long currMillis = millis();  

  // Check water level every waterCheckInterval (1 sec)
  if (currMillis - lastWaterCheck >= waterCheckInterval) {
    lastWaterCheck = currMillis;
    currentWaterLevel = analogRead(waterSensorPin);
  }

  // Interact with server every serverInterval
  if (currMillis - prevMillis >= serverInterval) { 

    // Attempt to connect to server
    if (client.connect(server, port)) { 
      Serial.println("-----------------------------"); 
      Serial.println("Connected to server."); 
      client.println("Is System armed?");   // Request system status from server
      
      unsigned long timeoutStart = millis();  // Timeout for server response
      while (!client.available()) { 
        // Water level checks while waiting for server response (every 1 sec)
        if (millis() - lastWaterCheck >= waterCheckInterval) {
          lastWaterCheck = millis();
          currentWaterLevel = analogRead(waterSensorPin);
          
          Serial.println("Water Level (server wait): " + String
          (currentWaterLevel));
        }
        
        // Stop waiting if server response takes too long (One second longer than serverInterval)
        if (millis() - timeoutStart > 11000) { 
          Serial.println("Server response timed out."); 
          client.stop(); 
          prevMillis = millis(); 
          return; 
        } 
      } 

      // Process response from server (validate message)
      String message = client.readStringUntil('\n'); 
      message.trim(); 
      Serial.println("-------------------------------");
      Serial.println("Received: " + message); 
      
      // Valid response from server
      if (message == "isArmed is set to True") {
        // Use the most recent water level reading
        if (currentWaterLevel > waterThreshold) { 
          // Send water level to server for alarm arduino
          client.println("Send Water level: " + String(currentWaterLevel)); 
          Serial.println("Water level sent to server: " + String(currentWaterLevel)); 
          activatePumpAndBuzzer();  // Activate pump/buzzer
        } 
        // Insufficient water level
        else if (currentWaterLevel < waterThreshold){  
          // Send water level to server for alarm arduino
          client.println("Send Water level: " + String(currentWaterLevel)); 
          Serial.println("INSUFFICIENT WATER. PUMP OFF!"); 
          Serial.println("Low water level: " + String(currentWaterLevel)); 
        } 
        // Invalid message from server
      }  else{
          client.println("Send Water level: " + String(currentWaterLevel)); 
          Serial.println("INVALID MESSAGE. PUMP OFF!");
          Serial.println("Water level sent to server: " + String(currentWaterLevel)); 
      }
      
      client.stop(); 
    
    // Unable to connect to server
    } else { 
      Serial.println("Failed to connect to server."); 
    } 

    prevMillis = currMillis; 
  } 
} 

void activatePumpAndBuzzer() {
  Serial.println("VALID MESSAGE. ACTIVATING PUMP & ALARM!");
  
  unsigned long startMillis = millis(); // Start the timer for pump activation
  digitalWrite(relayPin, HIGH);         // Turn on the pump
  Serial.println("...");

  while (millis() - startMillis < pumpDuration) {
    if (millis() - lastWaterCheck >= waterCheckInterval) {
      lastWaterCheck = millis();
      pumpWaterLevel = analogRead(waterSensorPin); // Read water level during pump operation
      Serial.println("Water Level (pump active): " + String(pumpWaterLevel));
    }

    // Buzzer logic: alternate between two tones for alarm sound
    unsigned long buzzCycle = millis() % 400;
    if (buzzCycle < buzzInterval) {
      tone(buzzPin, 1000); // High-pitched tone
    } else {
      tone(buzzPin, 600);  // Low-pitched tone
    }
  }

  // Turn off pump and buzzer after duration
  Serial.println("...");
  Serial.println("DONE SHOOTING. PUMP & ALARM OFF!");
  digitalWrite(relayPin, LOW);
  noTone(buzzPin);
}