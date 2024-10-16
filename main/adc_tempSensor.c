#include <stdio.h>
#include "esp_system.h"
#include "driver/adc.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define ADC_CHANNEL        ADC_CHANNEL_6  // GPIO34 for ADC1
#define ADC_ATTEN          ADC_ATTEN_DB_11
#define ADC_WIDTH          ADC_WIDTH_BIT_12
#define SENSOR_THRESHOLD   100            // Threshold to detect if the sensor is connected

static const char *TAG = "Temperature_Sensor";

void read_temperature_task(void *arg) {
    int adc_reading = 0;
    float temperature = 0.0;

    while (1) {
        // Read ADC value
        adc_reading = adc1_get_raw(ADC_CHANNEL);

        // Check if sensor is mounted
        if (adc_reading < SENSOR_THRESHOLD) {
            ESP_LOGE(TAG, "Sensor not mounted!");
        } else {
            // Example calculation (depends on the sensor's characteristics)
            // For an NTC thermistor or similar sensor, a formula would be needed.
            temperature = (adc_reading * 3.3 / 4095.0) * 100.0; // Example conversion

            ESP_LOGI(TAG, "Temperature: %.2f °C", temperature);
        }

        // Wait for 1 second
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void app_main(void) {
    // Configure ADC
    adc1_config_width(ADC_WIDTH);
    adc1_config_channel_atten(ADC_CHANNEL, ADC_ATTEN);

    // Create temperature reading task
    xTaskCreate(read_temperature_task, "read_temperature_task", 2048, NULL, 5, NULL);
}

