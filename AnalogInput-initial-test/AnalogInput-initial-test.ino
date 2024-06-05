// constants won't change. Used here to set a pin number:
const int ledPin1 = PA5;  // the number of the LED pin
const int ledPin2 = PA6;  // the number of the LED pin
const int ledPin3 = PA7;  // the number of the LED pin
const int ledPin4 = PA8;  // the number of the LED pin
const int ledPin5 = PB6;  // the number of the LED pin
const int ledPin6 = PB7;  // the number of the LED pin

// Variables will change:
int ledState = LOW;  // ledState used to set the LED

// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0;  // will store last time LED was updated

// constants won't change:
const long interval = 300;  // interval at which to blink (milliseconds)

void setup() {
  // set the digital pin as output:
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  pinMode(ledPin4, OUTPUT);
  pinMode(ledPin5, OUTPUT);
  pinMode(ledPin6, OUTPUT);
}

void loop() {
 
    digitalWrite(ledPin6, LOW);
    digitalWrite(ledPin1, HIGH);
    delay(1000);
    digitalWrite(ledPin1, LOW);
    digitalWrite(ledPin2, HIGH);
    delay(1000);
    digitalWrite(ledPin2, LOW);
    digitalWrite(ledPin3, HIGH);
    delay(1000);
    digitalWrite(ledPin3, LOW);
    digitalWrite(ledPin4, HIGH);
    delay(1000);
    digitalWrite(ledPin4, LOW);
    digitalWrite(ledPin5, HIGH);
    delay(1000);
    digitalWrite(ledPin5, LOW);
    digitalWrite(ledPin6, HIGH);
    delay(1000);
}