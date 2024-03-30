#include "ntp.h"

void fetch_time() {
    Serial.println("Fetching time from NTP-Server...");

    WiFiUDP ntpUDP;
    NTPClient timeClient{ntpUDP};

    timeClient.begin();
    timeClient.setTimeOffset(TIME_OFFSET);

    int tries = 0;
    while(!timeClient.update()) {
        Serial.println("Failed to fetch time from NTP-Server");
        
        timeClient.forceUpdate();

        if (++tries == MAX_TRIES) {
            break;
        }
    }

    if (tries == MAX_TRIES)
    {
        Serial.println("All attempts failed, using default time");
        //Can't reach NTP-Server, use default
        setTime(DEFAULT_TIMESTAMP);
    } else {//NTP-Server reached
        Serial.println("Time set to NTP-time");
        setTime(timeClient.getEpochTime());
    }
}
