#include <Arduino.h>
#include <SPIFFS.h>
#include <EloquentSurveillance.h>
#include "def.h"

EloquentSurveillance::Motion motion;

void setupMotion()
{
    delay(1000);
    camera.aithinker();
    camera.qvga();
    camera.highQuality();
    motion.setMinChanges(0.1);
    motion.setMinPixelDiff(10);
    motion.setMinSizeDiff(0.05);
    pinMode(FLASH_GPIO_NUM, OUTPUT);

    while (!camera.begin())
    {
        Serial.print("*** Camera setup fail: ");
        Serial.println(camera.getErrorMessage());
    }

    Serial.println("................ Camera okay");
    Serial.println("................ Setup done\n\n");

    delay(3000); // I found this to prevent some few first false detections
}

void detectMotion()
{
    if (!camera.capture())
    {
        Serial.print("*** Camera capture fail: ");
        Serial.println(camera.getErrorMessage());
        return;
    }

    if (!motion.update())
        return;

    if (motion.detect())
    {
        digitalWrite(FLASH_GPIO_NUM, HIGH);
        Serial.print("\n----------------- ");
        Serial.print("MOTION DETECTED in: ");
        Serial.println(motion.getExecutionTimeInMicros() + " us");

        if (camera.saveTo(SPIFFS, FILE_PHOTO))
        {
            digitalWrite(FLASH_GPIO_NUM, LOW);
            Serial.println("Saved frame to disk");
            postImage();
        }
        else
        {
            Serial.print("*** Failed to save frame to disk: ");
            Serial.println(camera.getErrorMessage());
        }
        digitalWrite(FLASH_GPIO_NUM, LOW);
    }
    else if (!motion.isOk())
    {
        Serial.print("*** Motion detection fail: ");
        Serial.println(motion.getErrorMessage());
    }
}