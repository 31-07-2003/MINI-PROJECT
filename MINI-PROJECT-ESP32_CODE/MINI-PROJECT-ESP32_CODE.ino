FINAL CODE OF MINI PROJECT

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define TRIGGER_PIN 13
#define ECHO_PIN 12
#define BUZZER_PIN 14
#define THRESHOLD_DISTANCE 30 // Threshold distance in centimeters

const int xPin = 32; // X-OUT pin of ADXL335 connected to GPIO 32
const int yPin = 35; // Y-OUT pin of ADXL335 connected to GPIO 35
const int zPin = 34; // Z-OUT pin of ADXL335 connected to GPIO 34

void setup() {
  Serial.begin(9600);

  // Initialize ultrasonic sensor pins
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  // Initialize the OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }

  display.display(); // Display initialization
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();
}

void loop() {
  // Ultrasonic sensor and buzzer operation
  long duration, distance;

  // Clear the trigger pin
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);

  // Set the trigger pin HIGH for 10 microseconds
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);

  // Read the echo pin, and calculate distance
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = (duration * 0.0343) / 2; // Speed of sound is 343 m/s (0.0343 cm/microsecond)

  // Print the distance to serial monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Check if the distance is below the threshold
  if (distance < THRESHOLD_DISTANCE) {
    // Calculate on-off cycle duration based on distance
    int cycleDuration = map(distance, 0, THRESHOLD_DISTANCE, 20, 100); // Adjust 20 and 100 for desired range

    // Turn on the buzzer at maximum volume
    digitalWrite(BUZZER_PIN, HIGH);
    delay(cycleDuration); // Keep the buzzer on for the calculated duration
    digitalWrite(BUZZER_PIN, LOW); // Turn off the buzzer
    delay(cycleDuration); // Delay before next cycle
  } else {
    // Turn off the buzzer if distance is above the threshold
    digitalWrite(BUZZER_PIN, LOW);
  }

  // Accelerometer and OLED display operation
  int xAcc = analogRead(xPin);
  int yAcc = analogRead(yPin);
  int zAcc = analogRead(zPin);

  // Convert analog values to G values
  float xG = map(xAcc, 0, 1023, -3, 3) - 7; // Subtracting 7g from X-axis reading
  float yG = map(yAcc, 0, 1023, -3, 3) - 7; // Subtracting 7g from Y-axis reading
  float zG = map(zAcc, 0, 1023, -3, 3) - 7; // Subtracting 7g from Z-axis reading

  // Calculate Euclidean acceleration
  float accel = sqrt(sq(xG) + sq(yG) + sq(zG));

  // Print acceleration values to Serial Monitor
  Serial.print("X-Acceleration: ");
  Serial.print(xG);
  Serial.print(" g, Y-Acceleration: ");
  Serial.print(yG);
  Serial.print(" g, Z-Acceleration: ");
  Serial.print(zG);
  Serial.print(" g, Euclidean Acceleration: ");
  Serial.print(accel);
  Serial.println(" g");

  // Display distance and acceleration values on OLED with increased font size
  display.clearDisplay();
  display.setTextSize(1); // Set text size to 2 for larger font
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Dist: ");
  display.print(distance);
  display.print(" cm");

  display.setCursor(0, 16); // Move to next line
  display.print("X: ");
  display.print(xG);
  display.print(" Y: ");
  display.print(yG);

  display.setCursor(0, 32); // Move to next line
  display.print("Z: ");
  display.print(zG);

  display.setCursor(0, 48); // Move to next line
  display.print("Accel: ");
  display.print(accel);
  display.print(" g");

  display.display();

  delay(100); // Short delay for ultrasonic sensor reading
}