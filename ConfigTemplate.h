 // Config.h
#ifndef CONFIG_H
#define CONFIG_H

// WiFi credentials
const char* const WIFI_SSID = "yourSSID";
const char* const WIFI_PASSWORD = "yourPassword";

// OpenAI API key
const char* const OPENAI_API_KEY = "sk-youropenaikey";

// Debug mode flag
const bool DEBUG_MODE = true; // Set to false to disable verbose logging

// Button and LED pins
#define BUTTON_PIN 0
#define LED_PIN 21
#define NUM_LEDS 1

// Define LED Status Colors
// https://htmlcolorcodes.com/color-names/
CRGB const COLOR_SETUP = CRGB::Orange;
CRGB const COLOR_CONNECTED = CRGB::Green;
CRGB const COLOR_API_RESPONSE = CRGB::Blue;
CRGB const COLOR_API_FAIL = CRGB::Red;
CRGB const COLOR_BUTTON_CLICK = CRGB::Magenta;

#endif // CONFIG_H
