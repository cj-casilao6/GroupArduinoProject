  #include <WiFiS3.h>


  const int buzzer = 9;
  const int ledPin = 7;  
  const int ledPin2 = 6;
  const int ledPin3 = 5;
  const int ledPin4 = 4;
  const int ledPin5 = 3;
  int waterLevel;
  int waterThreshold = 50;
  unsigned long start = 0;
  String response;
  int playedWarning = 0;
  int timeDiff;
  int waterNum;
  int toneInterval = 300;
  int melody[] = {311, 262, 185};   // the notes
  int noteDurations[] = {300, 300, 300}; // duration for each note
  int currentNote = 0;
  unsigned long lastNoteTime = 0;
  bool playingMelody = false;


  static int lastSaved = 0;


  int ledState;
  const char ssid[] = "MyCoolServer";
  const char pass[] = "mypassword";
  unsigned long timeoutStart = 0;
  int intialized = 0;




  IPAddress serverIP(192, 168, 4, 1); // server to connect to
  const int serverPort = 1234;
  WiFiClient client;


  unsigned long previousMillis = 0;  
  const long interval = 10000;


  void setup() {


    pinMode(buzzer, OUTPUT);
    pinMode(ledPin, OUTPUT);
    pinMode(ledPin2, OUTPUT);
    pinMode(ledPin3, OUTPUT);
    pinMode(ledPin4, OUTPUT);
    pinMode(ledPin5, OUTPUT);


    Serial.begin(9600);
    Serial.print(" Connecting to WiFi");
    while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
      Serial.print(".");
    }


    Serial.println("\n WiFi connected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
  }


void loop() {


    if (millis() - start > 13000 && client.connect(serverIP, serverPort)) {
      start = millis();
      timeoutStart = millis();
      Serial.println("Connected to server");


      client.println("GET_WATER_LEVEL"); // send request for water level
      Serial.println("Request sent.");


      while(!client.available()){
        if(millis()-timeoutStart > 14000){
          waterLevel = lastSaved;
          Serial.println("Server response timed out.");
          client.stop();
          return;
        }
      }


      response = client.readStringUntil('\n');
      waterNum = response.toInt(); // convert response to int
      waterLevel = waterNum;
      lastSaved = waterNum;
     
      client.stop();
     
      Serial.println(waterLevel);


      if(waterLevel <= 100){
        Serial.println("In lowest bracket.");   // light up LEDs based on level
        ledState = LOW;
        digitalWrite(ledPin, ledState);
        digitalWrite(ledPin2, ledState);
        digitalWrite(ledPin3, ledState);
        digitalWrite(ledPin4, ledState);
        digitalWrite(ledPin5, ledState);


        if(playedWarning == 0){
            playingMelody = true;
            currentNote = 0;
            lastNoteTime = millis();
            playedWarning = 1;
        }
      }
      else if(waterLevel > 100 && waterLevel < 121){
        digitalWrite(ledPin, HIGH);
        digitalWrite(ledPin2, LOW);
        digitalWrite(ledPin3, LOW);
        digitalWrite(ledPin4, LOW);
        digitalWrite(ledPin5, LOW);
        playedWarning = 0;
      }
      else if(waterLevel > 120 && waterLevel < 151){
        digitalWrite(ledPin, HIGH);
        digitalWrite(ledPin2, HIGH);
        digitalWrite(ledPin3, LOW);
        digitalWrite(ledPin4, LOW);
        digitalWrite(ledPin5, LOW);
        playedWarning = 0;
      }
      else if(waterLevel > 150 && waterLevel < 181){
        digitalWrite(ledPin, HIGH);
        digitalWrite(ledPin2, HIGH);
        digitalWrite(ledPin3, HIGH);
        digitalWrite(ledPin4, LOW);
        digitalWrite(ledPin5, LOW);
        playedWarning = 0;
      }
      else if(waterLevel > 180 && waterLevel < 220){
        digitalWrite(ledPin, HIGH);
        digitalWrite(ledPin2, HIGH);
        digitalWrite(ledPin3, HIGH);
        digitalWrite(ledPin4, HIGH);
        digitalWrite(ledPin5, LOW);
        playedWarning = 0;
      }
      else if(waterLevel >= 220){
        ledState = HIGH;
        digitalWrite(ledPin, ledState);
        digitalWrite(ledPin2, ledState);
        digitalWrite(ledPin3, ledState);
        digitalWrite(ledPin4, ledState);
        digitalWrite(ledPin5, ledState);
        playedWarning = 0;
      }


      while (playingMelody) {
        unsigned long now = millis();
        if (now - lastNoteTime >= 300) {
          noTone(buzzer);  


          currentNote++;
          if (currentNote < 3) {  
            tone(buzzer, melody[currentNote],300);
            lastNoteTime = now;
          } else {
          playingMelody = false;
          noTone(buzzer);
          }
        }
      }


    }
}

