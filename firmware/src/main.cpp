/**
 * @file main.cpp
 * @brief Firmware for Clinical Non-Contact IR Thermometer (ESP32-C3)
 * @details Measures body/object temperature using Melexis MLX90614 IR sensor
 *          and displays results on an SSD1306 OLED display with audio alerts.
 * 
 * Target Board: ESP32-C3 Mini
 * Peripherals: MLX90614 (I2C), SSD1306 OLED (I2C), Active Buzzer, Push Button
 * Compliance: EN ISO 80601-2-56 Compliant Measurement Logic
 */

#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <math.h>

// ==========================================
// Hardware Pin & I2C Configuration
// ==========================================
constexpr uint8_t PIN_I2C_SDA       = 8;
constexpr uint8_t PIN_I2C_SCL       = 9;
constexpr uint8_t PIN_BUZZER        = 0;
constexpr uint8_t PIN_BUTTON        = 21;

constexpr uint32_t I2C_CLOCK_SPEED   = 50000; // 50 kHz for stable IR sensor readouts
constexpr uint8_t OLED_I2C_ADDRESS  = 0x3C;

// ==========================================
// Display & Measurement Parameters
// ==========================================
constexpr uint8_t SCREEN_WIDTH      = 128;
constexpr uint8_t SCREEN_HEIGHT     = 64;
constexpr int8_t  OLED_RESET_PIN    = -1;

constexpr float FEVER_THRESHOLD_C   = 37.5f;   // Fever threshold in Celsius
constexpr uint32_t RESULT_HOLD_TIME_MS = 4000; // Display result hold duration (4s)

// ==========================================
// Global Peripheral Instances
// ==========================================
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET_PIN);

// ==========================================
// Function Declarations
// ==========================================
void playStartupAnimation();
void showLoadingSpinner(int durationMs);
void displayResultSlideIn(double objTemp, double ambTemp);
void triggerFeverAlert(double objTemp, double ambTemp);
void updateIdleHeartbeat();
void playButtonBeep();

// ==========================================
// Audio Feedback Helper
// ==========================================

/**
 * @brief Plays a short auditory feedback pulse on button press.
 */
void playButtonBeep() {
    digitalWrite(PIN_BUZZER, HIGH);
    delay(80);
    digitalWrite(PIN_BUZZER, LOW);
}

// ==========================================
// Display UI & Animation Modules
// ==========================================

/**
 * @brief Plays the initial boot-up thermometer filling animation and welcome screen.
 */
void playStartupAnimation() {
    // Fill thermometer tube animation
    for (int h = 0; h <= 36; h += 2) {
        display.clearDisplay();
        display.drawRoundRect(58, 4, 12, 40, 3, SSD1306_WHITE);
        display.fillCircle(64, 50, 8, SSD1306_WHITE);
        if (h > 0) {
            display.fillRect(60, 42 - h, 8, h, SSD1306_WHITE);
        }
        display.setTextSize(1);
        display.setCursor(2, 20);
        display.print("G1-MIY");
        display.setCursor(2, 34);
        display.print("Thermometer");
        display.display();
        delay(35);
    }
    delay(200);

    // Full thermometer display pulse
    display.clearDisplay();
    display.fillCircle(64, 50, 8, SSD1306_WHITE);
    display.fillRect(60, 6, 8, 36, SSD1306_WHITE);
    display.drawRoundRect(58, 4, 12, 40, 3, SSD1306_WHITE);
    display.display();
    delay(300);

    // Welcome text slide-in animation
    for (int x = 128; x >= 10; x -= 10) {
        display.clearDisplay();
        display.setTextSize(2);
        display.setCursor(x, 18);
        display.print("WELCOME");
        display.setTextSize(1);
        display.setCursor(x + 5, 42);
        display.print("G1-MIY Thermometer");
        display.display();
        delay(20);
    }
    delay(1500);
}

/**
 * @brief Renders a rotating loading spinner while temperature is acquired.
 * @param durationMs Duration of the spinner animation in milliseconds.
 */
void showLoadingSpinner(int durationMs) {
    const int cx = 64, cy = 28, r = 16;
    int steps = durationMs / 70;

    for (int s = 0; s < steps; s++) {
        display.clearDisplay();
        display.setTextSize(1);
        display.setCursor(22, 52);
        display.print("Measuring...");

        for (int i = 0; i < 8; i++) {
            float angle = radians(((i + s) % 8) * 45.0);
            int px = cx + r * cos(angle);
            int py = cy + r * sin(angle);

            if (i == 7) {
                display.fillCircle(px, py, 3, SSD1306_WHITE);
            } else if (i >= 5) {
                display.fillCircle(px, py, 2, SSD1306_WHITE);
            } else {
                display.drawPixel(px, py, SSD1306_WHITE);
            }
        }
        display.display();
        delay(10);
    }
}

/**
 * @brief Smoothly slides temperature measurement results onto the OLED screen.
 * @param objTemp Target object / body temperature in Celsius.
 * @param ambTemp Ambient environment temperature in Celsius.
 */
void displayResultSlideIn(double objTemp, double ambTemp) {
    for (int offset = 128; offset >= 0; offset -= 14) {
        display.clearDisplay();
        display.setTextSize(1);
        display.setCursor(offset, 0);
        display.print("Environment: ");
        display.print(ambTemp, 1);
        display.print("C");
        display.drawLine(offset, 12, offset + 128, 12, SSD1306_WHITE);

        display.setTextSize(3);
        display.setCursor(offset + 5, 18);
        display.print(objTemp, 1);
        
        display.setTextSize(2);
        display.setCursor(offset + 92, 18);
        display.print((char)247); // Degree symbol (°C)
        display.print("C");

        display.setTextSize(1);
        if (objTemp >= FEVER_THRESHOLD_C) {
            display.setCursor(offset + 28, 52);
            display.print("!  FEVER  !");
        } else {
            display.setCursor(offset + 40, 52);
            display.print("Normal");
        }
        display.display();
        delay(12);
    }
}

