#ifndef NTP_H
#define NTP_H

#include "Arduino.h"

#include <WiFiUdp.h>
#include <NTPClient.h>
#include <TimeLib.h>

const int MAX_TRIES = 1;
const int TIME_OFFSET = 60*60; //60min*60sec
const unsigned int DEFAULT_TIMESTAMP = 441848533; //23:42:13 01.01.1984

void fetch_time();

#endif