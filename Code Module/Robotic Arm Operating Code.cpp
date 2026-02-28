#include <Servo.h>

// Define the number of servos. It's good practice to use a constant.
const int NUM_SERVOS = 6;

// Use an array to manage all the servo objects. This is much cleaner!
Servo servos[NUM_SERVOS];

// Define the pins for each servo in an array.
// The index of the pin corresponds to the index of the servo.
// e.g., servos[0] is on pin 2, servos[1] is on pin 3, etc.
int servoPins[] = {2, 3, 4, 5, 6, 7};

// Define which servo index corresponds to the gripper.
// This makes the code's intent very clear.
const int GRIPPER_SERVO_INDEX = 4;


void setup() {
  Serial.begin(9600);
  
  // Use a for-loop to attach all the servos.
  // This is much more efficient than attaching them one by one.
  for (int i = 0; i < NUM_SERVOS; i++) {
    servos[i].attach(servoPins[i]);
  }
  
  Serial.println("Improved Robotic Arm Controller Initialized.");
  Serial.println("Ready to receive commands in format: [ServoIndex] [Angle]");
}

void loop() {
  // Check if there is data available to read from the serial port.
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    
    // Find the space that separates the index and the value.
    int spaceIndex = input.indexOf(' ');
    
    // If a space is found, parse the command.
    if (spaceIndex != -1) {
      // Extract the servo index and value.
      int servoIndex = input.substring(0, spaceIndex).toInt();
      int servoValue = input.substring(spaceIndex + 1).toInt();

      // --- Main Control Logic ---
      
      // First, check if the servo index is valid.
      if (servoIndex >= 0 && servoIndex < NUM_SERVOS) {
        
        Serial.print("Moving servo ");
        Serial.print(servoIndex);
        Serial.print(" to ");
        Serial.print(servoValue);
        Serial.println(" degrees.");

        // If the command is for the gripper...
        if (servoIndex == GRIPPER_SERVO_INDEX) {
          // We assume the next servo (index 5) is the other half of the gripper.
          servos[GRIPPER_SERVO_INDEX].write(servoValue); // e.g., servos[4]
          servos[GRIPPER_SERVO_INDEX + 1].write(180 - servoValue); // e.g., servos[5]
        } else {
          // Otherwise, it's a command for a single servo.
          servos[servoIndex].write(servoValue);
        }
        
      } else {
        Serial.println("Error: Invalid servo index.");
      }
    } else {
      Serial.println("Error: Invalid command format. Use: [Index] [Value]");
    }
  }
}
