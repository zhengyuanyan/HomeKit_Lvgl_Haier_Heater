#include <Arduino.h>
#include <Wire.h>
#include "./task/task.h"
#include "esp_log.h"
static const char *TAG = "main";
void setup()
{
#if CONFIG_IDF_TARGET_ESP32
  const char *chipModel = "ESP32";
#elif CONFIG_IDF_TARGET_ESP32S2
  const char *chipModel = "ESP32-S2";
#elif CONFIG_IDF_TARGET_ESP32S3
  const char *chipModel = "ESP32-S3";
#elif CONFIG_IDF_TARGET_ESP32C3
  const char *chipModel = "ESP32-C3";
#else
  const char *chipModel = "ESP32";
#endif
  Serial.begin(115200);
  // Serial.setDebugOutput(true);
  delay(1000);

  esp_log_level_set("*", ESP_LOG_INFO);
  ESP_LOGI(TAG, "ESP32 Model: %s", chipModel);
  ESP_LOGI(TAG, "ESP32 ChipRevision: %d", ESP.getChipRevision());
  task_init();
}

void loop()
{
  // put your main code here, to run repeatedly:
}
