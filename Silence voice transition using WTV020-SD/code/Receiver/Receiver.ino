#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "Wtv020sd16p.h"

int resetPin = 4;    // The pin number of the reset pin.
int clockPin = 5;    // The pin number of the clock pin.
int dataPin = 6;     // The pin number of the data pin.
int busyPin = 7;     // The pin number of the busy pin.
int totalAudioFiles = 16; // Change this to the total number of audio files you have

Wtv020sd16p wtv020sd16p(resetPin, clockPin, dataPin, busyPin);
int currentAudioFile = -1;  // Variable to track the currently playing audio file

RF24 radio(9, 10); // CE, CSN

const char addressStr[6] = "00001"; // This address should be the same as the transmitter
byte address[5];

// Define the mapping array
char charMap[] = {'*', '8', '5', '2', '!', '7', '4', '1', '6', '3'};
char desiredChars[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0'};
String lang_select = "";

void setup() {
  Serial.begin(9600);

  // Convert the string address to a byte array
  for (int i = 0; i < 5; i++) {
    address[i] = addressStr[i] - '0';
  }

  radio.begin();
  radio.openReadingPipe(1, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

  wtv020sd16p.reset();
  delay(1000); // Adjust this delay based on the behavior of your module
  lang_select = "e";
  
  while (currentAudioFile == -1) {
    if (radio.available()) {
      char receivedData;
      radio.read(&receivedData, sizeof(receivedData));

      // Find the index of the input character in the mapping array
      int index = findCharIndex(receivedData);


    // Check if the index is valid
    if (index != -1) {
      // Use the index to get the corresponding desired character
      receivedData = desiredChars[index];

       //select english
      if (receivedData == '9' && lang_select == "f"){lang_select = "e";}
      //select french
      else if(receivedData == '0' && lang_select == "e"){lang_select = "f";}
      //play voice note
      else{
        
        Serial.println("Received data: " + lang_select);
     // Button is pressed, change audio file
    if(receivedData - '0' == 9 || receivedData - '0' == 0){}
    else{
    currentAudioFile = lang_select == "f" ? 2*((receivedData - '0') - 1) : 2*((receivedData - '0') - 1) + 1;
    Serial.println(currentAudioFile);
    playAudio(currentAudioFile);
    delay(500);
    }
        }   
    }

      //Serial.println("Received data: " + String(receivedData));
      //currentAudioFile = receivedData =='0' ? 10 : receivedData - '0';
      //playAudio(currentAudioFile);
    }
  }

  
}

void loop() {
  if (radio.available()) {
    char receivedData;
    radio.read(&receivedData, sizeof(receivedData));

    // Find the index of the input character in the mapping array
    int index = findCharIndex(receivedData);

    // Check if the index is valid
    if (index != -1) {
      // Use the index to get the corresponding desired character
      receivedData = desiredChars[index];

       //select english
      if (receivedData == '9' && lang_select == "f"){lang_select = "e";}
      //select french
      else if(receivedData == '0' && lang_select == "e"){lang_select = "f";}
      //play voice note
      else{
        
        Serial.println("Received data: " + lang_select);
     // Button is pressed, change audio file
    if(receivedData - '0' == 9 || receivedData - '0' == 0){}
    else{
    currentAudioFile = lang_select == "f" ? 2*((receivedData - '0') - 1) : 2*((receivedData - '0') - 1) + 1;
    Serial.println(currentAudioFile);
    playAudio(currentAudioFile);
    delay(500);
    }

        }   
    }

    
/*
    playAudio(currentAudioFile);
    delay(500);  // Debounce delay to avoid rapid button presses
    */
  }
  
  
}

void playAudio(int fileNumber) {
  
  // Play audio file synchronously
  wtv020sd16p.playVoice(fileNumber);

  // Wait for the playback to finish (busyPin goes LOW)
  while (digitalRead(busyPin) == HIGH) {
    delay(10);
  }
}

// Function to find the index of a character in the mapping array
int findCharIndex(char inputChar) {
  for (int i = 0; i < 10; i++) {
    if (charMap[i] == inputChar) {
      return i;
    }
  }
  // Return -1 if the character is not found
  return -1;
}
