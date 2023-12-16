#include <NTPClient.h>
#include <WiFiUdp.h>

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

String formattedDate;

void setupTime()
{
    timeClient.begin();
    // Nairobi: GMT+3: 10800 offset
    timeClient.setTimeOffset(10800);
}

String getFileName()
{
    timeClient.update();
    formattedDate = timeClient.getFormattedDate().substring(0, 19);
    formattedDate.replace(":", "-");
    // formattedDate: 2023-08-12T11-33-56

    return "/sss/" + formattedDate + ".jpg";
}