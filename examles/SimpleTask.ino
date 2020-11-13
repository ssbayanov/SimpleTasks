#include <SimpleTasks.h>

SimpleTasks tasks;

#define LED_PIN 13

uint8_t tIDFastBlink;
uint8_t tIDSlowBlink;
uint8_t tIDWithoutCallback;

// Callback reciever for blink led
void blink()
{
    Serial.println("Led change state");
    if(digitalRead(LED_PIN))
    {
        digitalWrite(LED_PIN, LOW);
    }
    else
    {
        digitalWrite(LED_PIN, HIGH);
    }
    
}

// Callback reciever for check state of LED
void checkState()
{
    Serial.print("Led is ");
    if(digitalRead(LED_PIN))
    {
        Serial.println("ON");
    }
    else
    {
        Serial.println("OFF");
    }
}

void setup()
{
    Serial.begin(115200);

    pinMode(LED_PIN, OUTPUT);


    // Add two task for fast and slow blink
    tIDFastBlink = tasks.add(500, blink, 5);
    tIDSlowBlink = tasks.add(2000, blink, 5);

    // Set for each other task callbacks by stops. In this callbacks start other task
    tasks.setStopCallback(tIDFastBlink, [](){ tasks.enable(tIDSlowBlink); });
    tasks.setStopCallback(tIDSlowBlink, [](){ tasks.enable(tIDFastBlink); });

    // If you won't set callback save id and after (in loop) you can check state of this task
    tIDWithoutCallback = tasks.add(30000);

    // if you not need change state of task you can don't save id of task
    tasks.add(500, checkState);
}

void loop()
{
    // Operate tasks
    tasks.run();

    // check state task without callback
    if(tasks.isCame(tIDWithoutCallback))
    {
        Serial.println("Start blink");
        if(tasks.isEnabled(tIDFastBlink) || tasks.isEnabled(tIDSlowBlink))
        {
            tasks.disableWOCallback(tIDFastBlink);
            tasks.disableWOCallback(tIDSlowBlink);
        }
        else
        {
            tasks.enable(tIDFastBlink);
        }        
    }
}