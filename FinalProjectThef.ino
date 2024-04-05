#include <SoftwareSerial.h>

#define RX_PIN 2
#define TX_PIN 3
#define SPEAKER_PIN 9 
#define PIR_PIN 6 
#define RED_LED_PIN 10 
#define GREEN_LED_PIN 11 

SoftwareSerial SimModule(RX_PIN, TX_PIN);


int pirSensorPin = PIR_PIN;
int redLedPin = RED_LED_PIN;
int greenLedPin = GREEN_LED_PIN;
bool motionDetected = false;
unsigned long lastMotionTime = 0;
const unsigned long cooldownPeriod = 10000;

void setup() {
    pinMode(pirSensorPin, INPUT);
    pinMode(SPEAKER_PIN, OUTPUT);
    pinMode(redLedPin, OUTPUT);
    pinMode(greenLedPin, OUTPUT);
    
    digitalWrite(SPEAKER_PIN, LOW);
    digitalWrite(redLedPin, LOW);
    digitalWrite(greenLedPin, LOW);

    Serial.begin(115200);
    SimModule.begin(115200);
    SimModule.print("AT");
}

void loop() {
    checkSerial();
    checkPIR();
}

void checkSerial() {
    while (SimModule.available()) {
        Serial.println(SimModule.readString()); 
    }
    while (Serial.available()) {
        SimModule.println(Serial.readString());
    }
}

void checkPIR() {
    delay(2000);
    int val = digitalRead(pirSensorPin);
    if(val == HIGH && !motionDetected) {
        Serial.println("Thef or Motion Detected");
        Serial.println("Calling the Owner");
        
        // If Motion Detected
        digitalWrite(redLedPin, HIGH);
        digitalWrite(SPEAKER_PIN, HIGH);
        digitalWrite(greenLedPin, LOW);

        motionDetected = true;
        lastMotionTime = millis();
        
        SimModule.println("ATD+639630488883+;");

        if(millis() - lastMotionTime >= cooldownPeriod) {
            motionDetected = false;
            digitalWrite(SPEAKER_PIN, LOW);       
            digitalWrite(redLedPin, LOW);
            digitalWrite(greenLedPin, HIGH);
        }
    } 
} 

