#include <Servo.h>

#define FLAME_SENSOR_LEFT A0
#define FLAME_SENSOR_RIGHT A1

#define MOTOR_L1 5
#define MOTOR_L2 6
#define MOTOR_R1 9
#define MOTOR_R2 10

#define RELAY_PIN 7
#define SERVO_PIN 3

Servo waterServo;

#define FIRE_DETECTED_THRESHOLD 800

void setup() {
    Serial.begin(9600);

    pinMode(MOTOR_L1, OUTPUT);
    pinMode(MOTOR_L2, OUTPUT);
    pinMode(MOTOR_R1, OUTPUT);
    pinMode(MOTOR_R2, OUTPUT);

    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, HIGH);

    waterServo.attach(SERVO_PIN);
    waterServo.write(90);

    pinMode(FLAME_SENSOR_LEFT, INPUT);
    pinMode(FLAME_SENSOR_RIGHT, INPUT);

    Serial.println("Fire Fighting Robot Initialized");
}

void loop() {
    int flameLeft = analogRead(FLAME_SENSOR_LEFT);
    int flameRight = analogRead(FLAME_SENSOR_RIGHT);

    Serial.print("Flame Left: ");
    Serial.print(flameLeft);
    Serial.print(" | Flame Right: ");
    Serial.println(flameRight);

    if (flameLeft < FIRE_DETECTED_THRESHOLD && flameRight < FIRE_DETECTED_THRESHOLD) {
        moveForward();
        activateWaterPump();
    }
    else if (flameLeft < FIRE_DETECTED_THRESHOLD) {
        turnLeft();
        activateWaterPump();
    }
    else if (flameRight < FIRE_DETECTED_THRESHOLD) {
        turnRight();
        activateWaterPump();
    }
    else {
        stopMotors();
        deactivateWaterPump();
    }

    delay(500);
}

void moveForward() {
    digitalWrite(MOTOR_L1, HIGH);
    digitalWrite(MOTOR_L2, LOW);
    digitalWrite(MOTOR_R1, HIGH);
    digitalWrite(MOTOR_R2, LOW);
    delay(200);
    stopMotors();
}

void turnLeft() {
    digitalWrite(MOTOR_L1, LOW);
    digitalWrite(MOTOR_L2, HIGH);
    digitalWrite(MOTOR_R1, HIGH);
    digitalWrite(MOTOR_R2, LOW);
    delay(200);
    stopMotors();
}

void turnRight() {
    digitalWrite(MOTOR_L1, HIGH);
    digitalWrite(MOTOR_L2, LOW);
    digitalWrite(MOTOR_R1, LOW);
    digitalWrite(MOTOR_R2, HIGH);
    delay(200);
    stopMotors();
}

void stopMotors() {
    digitalWrite(MOTOR_L1, LOW);
    digitalWrite(MOTOR_L2, LOW);
    digitalWrite(MOTOR_R1, LOW);
    digitalWrite(MOTOR_R2, LOW);
}

void activateWaterPump() {
    digitalWrite(RELAY_PIN, LOW);

    for (int pos = 60; pos <= 120; pos += 5) {
        waterServo.write(pos);
        delay(100);
    }
    for (int pos = 120; pos >= 60; pos -= 5) {
        waterServo.write(pos);
        delay(100);
    }

    delay(2000);
    deactivateWaterPump();
}

void deactivateWaterPump() {
    digitalWrite(RELAY_PIN, HIGH);
    waterServo.write(90);
}
