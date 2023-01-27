#include <Arduino.h>
#include <Wire.h>
#include <RtcDS1307.h>

RtcDS1307<TwoWire> external_Rtc(Wire);

static constexpr unsigned max_bufsize = 64;

#define countof(a) (sizeof(a) / sizeof(a[0]))

void printDateTime(const RtcDateTime& dt)
{
    char datestring[20];

    snprintf_P(datestring,
            countof(datestring),
            PSTR("%02u.%02u.%04u %02u:%02u:%02u"),
            dt.Day(),
            dt.Month(),
            dt.Year(),
            dt.Hour(),
            dt.Minute(),
            dt.Second() );
    Serial.print(datestring);
}

//necessary, because pow(x,y) returns float and produces rounding errors
uint64_t powerOfTen(unsigned power) {
    uint64_t ret = 1;
    for(uint64_t i = 0; i < power; i++)
        ret *= 10;
    return ret;
}

uint32_t charToTime(char* array){
    uint32_t ret = 0;
    unsigned last = strnlen(array, max_bufsize);
    for(unsigned i = last; i > 0; i--){
        if(array[i-1] < '0' || array[i-1] > '9') {
            Serial.print("Invalid character '");
            Serial.print(array[i-1]);
            Serial.print("' at index ");
            Serial.println(i-1);
            return 0;
        }
        ret += powerOfTen(last - i) * (array[i-1] - '0');
    }
    return ret;
}

void setup() {
    external_Rtc.Begin();
    Serial.begin(115200);
    while (!Serial) {
        digitalWrite(LED_BUILTIN, 1);
        delay(250);
        digitalWrite(LED_BUILTIN, 0);
        delay(25);
    }
}

void loop() {
    static char timestamp_c[max_bufsize+1];
    memset(timestamp_c, 0, max_bufsize+1);
    uint8_t count = 0;
    while(count < max_bufsize) {
        while(!Serial.available()){};
        timestamp_c[count] = Serial.read();
        if(timestamp_c[count] == '\r' || timestamp_c[count] == '\n') {
            timestamp_c[count] = 0;
            break;
        }
        // echo
        Serial.print(timestamp_c[count]);
        count++;
    }
    int32_t new_timestamp = charToTime(timestamp_c);
    if(new_timestamp < 1) {
        Serial.println("Skipping invalid timestamp");
        return;
    }

    // Remove stray chars from serial buffer
    while(Serial.available()){ Serial.read(); }

    external_Rtc.SetDateTime(RtcDateTime(new_timestamp));
    if(external_Rtc.LastError() == 0) {
        Serial.print("Success: ");
        printDateTime(external_Rtc.GetDateTime());
    } else {
        Serial.println ("Failed to set RTC");
    }
}
