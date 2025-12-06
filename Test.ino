#include <Servo.h>

// Define flame sensor pins (only left and right now)
#define FLAME_SENSOR_LEFT A0
#define FLAME_SENSOR_RIGHT A1

// Define motor driver pins
#define MOTOR_L1 5  // Left Motor Forward
#define MOTOR_L2 6  // Left Motor Backward
#define MOTOR_R1 9  // Right Motor Forward
#define MOTOR_R2 10 // Right Motor Backward

// Define relay for water pump (active LOW)
#define RELAY_PIN 7

// Define servo motor pin
#define SERVO_PIN 3

// Create Servo object
Servo waterServo;

// Flame sensor threshold
#define FIRE_DETECTED_THRESHOLD 800  // Adjust based on sensor readings

void setup() {
    // Initialize Serial Monitor
    Serial.begin(9600);

    // Set motor pins as output
    pinMode(MOTOR_L1, OUTPUT);
    pinMode(MOTOR_L2, OUTPUT);
    pinMode(MOTOR_R1, OUTPUT);
    pinMode(MOTOR_R2, OUTPUT);

    // Set relay as output and ensure pump is OFF (active LOW relay)
    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, HIGH);  // HIGH = pump OFF for active LOW relay

    // Initialize servo motor
    waterServo.attach(SERVO_PIN);
    waterServo.write(90); // Set initial servo position to center

    // Set flame sensor pins as input
    pinMode(FLAME_SENSOR_LEFT, INPUT);
    pinMode(FLAME_SENSOR_RIGHT, INPUT);

    Serial.println("🚀 Fire Fighting Robot Initialized (2 sensors)...");
}

void loop() {
    // Read flame sensor values (only left and right)
    int flameLeft = analogRead(FLAME_SENSOR_LEFT);
    int flameRight = analogRead(FLAME_SENSOR_RIGHT);

    // Print sensor values to Serial Monitor
    Serial.print("Flame Left: ");
    Serial.print(flameLeft);
    Serial.print(" | Flame Right: ");
    Serial.println(flameRight);

    // Detection logic:
    // - if both sensors detect fire -> approach (move forward) and spray
    // - else if left detects -> turn left and spray
    // - else if right detects -> turn right and spray
    // - else stop and ensure pump off
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
        deactivateWaterPump();  // Ensure pump is OFF when no fire is detected
    }

    delay(500); // Delay for stability
}

// Function to move forward
void moveForward() {
    digitalWrite(MOTOR_L1, HIGH);
    digitalWrite(MOTOR_L2, LOW);
    digitalWrite(MOTOR_R1, HIGH);
    digitalWrite(MOTOR_R2, LOW);
     delay(200);  // Adjust timing if needed
    stopMotors();
}

// Function to turn left
void turnLeft() {
    digitalWrite(MOTOR_L1, LOW);
    digitalWrite(MOTOR_L2, HIGH);
    digitalWrite(MOTOR_R1, HIGH);
    digitalWrite(MOTOR_R2, LOW);
    delay(200);  // Adjust timing if needed
    stopMotors();
}

// Function to turn right
void turnRight() {
    digitalWrite(MOTOR_L1, HIGH);
    digitalWrite(MOTOR_L2, LOW);
    digitalWrite(MOTOR_R1, LOW);
    digitalWrite(MOTOR_R2, HIGH);
    delay(200);  // Adjust timing if needed
    stopMotors();
}

// Function to stop motors
void stopMotors() {
    digitalWrite(MOTOR_L1, LOW);
    digitalWrite(MOTOR_L2, LOW);
    digitalWrite(MOTOR_R1, LOW);
    digitalWrite(MOTOR_R2, LOW);
}

// Function to activate water pump and move servo
void activateWaterPump() {
    digitalWrite(RELAY_PIN, LOW);  // Turn ON the water pump (active LOW)

    // Sweep the servo from left to right and back
    for (int pos = 60; pos <= 120; pos += 5) {
        waterServo.write(pos);
        delay(100);
    }
    for (int pos = 120; pos >= 60; pos -= 5) {
        waterServo.write(pos);
        delay(100);
    }

    delay(2000);  // Pump stays on for 2 seconds (adjust if needed)
    deactivateWaterPump();  // Turn off the water pump
}

// Function to deactivate water pump
void deactivateWaterPump() {
    digitalWrite(RELAY_PIN, HIGH);  // Turn OFF the water pump (active LOW)
    waterServo.write(90);  // Reset servo to center position
}  