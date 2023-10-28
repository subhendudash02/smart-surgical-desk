 #include <WiFi.h>
 #include <ESP32_Servo.h>
 #include <HTTPClient.h>
 #include <bits/stdc++.h>

const char* ssid = "ssid";
const char* password =  "password";

#define base_pin 13
#define arm_pin 12
#define claw_pin 14

Servo base;
Servo arm;
Servo claw;

void setup() {
  
  base.attach(base_pin);
  arm.attach(arm_pin);
  claw.attach(claw_pin);
  Serial.begin(115200);
  delay(4000);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  
  Serial.println("Connected to the WiFi network");
  Serial.println("arm setup");
  base.write(0);
  delay(2000);
  arm.write(0);
  delay(2000);
  claw.write(0);
  delay(2000);
  Serial.println("done setup");
  delay(2000);
  claw.write(90);
  Serial.println("arm closed");
  delay(2000);
  claw.write(0);
  Serial.println("arm open");
  
}
  
void loop() {
  
  if ((WiFi.status() == WL_CONNECTED)) { //Check the current connection status
  
    HTTPClient http;//http object
  
    http.begin("http://192.168.114.66:5000/api/hand_gesture/"); //Specify the URL
    int httpCode = http.GET();                                        //Make the request and storing the request code
  
    if (httpCode > 0) { //Check for the returning code
  
        String payload = http.getString();
        Serial.println(httpCode);
        Serial.println(payload);
        int n=payload.length();
        char c=payload[n-2];
        int finger=(int)c-'0';
        Serial.println(finger);

        switch (finger){

          case 1:

          for(int i=0;i<=120;i+=5){
          arm.write(i);
          delay(20);
          }
          for(int i=0;i<=60;i+=5){
          base.write(i);
          delay(20);
          }
          for(int i=120;i>=0;i-=5){
          arm.write(i);
          delay(20);
          }
            
          delay(1000); 
          claw.write(95);
          Serial.println("arm closed");
          delay(1000);
          
          for(int i=0;i<=120;i+=5){
          arm.write(i);
          delay(20);
          }
          for(int i=60;i>=0;i-=5){
          base.write(i);
          delay(20);
          }
          for(int i=120;i>=0;i-=5){
          arm.write(i);
          delay(20);
          }
          claw.write(0);
          Serial.println("arm open");        
          delay(1000);       
          break;

          case 2:
          for(int i=0;i<=120;i+=5){
          arm.write(i);
          delay(20);
          }
          for(int i=0;i<=120;i+=5){
          base.write(i);
          delay(20);
          }
          for(int i=120;i>=0;i-=5){
          arm.write(i);
          delay(20);
          }          
          delay(1000); 
          claw.write(95);
          Serial.println("arm closed");
          delay(1000);
          
          for(int i=0;i<=120;i+=5){
          arm.write(i);
          delay(20);
          }          
          for(int i=120;i>=0;i-=5){
          base.write(i);
          delay(20);
          }
          for(int i=120;i>=0;i-=5){
          arm.write(i);
          delay(20);
          }
          
          claw.write(0);
          Serial.println("arm open");        
          delay(1000);      
          break;

          case 3:
          for(int i=0;i<=120;i+=5){
          arm.write(i);
          delay(20);
          }
          for(int i=0;i<=180;i+=5){
          base.write(i);
          delay(20);
          }
          
          for(int i=120;i>=0;i-=5){
          arm.write(i);
          delay(20);
          }
                    
          delay(1000); 
          claw.write(95);
          Serial.println("arm closed");
          delay(1000);       
                
          for(int i=0;i<=120;i+=5){
          arm.write(i);
          delay(20);
          }          
          for(int i=180;i>=0;i-=5){
          base.write(i);
          delay(20);
          }
          for(int i=120;i>=0;i-=5){
          arm.write(i);
          delay(20);
          }
          claw.write(0);
          Serial.println("arm open");        
          delay(1000);      
          break;

          default:
          base.write(0);
          arm.write(0);
          claw.write(0);
          delay(1000);
          break;
        }
      }
  
    else {
      Serial.println("Error on HTTP request");
    }

    http.end(); //Free the resources
  }
  
  delay(2000);
  
}
