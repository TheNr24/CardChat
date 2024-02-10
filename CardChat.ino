#include <WiFi.h>
#include <FastLED.h>
#include "M5Cardputer.h"
#include "OpenAI.h"
#include "Config.h" // Ensure DEBUG_MODE is defined here

M5Canvas canvas(&M5Cardputer.Display);

void setup() {
    Serial.begin(115200);
    M5.begin();
    connectToWiFi();
    setupDisplay();
}

void connectToWiFi() {
    Serial.print("Connecting to WiFi...");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        if (DEBUG_MODE) Serial.print(".");
    }
    if (DEBUG_MODE) Serial.println(" Connected");
}

void setupDisplay() {
    auto cfg = M5.config();
    M5Cardputer.begin(cfg, true);
    M5Cardputer.Display.setRotation(1);
    M5Cardputer.Display.setTextSize(0.5);
    canvas.createSprite(M5Cardputer.Display.width(), M5Cardputer.Display.height());
    canvas.fillSprite(BLACK);
    canvas.setTextSize(1);
    canvas.setTextFont(&fonts::FreeMonoBold9pt7b);
    canvas.setTextScroll(true);
    canvas.setTextColor(GREEN);
    canvas.setCursor(15, 60);
    canvas.println("What is your query?");
    canvas.pushSprite(0, 0);
    if (DEBUG_MODE) Serial.println("Display setup complete.");
}

void loop() {
    static String currentInput = "> ";
    handleUserInput(currentInput);
}

void handleUserInput(String& inputText) {
    M5Cardputer.update();
    if (M5Cardputer.Keyboard.isChange() && M5Cardputer.Keyboard.isPressed()) {
        updateInputText(inputText);
    }
}

void updateInputText(String& inputText) {
    Keyboard_Class::KeysState status = M5Cardputer.Keyboard.keysState();
    for (auto i : status.word) {
        if (i) inputText += i;
    }

    if (status.del && inputText.length() > 2) {
        inputText.remove(inputText.length() - 1);
    }

    if (status.enter) {
        String query = inputText.substring(2);
        query.trim();
        canvas.fillSprite(BLACK);
        canvas.setCursor(0, 0);
        canvas.println(inputText);
        queryOpenAIAndDisplayResponse(query);
        inputText = "> ";
    } else {
        refreshDisplay(inputText);
    }
}

void refreshDisplay(String& inputText) {
    canvas.fillSprite(BLACK);
    canvas.setCursor(0, 0);
    canvas.println(inputText);
    canvas.pushSprite(0, 0);
    // if (DEBUG_MODE) Serial.println("Display refreshed with current input.");
}

void queryOpenAIAndDisplayResponse(String userInput) {
    if (DEBUG_MODE) Serial.println("Querying OpenAI with user input: " + userInput);
    String responseText = fetchResponseFromOpenAI(userInput);
    displayResponse(responseText);
}

String fetchResponseFromOpenAI(String query) {
    OpenAI openai(OPENAI_API_KEY);
    OpenAI_Completion completion = openai.completion();
    completion.setModel("gpt-3.5-turbo-instruct");
    completion.setMaxTokens(50);
    OpenAI_StringResponse response = completion.prompt(query);

    if (response.error()) {
        if (DEBUG_MODE) Serial.println("OpenAI API Error: " + String(response.error()));
        return "> API Error. Check Serial.";
    } else {
        String responseText = String(response.getAt(0)); // Correct conversion to String
        responseText.trim();
        if (DEBUG_MODE) Serial.println("Received response from OpenAI: " + responseText);
        return responseText;
    }
}

void displayResponse(String responseText) {
    canvas.setTextColor(BLUE);
    canvas.println("> " + responseText);
    canvas.setTextColor(GREEN);
    canvas.pushSprite(0, 0);
    if (DEBUG_MODE) Serial.println("Response displayed: " + responseText);
}
