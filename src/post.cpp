#include <Arduino.h>
#include "WiFi.h"
#include "esp_camera.h"
#include "soc/soc.h"          // Disable brownout problems
#include "soc/rtc_cntl_reg.h" // Disable brownout problems
#include "driver/rtc_io.h"
#include <SPIFFS.h>
#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>
#include "def.h"

// Replace with your network credentials
const char *ssid = "Hero";
const char *password = "heroinex";

// Define Firebase Data objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

void initWiFi()
{
    int retries = 0;
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        if (retries > MAX_WIFI_RECCONECTS)
        {
            Serial.println("\n*** RESTARTING: Max WiFi reconnections [5]\n");
            ESP.restart();
        }
        retries++;
        delay(1000);
        Serial.println("................ Connecting to WiFi...");
    }
    Serial.println("................ Yaay!! Connected to WiFi");
}

void initSPIFFS()
{
    if (!SPIFFS.begin(true))
    {
        Serial.println("\n*** RESTARTING: Failed to mount SPIFFS\n");
        ESP.restart();
    }
    else
    {
        delay(500);
        Serial.println("................ SPIFFS mounted successfully");
    }
}

void setupFirebase()
{
    Serial.println("\n................ Start setup");

    initWiFi();
    initSPIFFS();
    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); // Turn-off the 'brownout detector'
    config.api_key = API_KEY;
    auth.user.email = USER_EMAIL;
    auth.user.password = USER_PASSWORD;
    config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h
    Firebase.begin(&config, &auth);
    Firebase.reconnectWiFi(true);
}

void postImage()
{
    String filename = getFileName();
    Serial.print("Uploading picture: ");
    Serial.println(filename);

    if (Firebase.ready())
    {
        if (Firebase.Storage.upload(&fbdo,
                                    STORAGE_BUCKET_ID /* Firebase Storage bucket id */,
                                    FILE_PHOTO /* path to local file */,
                                    mem_storage_type_flash /* memory storage type, mem_storage_type_flash and mem_storage_type_sd */,
                                    filename /* path of remote file stored in the bucket */, "image/jpeg" /* mime type */))
        {
            Serial.printf("Download URL: %s\n", fbdo.downloadURL().c_str());
        }
        else
        {
            Serial.println(fbdo.errorReason());
        }
    }
}