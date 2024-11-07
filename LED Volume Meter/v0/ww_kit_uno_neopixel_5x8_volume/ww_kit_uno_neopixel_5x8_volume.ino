#include <Adafruit_NeoMatrix.h>
#include <Adafruit_GFX.h>
#include <Adafruit_NeoPixel.h>

#define MIC_PIN         A0       // Microphone output connected to A0
#define PIN             6        // NeoPixel data pin
#define NUMPIXELS       40       // Number of LEDs on the shield
#define BRIGHTNESS      50       // Set NeoPixel brightness

// Initialize NeoMatrix in row-major configuration, starting from the bottom-left
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(8, 5, PIN,
  NEO_MATRIX_TOP + NEO_MATRIX_LEFT + 
  NEO_MATRIX_ROWS + NEO_MATRIX_PROGRESSIVE,
  NEO_GRB + NEO_KHZ800); 
  
// Define colors for each row in the 5x8 grid (from bottom to top) with smoother gradient
uint16_t rowColors[] = {
  matrix.Color(0, 0, 255),      // Bottom row - Blue
  matrix.Color(0, 128, 255),    // Light Blue
  matrix.Color(0, 255, 128),    // Cyan
  matrix.Color(0, 255, 0),      // Green
  matrix.Color(255, 255, 0),    // Yellow
  matrix.Color(255, 128, 0),    // Orange
  matrix.Color(255, 64, 0),     // Orange-Red
  matrix.Color(255, 0, 0)       // Top row - Red
};

void setup() {
  matrix.begin();
  matrix.setBrightness(BRIGHTNESS);
  Serial.begin(9600);
 
  // Display all LEDs for 3 seconds
  displaySoundLevel(8);
  delay(3000); // Wait for 3 seconds

  matrix.fillScreen(0); // Clear the matrix after the 3-second delay
  matrix.show();
}

void loop() {
  int soundLevel = getSmoothedSoundLevel();

  // Set a minimum threshold to ignore low ambient noise
  if (soundLevel < 50) {
    soundLevel = 0;
  }

  // Map sound level to the number of rows to light up (0 to 8 rows)
  int rowCount = map(soundLevel, 0, 600, 0, 8); // Adjust max sound level if needed

  Serial.println(soundLevel); // For debugging

  displaySoundLevel(rowCount);
  delay(50);
}

// Smooth sound level by averaging readings
int getSmoothedSoundLevel() {
  int total = 0;
  for (int i = 0; i < 10; i++) {
    total += analogRead(MIC_PIN);
    delay(1);
  }
  return total / 10;
}

void displaySoundLevel(int rowCount) {
  matrix.fillScreen(0); // Clear the matrix

  // Light up each row based on the sound level, starting from the bottom
  for (int row = 0; row < rowCount; row++) {
    lightUpRow(row, rowColors[row]); // Light up each row in a specific color
  }

  matrix.show();
}

// Function to light up an entire row with a specific color
void lightUpRow(int row, uint16_t color) {
  for (int col = 0; col < 5; col++) { // Each row has 5 LEDs
    matrix.drawPixel(row, col, color); // Light up each pixel in the row
  }
}
