#include <Arduino.h>
#include <avr/io.h>
#include <FreeRTOS.h>
#include <task.h>

#define STACK_SIZE 200
#define leftAin1  4
#define leftAin2  3
#define rightAin1  6
#define rightAin2  5
#define frontLED1 8
#define frontLED2 9
#define frontLED3 10
#define frontLED4 11
#define frontLED5 12
#define frontLED6 13
#define frontLED7 7
#define frontLED8 2
#define Audio A0
#define D  294
#define E  330
#define F  349
#define G  392

char commandReceived;
boolean isRunning = false;

void babyShark(){
  for(int i=0;i<3;i++)
  {
  	tone(Audio,D,500);
  	delay(200);
  	tone(Audio,E,500);
  	delay(200);
  	 for(int j=0;j<7;j++)
  	 {
  	 	tone(Audio,G,250);
  	 	delay(200);
  	 }
  	tone(Audio,G,500);
  	delay(200);
  	tone(Audio,G,500);
  	delay(200);
  	tone(Audio,F,250);
  	delay(1000);
  }
} 

/* Control the action of motors*/
void tMotorControl(void *p){
    while(1){
        /* Move forward*/
        if(commandReceived == 'a'){
            //R forward
            analogWrite(rightAin1, 128);
            digitalWrite(rightAin2, LOW);
            //L forward
            analogWrite(leftAin2, 128);
            digitalWrite(leftAin1, LOW);
        }

        /* Turn left*/
        else if(commandReceived == 'b'){
            //R forward
            analogWrite(rightAin1, 128);
            digitalWrite(rightAin2, LOW);
            //L backward
            analogWrite(leftAin1, 128);
            digitalWrite(leftAin2, LOW);
        }

        /* Turn right*/
        else if(commandReceived == 'c'){
            //R backward
            analogWrite(rightAin2, 190);
            digitalWrite(rightAin1, LOW);
            //L forward
            analogWrite(leftAin2, 128);
            digitalWrite(leftAin1, LOW);
        }

        /* Move backward*/
        else if(commandReceived == 'd'){
            //L backward
            analogWrite(rightAin2, 190);
            digitalWrite(rightAin1, LOW);
            //R backward
            analogWrite(leftAin1, 128);
            digitalWrite(leftAin2, LOW);
        }

        /* Stop*/
        else if(commandReceived == 'e'){
            //L stop
            analogWrite(leftAin1, 0);
            digitalWrite(leftAin2, 0);
            //R stop
            analogWrite(rightAin1, 0);
            digitalWrite(rightAin2, 0);
        }
        
        else{
            Serial.println(commandReceived);
        }
    }
}

/* Control the LEDs*/
void tLED(void *p){
    while(1){

        /* LEDs in running mode*/
        if(isRunning){
            digitalWrite(frontLED1, LOW);
            digitalWrite(frontLED2, LOW);
            digitalWrite(frontLED3, LOW);
            digitalWrite(frontLED4, LOW);
            digitalWrite(frontLED5, LOW);
            digitalWrite(frontLED6, LOW);
            digitalWrite(frontLED7, LOW);
            digitalWrite(frontLED8, LOW);
            for(int j = 8; j < 14; j++){
            if(!isRunning) break;
            digitalWrite(j, HIGH);
            delay(250);
            digitalWrite(j, LOW);
            delay(250);
            if(!isRunning) break;
            if(j == 13){
                if(!isRunning) break;
                j = 2;
                digitalWrite(j, HIGH);
                delay(250);
                digitalWrite(j, LOW);
                delay(250);
                j = 7;
            }
            if(j == 7){
                if(!isRunning) break;
                digitalWrite(j, HIGH);
                delay(250);
                digitalWrite(j, LOW);
                delay(250);
                j = 8;
            }
            }
        }
        /* LEDs in stationary mode*/
        else{
            digitalWrite(frontLED1, HIGH);
            digitalWrite(frontLED2, HIGH);
            digitalWrite(frontLED3, HIGH);
            digitalWrite(frontLED4, HIGH);
            digitalWrite(frontLED5, HIGH);
            digitalWrite(frontLED6, HIGH);
            digitalWrite(frontLED7, HIGH);
            digitalWrite(frontLED8, HIGH);
        }
    }
}

/* Provide audio output*/
void tAudio(void *p){
    if(commandRecieved == 'i')
        babyShark();
    //else if(blueToothVal == '7')
    //pleyEndAudio();
    //else if()
    //babyShark();
}

/* Monitor the serial port*/
void tSerial(void *p){
    while(1){
	    if(Serial.available()){
            commandReceived = Serial.read();
            if(commandReceived == 'a' || commandReceived == 'b' || commandReceived == 'c' || commandReceived == 'd'){
                isRunning = true;
            }else if(commandReceived == 'e'){
                isRunning == false;
            }
            else{
                isRunning = false;
            }
        }
    }
}

void setup(){
    Serial.begin(9600);
    pinMode(rightAin1, OUTPUT);
    pinMode(rightAin2, OUTPUT);
    pinMode(leftAin1, OUTPUT);
    pinMode(leftAin2, OUTPUT);
    Serial.println("Type the command...");
}

void loop() {
	xTaskCreate(tSerial, "Serial", STACK_SIZE, NULL, 1, NULL);
	xTaskCreate(tMotorControl, "MotorControl", STACK_SIZE, NULL, 1, NULL);
    xTaskCreate(tLED, "LEDs", STACK_SIZE, NULL, 1, NULL);
	//xTaskCreate(task3, "Task3", STACK_SIZE, NULL, 1, NULL);
	/* start scheduler */
	vTaskStartScheduler();
}
