# Overview
This semester long group project involved designing, building, and presenting a working system that utilizes microcontrollers (Arduinos), external devices (leds, sensors, buzzers, etc.), and some form of communication between components (serial). The system must include original work, beyond just replicating an online tutorial. 

# Abstract
This project involves developing a defense system to protect valuable assets by preventing unauthorized access. The system utilizes multiple Arduinos, each with specific roles that include water management, surveillance, user operation, and alarms. Communication is achieved by using a client-server model over wifi which enables coordinated outputs based on unique inputs based on the specific arduino. Key features include using a motion sensor to detect intruders, shooting water at intruders, and a camera to capture and send images of the intruder to the user. Additionally, LEDs and buzzers provide alerts for water level and motion detection.

# Our Project Components
Our project included used 5 different Arduinos:
- Server: Manages communication between components by interacting with each Arduino to give back some sort of information tailored to each client
- Camera/sensor: Handles intruder detection and takes picture of intruder
- LCD: Displays the status of the system on an LCD. Also prompts the user to enter a password using a joystick to arm the system and pushbuttons to send signals.
- Water pump: Handles shooting water through a water pupmp directly at the intruder along with playing an alarm-like sound through a buzzer.
- Alarm: Retains the level of the water tank through use of LEDs and plays an alarm-like soudn through a buzzer when water tank levels are critically low.

# Arduino 1 (PIR Motion Sensor/Camera)
![image](https://github.com/user-attachments/assets/7e05dbdb-0dc2-422d-8fe2-5d894963e852)
![image](https://github.com/user-attachments/assets/28f6ee78-c066-4115-b0d9-0bf41b64d7d8)

# Arduino 2 (Water Pump/Buzzer/Water Level Sensor/Dual Channel Relay)
![image](https://github.com/user-attachments/assets/015779d2-ffe4-4b70-b99d-49da21a7bbba)
![image](https://github.com/user-attachments/assets/c7b4d47e-4b23-4137-81ac-eb98c5de3b71)


# Arduino 3 & 4 (LCD/Joystick & LED/Buzzer)
![image](https://github.com/user-attachments/assets/c55b8232-269e-41c7-8ca9-9098582f8881)
![image](https://github.com/user-attachments/assets/ce5a0d2d-d328-45f6-a5b4-365e17d3acc9)
![image](https://github.com/user-attachments/assets/fe8cb893-f285-4c69-9cdf-84982c051727)

