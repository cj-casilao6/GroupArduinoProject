#include <WiFiS3.h>
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <stdio.h>
#include <ezButton.h>


#define VRX_PIN  A1 // Arduino pin connected to VRX pin
#define VRY_PIN  A0 // Arduino pin connected to VRY pin
#define SW_PIN   2  // Arduino pin connected to SW  pin


ezButton button(SW_PIN);




// WiFi credentials
char ssid[] = /*"NETGEAR43-5G";//*/"MyCoolServer";
char pass[] = /*"deepstreet273";//*/"mypassword";
IPAddress server(192,168,4,1); // Server IP
uint16_t port = 1234;
WiFiClient client;


unsigned long start = 0;


const int yesButton = 5;
const int noButton = 6;
int response = 0;


LiquidCrystal_I2C lcd(0x27, 16, 2);




int xValue = 0; // To store value of the X axis
int yValue = 0; // To store value of the Y axis
int bValue = 0; // To store value of the button


int presses = 0;
int password[4] = {1,2,3,4};
int attempt[4];


int checkPassword(){
  for(int i=0; i<4; ++i){
    if(attempt[i] != password[i]){
      return 0;
    }
  }
  return 1;
}


void getDirection(){
  int index = presses;
  if(xValue < 250 && yValue < 250){ //1
    //top right
    attempt[index] = 1;
    //presses++;
    Serial.println("Top right");
  }
  else if(xValue > 250 && xValue < 750 && yValue < 250){ //2
    //middle right
    attempt[index] = 2;
    //presses++;
    Serial.println("Middle right");
  }
  else if(xValue > 750 && yValue < 250){ //3
    //bottom right
    attempt[index] = 3;
    //presses++;
    Serial.println("Bottom right");
  }
  //////////////////////////////////////////////////////////////////////////////////////////////
  else if(xValue < 250 && yValue > 250 && yValue < 750){ //4
    //top middle
    attempt[index] = 4;
    //presses++;
    Serial.println("Top Middle");
  }
  else if(xValue > 250 && xValue < 750 && yValue > 250 && yValue < 750){ //5
    //middle middle
    Serial.print("Index: ");
    Serial.println(index);
    attempt[index] = 5;
    //presses++;
    Serial.println("Middle Middle");
    Serial.print("Presses: ");
    Serial.println(presses);
  }
  else if(xValue > 750 && yValue > 250 && yValue < 750){ //6
    //bottom middle
    attempt[index] = 6;
    //presses++;
    Serial.println("Bottom Middle");
  }
  //////////////////////////////////////////////////////////////////////////////////////////////
  else if(xValue < 250 && yValue > 750){ //7
    //top left
    attempt[index] = 7;
    //presses++;
    Serial.println("Top Left");
  }
  else if(xValue > 250 && xValue < 750 && yValue > 750){ //8
    //middle left
    attempt[index] = 8;
    //presses++;
    Serial.println("Middle Left");
  }
  else if(xValue > 750 && yValue > 750){ //9
    //bottom left
    attempt[index] = 9;
    //presses++;
    Serial.println("Bottom Left");
  }
  lcd.setCursor(index,1);
  lcd.print("*");
  presses++;
}


//unsigned long start = 0;
int valid = 0;
unsigned long lastTime = 0;
const unsigned long debounce = 500;




void askQuestions(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.println("Arm system?     ");
  while(response == 0){
    if(digitalRead(yesButton) == LOW && digitalRead(noButton) != LOW){
      response = 1; //yes
      lcd.clear();
    }
    else if(digitalRead(yesButton) != LOW && digitalRead(noButton) == LOW){
      response = 2; //no
      lcd.clear();
    }
  }
}


int tries = 0;
int firstTime = 0;


void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.begin(16,2);


  Serial.print("🔌 Connecting to WiFi");
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }




  Serial.println("\n✅ WiFi connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());


  button.setDebounceTime(50);
}


int detected = 0;
int correct = 0;
int getPassword = 0;
unsigned long checkStart = 0;
unsigned long newStart = 0;
unsigned long timeoutStart = 0;


void loop() {
  button.loop();
//start = millis();
  //Serial.println("Check one");
  // put your main code here, to run repeatedly:
  if(millis() - start > 7000 && client.connect(server, port)){
    Serial.println("Check two");
    if(detected == 0){
      client.println("is something detected?");
      Serial.println("Check Three");
      timeoutStart = millis();
      while(!client.available()){
        if(millis()-timeoutStart > 8000){
          Serial.println("Server response timed out.");
          client.stop();
          start = millis();
          return;
        }
      }
      Serial.println("Check four");
      Serial.println("Client Available");
      String reply = client.readStringUntil('\n');
      Serial.println(reply);
      reply.trim();
      if(reply == "isDetected is set to True"){
        Serial.println("Received message from server: isDetected is True!");
        detected = 1;
        lcd.setCursor(0,0);
        lcd.println("Motion Detected!");
        if(firstTime == 0){
          //correct = getPassword();
          getPassword = 1;
          firstTime = 1;
          lcd.setCursor(0,0);
          lcd.println("Enter Password: ");
        }
      }
      else{
        Serial.println("Received message from server: isDetected is False!");
      }
    }
    if(response == 1){
      client.println("System is armed!");
      Serial.println("Sending signal to arm system!");
      lcd.setCursor(0,0);
      lcd.clear();
      lcd.println("System armed!   ");
      detected = 0;
      response = 0;
      getPassword = 0;
    }
    else if(response == 2){
      client.println("System not armed!");
      Serial.println("Sending signal to not arm system!");
      lcd.setCursor(0,0);
      lcd.clear();
      lcd.println("System not armed");
      detected = 0;
      response = 0;
      getPassword = 0;
    }
    start = millis();
  }
  client.stop();
  if(getPassword == 1){
    //Serial.println("In here!");
    //if((millis()-lastTime) > debounce){
      //Serial.println("Now in here");
      if(button.isPressed() && presses < 4){
        lastTime = millis();
        xValue = analogRead(VRX_PIN);
        yValue = analogRead(VRY_PIN);
        bValue = button.getState();
        Serial.print("x = ");
        Serial.print(xValue);
        Serial.print(", y = ");
        Serial.print(yValue);
        Serial.print(" : button = ");
        Serial.println(bValue);
        Serial.print("Direction: ");
        getDirection();
      }
      if(presses == 4){
        correct = checkPassword();
      }
      //if(millis()-checkStart > 5000){
        //checkStart = millis();
        if(presses == 4){
          if(correct == 1){
            Serial.println("Correct Password!");
          }
          else{
            Serial.println("Incorrect Password! Try Again!");
          }
          presses = 0;
        }
      //}
    //}
    //getDirection();
  }/*
  if(presses == 4){
    correct = checkPassword();
  }*/
  if(detected == 1 && correct == 1 && response == 0){
    askQuestions();
  }
}























