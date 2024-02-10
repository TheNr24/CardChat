/*
  CardChat Application
  Written by TheNr24
  This application enables querying OpenAI's GPT model and displaying the response on an M5Cardputer device.
  It includes functionalities for connecting to WiFi, handling user input, and managing display output.
*/

#include <WiFi.h>
#include <FastLED.h>
#include "M5Cardputer.h"
#include "OpenAI.h"
#include "Config.h" // Import configuration settings (WiFi credentials, API keys, etc.)

M5Canvas canvas(&M5Cardputer.Display); // Initialize the canvas for display operations

void setup() {
    USBSerial.begin(115200); // Start serial communication for debugging
    M5.begin(); // Initialize the M5Stack library
    connectToWiFi(); // Connect to WiFi network
    setupDisplay(); // Configure the display settings
}

// Connects the device to a WiFi network
void connectToWiFi() {
    USBSerial.print("Connecting to WiFi...");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD); // Start WiFi connection using credentials from Config.h
    while (WiFi.status() != WL_CONNECTED) { // Wait until connected
        delay(500); // Delay to prevent spamming the connection attempts
        if (DEBUG_MODE) USBSerial.print("."); // Debug output if DEBUG_MODE is enabled
    }
    if (DEBUG_MODE) USBSerial.println(" Connected"); // Confirm connection in debug mode
}

// Sets up the display parameters for the M5Cardputer
void setupDisplay() {
    auto cfg = M5.config(); // Get the M5Cardputer configuration
    M5Cardputer.begin(cfg, true); // Initialize the M5Cardputer with the configuration
    M5Cardputer.Display.setRotation(1); // Set the display rotation
    M5Cardputer.Display.setTextSize(0.5); // Set the text size for the display
    canvas.createSprite(M5Cardputer.Display.width(), M5Cardputer.Display.height()); // Create a sprite for the canvas
    canvas.fillSprite(BLACK); // Fill the canvas with black color
    canvas.setTextSize(1); // Set text size for the canvas
    canvas.setTextFont(&fonts::FreeMonoBold9pt7b); // Set the font for the canvas
    canvas.setTextScroll(true); // Enable text scrolling
    canvas.setTextColor(GREEN); // Set text color to green
    canvas.setCursor(15, 60); // Position the cursor for the initial message
    canvas.println("What is your query?"); // Print the initial query message
    canvas.pushSprite(0, 0); // Display the sprite on the screen
    if (DEBUG_MODE) USBSerial.println("Display setup complete."); // Debug output for display setup completion
}

void loop() {
    static String currentInput = "> "; // Initialize the user input with a prompt
    handleUserInput(currentInput); // Handle any user input
}

// Processes keyboard input and updates the display accordingly
void handleUserInput(String& inputText) {
    M5Cardputer.update(); // Update the state of the M5Cardputer
    if (M5Cardputer.Keyboard.isChange() && M5Cardputer.Keyboard.isPressed()) {
        updateInputText(inputText); // Update the input text based on keyboard input
    }
}

// Updates the input text based on keyboard input
void updateInputText(String& inputText) {
    Keyboard_Class::KeysState status = M5Cardputer.Keyboard.keysState(); // Get the keyboard state
    for (auto i : status.word) { // Iterate through any pressed keys
        if (i) inputText += i; // Append the character to the input text
    }

    if (status.del && inputText.length() > 2) { // Handle deletion of characters
        inputText.remove(inputText.length() - 1);
    }

    if (status.enter) { // If the enter key was pressed
        String query = inputText.substring(2); // Extract the query from the input text
        query.trim(); // Trim any leading or trailing whitespace from the query
        canvas.fillSprite(BLACK); // Clear the canvas for new output
        canvas.setCursor(0, 0); // Reset the cursor position
        canvas.println(inputText); // Display the input text
        queryOpenAIAndDisplayResponse(query); // Query OpenAI and display the response
        inputText = "> "; // Reset the input text for a new query
    } else {
        refreshDisplay(inputText); // Refresh the display with the current input text
    }
}

// Refreshes the display with the current input text
void refreshDisplay(String& inputText) {
    canvas.fillSprite(BLACK); // Clear the canvas
    canvas.setCursor(0, 0); // Reset the cursor position
    canvas.println(inputText); // Display the current input text
    canvas.pushSprite(0, 0); // Update the display with the new content
}

// Queries OpenAI with the user input and displays the response
void queryOpenAIAndDisplayResponse(String userInput) {
    if (DEBUG_MODE) USBSerial.println("Querying OpenAI with user input: " + userInput); // Debug output for the query
    String responseText = fetchResponseFromOpenAI(userInput); // Fetch the response from OpenAI
    displayResponse(responseText); // Display the response text
}

// Fetches a response from OpenAI based on the user's query
String fetchResponseFromOpenAI(String query) {
    OpenAI openai(OPENAI_API_KEY); // Initialize the OpenAI object with the API key
    OpenAI_Completion completion = openai.completion(); // Prepare a new completion request
    completion.setModel("gpt-3.5-turbo-instruct"); // Set the model for the request
    completion.setMaxTokens(50); // Set the maximum number of tokens for the response
    OpenAI_StringResponse response = completion.prompt(query); // Send the query and receive a response

    if (response.error()) { // If there was an error with the request
        if (DEBUG_MODE) USBSerial.println("OpenAI API Error: " + String(response.error())); // Debug output for the error
        return "> API Error. Check Serial."; // Return an error message
    } else {
        String responseText = String(response.getAt(0)); // Convert the response to a String
        responseText.trim(); // Trim any leading or trailing whitespace
        if (DEBUG_MODE) USBSerial.println("Received response from OpenAI: " + responseText); // Debug output for the response
        return responseText; // Return the formatted response text
    }
}

// Displays the response text on the canvas
void displayResponse(String responseText) {
    canvas.setTextColor(BLUE); // Set the text color to blue for the response
    canvas.println("> " + responseText); // Display the response text with a prompt
    canvas.setTextColor(GREEN); // Reset the text color to green for the next input
    canvas.pushSprite(0, 0); // Update the display with the new content
    if (DEBUG_MODE) USBSerial.println("Response displayed: " + responseText); // Debug output for the displayed response
}
