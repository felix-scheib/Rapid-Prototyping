#include <Arduino.h>

#define GPIO12 12

int prev_state = LOW;
int current_state = LOW;

void setup()
{
  Serial.begin(9600);

  pinMode(GPIO12, INPUT);

  Serial.println("Initialization done!");
}

void loop()
{
  //Button

  current_state = digitalRead(GPIO12);

  if (current_state != prev_state)
  {
    if (current_state == HIGH)
    {
      Serial.println("Button pressed!");
    }

    if (current_state == LOW)
    {
      Serial.println("Button released!");
    }
    
    delay(50);
  }

  prev_state = current_state;
}

