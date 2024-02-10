// Config.h
#ifndef CONFIG_H
#define CONFIG_H

// WiFi credentials
const char* const WIFI_SSID = "";
const char* const WIFI_PASSWORD = "";

// OpenAI API key
const char* const OPENAI_API_KEY = "sk-your-api-key";

// Button and LED pins
#define BUTTON_PIN 0
#define LED_PIN 21
#define NUM_LEDS 1

// Define LED Status Colors
CRGB leds[NUM_LEDS];
CRGB const COLOR_SETUP = CRGB::Orange;
CRGB const COLOR_CONNECTED = CRGB::Green;
CRGB const COLOR_API_RESPONSE = CRGB::Blue;
CRGB const COLOR_API_FAIL = CRGB::Red;
CRGB const COLOR_BUTTON_CLICK = CRGB::Magenta;

#endif // CONFIG_H
