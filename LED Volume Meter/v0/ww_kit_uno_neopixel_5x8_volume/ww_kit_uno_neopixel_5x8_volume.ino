#include <Adafruit_NeoMatrix.h>     // Include the Adafruit NeoMatrix library for controlling the LED matrix
#include <Adafruit_GFX.h>           // Include the Adafruit GFX library, required for graphics functions
#include <Adafruit_NeoPixel.h>      // Include the Adafruit NeoPixel library for handling LED color and brightness

#define MIC_PIN         A0          // Define the pin for the microphone input (A0 on the Arduino)
#define PIN             6           // Define the data pin for the NeoPixel matrix
#define NUMPIXELS       40          // Set the number of LEDs in the NeoPixel matrix (8x5 grid = 40 LEDs)
#define BRIGHTNESS      50          // Set the brightness of the NeoPixels (range: 0 to 255)

// Initialize an 8x5 NeoMatrix grid, using the specified pin, orientation, and color order
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(8, 5, PIN,
  NEO_MATRIX_TOP + NEO_MATRIX_LEFT +             // Start orientation from the top left of the matrix
  NEO_MATRIX_ROWS + NEO_MATRIX_PROGRESSIVE,      // Fill matrix in rows, with a progressive layout
  NEO_GRB + NEO_KHZ800);                         // Set color order to GRB and 800kHz frequency for NeoPixels
  
// Define a color for each row, creating a gradient effect (from blue at the bottom to red at the top)
uint16_t rowColors[] = {
  matrix.Color(0, 0, 255),      // Color for bottom row (blue)
  matrix.Color(0, 128, 255),    // Light blue for the next row up
  matrix.Color(0, 255, 128),    // Cyan for the third row
  matrix.Color(0, 255, 0),      // Green for the fourth row
  matrix.Color(255, 255, 0),    // Yellow for the fifth row
  matrix.Color(255, 128, 0),    // Orange for the sixth row
  matrix.Color(255, 64, 0),     // Orange-red for the seventh row
  matrix.Color(255, 0, 0)       // Red for the top row
};

void setup() {
  matrix.begin();                // Initialize the NeoMatrix
  matrix.setBrightness(BRIGHTNESS); // Set the brightness of the LEDs
  Serial.begin(9600);            // Start serial communication for debugging

  displaySoundLevel(8);          // Light up all rows to show the gradient for 3 seconds
  delay(3000);                   // Keep the display on for 3 seconds

  matrix.fillScreen(0);          // Clear the LED matrix
  matrix.show();                 // Display the cleared matrix
}

void loop() {
  int soundLevel = getSmoothedSoundLevel(); // Get an averaged sound level reading from the microphone

  if (soundLevel < 50) {         // Set a minimum threshold to ignore background noise
    soundLevel = 0;              // If below threshold, treat the sound level as 0
  }

  // Map the sound level (0 to 600) to the number of rows to light up (0 to 8)
  int rowCount = map(soundLevel, 0, 600, 0, 8); // Max level of 600 may need adjusting

  Serial.println(soundLevel);    // Output the sound level to the serial monitor for debugging

  displaySoundLevel(rowCount);   // Light up rows based on the sound level
  delay(50);                     // Short delay to stabilize display
}

// Function to read the sound level and average it for smoothness
int getSmoothedSoundLevel() {
  int total = 0;                 // Initialize the total for averaging
  for (int i = 0; i < 10; i++) { // Take 10 readings for smoothing
    total += analogRead(MIC_PIN); // Read the microphone level and add to total
    delay(1);                    // Small delay between readings
  }
  return total / 10;             // Return average of 10 readings
}

// Function to display the sound level by lighting up rows on the matrix
void displaySoundLevel(int rowCount) {
  matrix.fillScreen(0);          // Clear all LEDs on the matrix

  for (int row = 0; row < rowCount; row++) { // Loop through each row up to rowCount
    lightUpRow(row, rowColors[row]); // Use row-specific colors to create a gradient effect
  }

  matrix.show();                 // Update the display with the new lighting
}

// Helper function to light up a single row with a specified color
void lightUpRow(int row, uint16_t color) {
  for (int col = 0; col < 5; col++) { // Loop through each column in the row (5 columns total)
    matrix.drawPixel(row, col, color); // Set each LED in the row to the specified color
  }
}