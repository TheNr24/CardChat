# CardChat for M5Cardputer

CardChat connects your M5Stack Card Computer with OpenAI's GPT, offering a straightforward way for anyone to experiment with AI conversations on this compact device.

## Setup

**Requirements:**
- M5Cardputer
- Arduino IDE
- OpenAI API key

**Steps:**
1. Clone the repository:

    ``` git clone https://github.com/your-username/CardChat.git ```

2. Rename `ConfigTemplate.h` to `Config.h` and update it with your WiFi and OpenAI API details.
3. Open `CardChat.ino` in the Arduino IDE, connect your M5Cardputer, and upload the sketch.

## Features

- Engage with OpenAI's GPT model by typing your queries using the CardPuter's keyboard (a Voice to Text feature is in the works).
- Planned features: Scroll through chat history, real-time response streaming, use of latest GPT models, ASR using Whisper, TTS using e.g. ElevenLabs, and settings stored in a config file on the SD card.

## Contribute

Feel free to fork, modify, and make pull requests. For major changes, please open an issue first.

## License

MIT License. Share and enjoy.