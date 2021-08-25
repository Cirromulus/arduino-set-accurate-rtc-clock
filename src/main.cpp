#include <Arduino.h>
#include <Wire.h>
#include <DS1307RTC.h>
#include <Time.h>

static constexpr unsigned max_bufsize = 64;

//necessary, because pow(x,y) returns float and produces rounding errors
uint64_t powerOfTen(unsigned power) {
    uint64_t ret = 1;
    for(uint64_t i = 0; i < power; i++)
        ret *= 10;
    return ret;
}

time_t charToTime(char* array){
    time_t ret = 0;
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
  Serial.begin(115200);
}

void loop() {
    static char timestamp_c[max_bufsize+1];
    uint8_t count = 0;
    while(count < max_bufsize) {
        while(!Serial.available()){};
        timestamp_c[count] = Serial.read();
        if(timestamp_c[count] == '\r')
            continue;
        if(timestamp_c[count] == '\n') {
            timestamp_c[count] = 0;
            break;
        }
        count++;
    }
    time_t new_timestamp = charToTime(timestamp_c);
    if(new_timestamp < 1)
        return;

    if(RTC.set(new_timestamp)) {
        Serial.print("Success: ");
        Serial.println(RTC.get());
    } else {
        Serial.println ("Failed to set RTC");
    }
}
