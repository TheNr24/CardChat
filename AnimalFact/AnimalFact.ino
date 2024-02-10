#include <WiFi.h>
#include <FastLED.h>
#include <M5Cardputer.h>
#include "OpenAI.h"
#include "Config.h"

// OpenAI object
OpenAI openai(OPENAI_API_KEY);

void setLEDColor(CRGB color) {
    leds[0] = color;
    FastLED.show();
}

void setup() {
    // Initialize serial communication
    Serial.begin(115200);

    // Initialize the M5Stack object
    M5.begin();
    M5.Display.setRotation(1); // Set display rotation if necessary
    M5.Display.fillScreen(BLACK);
    M5.Display.setTextColor(GREEN);
    M5.Display.setTextSize(2);
    M5.Display.setCursor(0, 0);

    // Initialize button pin and LEDs
    pinMode(BUTTON_PIN, INPUT);
    FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
    setLEDColor(COLOR_SETUP);

    // Connect to WiFi
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi Connected!");
    setLEDColor(COLOR_CONNECTED);
    sendOpenAIRequest();
}

void sendOpenAIRequest() {
    // Make an OpenAI API request for a joke
    OpenAI_Completion completion = openai.completion();
    completion.setModel("gpt-3.5-turbo-instruct");
    completion.setMaxTokens(50);
    OpenAI_StringResponse response = completion.prompt("Tell me a unique fact about an animal in under 20 words.");

    if (response.error()) {
        // Handle API error
        Serial.println("Error:");
        Serial.println(response.error());
        setLEDColor(COLOR_API_FAIL);
        M5.Display.println("API Error");
    } else {
        // Process API response
        M5.Display.clear();
        M5.Display.setCursor(0, 0);
        Serial.println("Response:");
        for (unsigned int i = 0; i < response.length(); i++) {
            Serial.println(response.getAt(i));
            M5.Display.println(response.getAt(i));
        }
        setLEDColor(COLOR_API_RESPONSE);
    }
}

void loop() {
    if (!digitalRead(BUTTON_PIN)) {
        delay(5);
        if (!digitalRead(BUTTON_PIN)) {
            setLEDColor(COLOR_BUTTON_CLICK);
            M5Cardputer.Speaker.tone(8000, 20);
            while (!digitalRead(BUTTON_PIN)) {
            } // Wait for button release
            Serial.println("Button pressed!");
            sendOpenAIRequest();
            delay(500); // Wait a bit before resetting to the connection status color
            setLEDColor(COLOR_CONNECTED);
            M5.update();
        }
    }
}