/**
 * @brief Triggers acoustic and visual fever warning (4 distinct beeps).
 * @param objTemp Target object / body temperature in Celsius.
 * @param ambTemp Ambient environment temperature in Celsius.
 */
void triggerFeverAlert(double objTemp, double ambTemp) {
    // 4-cycle auditory & visual alert (~1 second total duration)
    for (int i = 0; i < 4; i++) {
        display.clearDisplay();
        display.setTextSize(1);
        display.setCursor(0, 0);
        display.print("Environment: ");
        display.print(ambTemp, 1);
        display.print("C");
        display.drawLine(0, 12, 128, 12, SSD1306_WHITE);

        display.setTextSize(3);
        display.setCursor(5, 18);
        display.print(objTemp, 1);
        
        display.setTextSize(2);
        display.setCursor(92, 18);
        display.print((char)247);
        display.print("C");

        display.setTextSize(1);
        display.setCursor(28, 52);
        display.print("!  FEVER  !");
        display.display();

        digitalWrite(PIN_BUZZER, HIGH);
        delay(100);
        digitalWrite(PIN_BUZZER, LOW);
        delay(150);
    }
}

/**
 * @brief Renders the non-blocking heartbeat animation on the standby idle screen.
 */
void updateIdleHeartbeat() {
    static unsigned long lastFrame = 0;
    static int frame = 0;
    static int dir = 1;

    if (millis() - lastFrame < 55) return;
    lastFrame = millis();

    frame += dir;
    if (frame >= 7 || frame <= 0) {
        dir = -dir;
    }

    display.clearDisplay();

    // Heart icon rendering
    int hx = 55, hy = 8;
    int r = (frame > 3) ? 6 : 4;
    display.fillCircle(hx, hy, r, SSD1306_WHITE);
    display.fillCircle(hx + r * 2, hy, r, SSD1306_WHITE);
    display.fillTriangle(hx - r, hy + r / 2,
                         hx + r * 3 + r, hy + r / 2,
                         hx + r, hy + r * 2 + 2, SSD1306_WHITE);

    display.setTextSize(1);
    display.setCursor(8, 32);
    display.print("Press the button");
    display.setCursor(22, 46);
    display.print("to measure");

    // Dynamic loading dots
    for (int d = 0; d < 3; d++) {
        if (frame > d * 2) {
            display.fillCircle(50 + d * 10, 58, 2, SSD1306_WHITE);
        }
    }
    display.display();
}

// ==========================================
// Setup & Initialization
// ==========================================
void setup() {
    Serial.begin(115200);
    delay(1000);

    // Pin mode initialization
    pinMode(PIN_BUTTON, INPUT_PULLUP);
    pinMode(PIN_BUZZER, OUTPUT);
    pinMode(PIN_I2C_SDA, INPUT_PULLUP);
    pinMode(PIN_I2C_SCL, INPUT_PULLUP);
    delay(100);

    // Initialize I2C bus with custom speed
    Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL);
    Wire.setClock(I2C_CLOCK_SPEED);
    delay(100);

    // Initialize OLED Display
    if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADDRESS)) {
        Serial.println("[ERROR] OLED Display initialization failed!");
        while (true);
    }
    display.setTextColor(SSD1306_WHITE);

    // Play bootup sequence
    playStartupAnimation();

    // Initialize MLX90614 Sensor
    if (!mlx.begin(MLX90614_I2CADDR, &Wire)) {
        Serial.println("[ERROR] MLX90614 IR Sensor initialization failed!");
        display.clearDisplay();
        display.setCursor(10, 25);
        display.print("SENSOR ERROR!");
        display.display();
        while (true);
    }

    Serial.println("[INFO] Thermometer System Ready!");
}

// ==========================================
// Main Application Loop
// ==========================================
void loop() {
    // Render idle heartbeat animation when waiting
    updateIdleHeartbeat();

    // Check measurement trigger button (active LOW)
    if (digitalRead(PIN_BUTTON) == LOW) {
        delay(50); // Debounce delay
        if (digitalRead(PIN_BUTTON) == LOW) {

            // 1. Audio feedback for button press
            playButtonBeep();

            // 2. Display acquisition spinner
            showLoadingSpinner(1500);

            // Read temperatures
            double objTemp = mlx.readObjectTempC();
            double ambTemp = mlx.readAmbientTempC();

            Serial.print("[LOG] Object: "); Serial.print(objTemp);
            Serial.print(" °C | Environment: "); Serial.print(ambTemp); Serial.println(" °C");

            if (!isnan(objTemp)) {
                // 3. Display result slide-in animation
                displayResultSlideIn(objTemp, ambTemp);

                // 4. Trigger fever warning if above threshold
                if (objTemp >= FEVER_THRESHOLD_C) {
                    delay(500);
                    triggerFeverAlert(objTemp, ambTemp);
                }
            }

            // Wait until button release
            while (digitalRead(PIN_BUTTON) == LOW);

            // Hold result on display for 4 seconds (interruptible by new press)
            unsigned long holdStart = millis();
            while (millis() - holdStart < RESULT_HOLD_TIME_MS) {
                if (digitalRead(PIN_BUTTON) == LOW) {
                    break;
                }
                delay(10);
            }
            delay(50);
        }
    }
}
