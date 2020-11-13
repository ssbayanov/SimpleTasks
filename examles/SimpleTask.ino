#include <SimpleTasks.h>

SimpleTasks tasks;

#define LED_PIN 13

uint8_t tIDFastBlink;
uint8_t tIDSlowBlink;
uint8_t tIDWithoutCallback;

// Callback reciever for blink led
void fastBlink()
{
    Serial.println("Led fast blink");
    if(digitalRead(LED_PIN))
    {
        digitalWrite(LED_PIN, LOW);
    }
    else
    {
        digitalWrite(LED_PIN, HIGH);
    }
    
}

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
    tIDFastBlink = tasks.add(500, fastBlink, 10);
    tIDSlowBlink = tasks.add(2000, blink, 10);

    // Set for each other task callbacks by stops. In this callbacks start other task
    tasks.setStopCallback(tIDFastBlink, [](){ tasks.enable(tIDSlowBlink); });
    tasks.setStopCallback(tIDSlowBlink, [](){ tasks.enable(tIDFastBlink); });
    tasks.disableWOCallback(tIDSlowBlink);

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
        if(tasks.isEnabled(tIDFastBlink) || tasks.isEnabled(tIDSlowBlink))
        {
            Serial.println("Stop blink");
            tasks.disableWOCallback(tIDFastBlink);
            tasks.disableWOCallback(tIDSlowBlink);
        }
        else
        {
            Serial.println("Start blink");
            tasks.enable(tIDFastBlink);
        }        
    }
}