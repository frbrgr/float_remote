#include <Arduino.h>
#include <ESP8266WiFi.h>

const static int FLOAT_SWITCH_PIN = 1;

unsigned long last_millis = 0;

void set_plug_active(bool on)
{
    WiFiClient c{};
    c.connect({10, 10, 100, 254}, 8080);
    String request{"GET /state?sw=0 HTTP/1.1\r\nConnection: close\r\n\r\n"};
    if (on)
    {
        request.setCharAt(14, '1');
    }
    c.print(request);
}

void setup()
{
    Serial.begin(115200);
    Serial.println();

    pinMode(FLOAT_SWITCH_PIN, INPUT_PULLUP);

    WiFi.begin("HFeasy", "regenfass"); // super secret wifi plug login

    Serial.print("Connecting");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println();
    Serial.print("Connected, IP address: ");
    Serial.println(WiFi.localIP());
}

void loop()
{
    if (last_millis - millis() > 5000) // check every 5 seconds
    {
        if (int float_switch = digitalRead(FLOAT_SWITCH_PIN); float_switch == LOW) // low means full
        {
            set_plug_active(false);
        }
        else
        {
            set_plug_active(true);
        }
        last_millis = millis();
    }
}
