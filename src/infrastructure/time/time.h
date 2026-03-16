#pragma once
#include <ESP8266WiFi.h>
#include <time.h>

void timeSynchronize();
time_t timeSince(time_t startTime);
String timeToString(time_t time);