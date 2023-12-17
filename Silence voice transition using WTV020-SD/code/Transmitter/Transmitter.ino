/* @file CustomKeypad.pde
|| @version 1.0
|| @author Alexander Brevig
|| @contact alexanderbrevig@gmail.com
||
|| @description
|| | Demonstrates changing the keypad size and key values.
|| #
*/
#include <Keypad.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10); // CE, CSN

const char addressStr[6] = "00001"; // This address should be the same on the receiver

const byte ROWS = 4; // four rows
const byte COLS = 3; // four columns
// define the symbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'!', '*', '#'},
};
byte rowPins[ROWS] = {2, 3, 4, 5}; // connect to the row pinouts of the keypad
byte colPins[COLS] = {6, 7, 8};    // connect to the column pinouts of the keypad

// initialize an instance of the class Keypad
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

void setup()
{
  Serial.begin(9600);
  // Set pins to INPUT_PULLUP if they are used for columns in the keypad
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);

  // Convert the string address to a byte array
  byte address[5];
  for (int i = 0; i < 5; i++)
  {
    address[i] = addressStr[i] - '0';
  }

  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}

void loop()
{
  char customKey = customKeypad.getKey();

  if (customKey)
  {
    // Serial.println(customKey);
    // Send the data over the radio
    radio.write(&customKey, sizeof(customKey));
  }
}
