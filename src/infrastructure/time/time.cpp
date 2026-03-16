#include "time.h"

time_t lastSync = 0;

void timeSynchronize()
{
    time_t now = time(nullptr);
    if ((now - lastSync > 86400) && (WiFi.status() == WL_CONNECTED))
    {
        configTime("CET-1CEST,M3.5.0,M10.5.0", "pool.ntp.org", "time.nist.gov");
        lastSync = time(nullptr);
    }
}

time_t timeSince(time_t startTime)
{
    return time(nullptr) - startTime;
}

String timeToString(time_t time)
{
    struct tm timeInfo;
    localtime_r(&time, &timeInfo);
    char buffer[25];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeInfo);

    return String(buffer);
}
