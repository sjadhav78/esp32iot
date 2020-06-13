/* Hello World Example
   Dual Core Thread Execution
   Satish
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "driver/gpio.h"

#define BLINK_GPIO 2

// Create tasks
TaskHandle_t Task1;
TaskHandle_t Task2;

void func1()
{
    while(1)
    {
        printf("Displaying ESP Chip Information...\n");

        esp_chip_info_t chip_info;
        esp_chip_info(&chip_info);
        printf("This is ESP32 chip with %d CPU cores, WiFi%s%s, ", 
            chip_info.cores, 
            (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "", 
            (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");
        printf("silicon revision %d, ", chip_info.revision);
        printf("%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024), 
            (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
    
    /*for (int i = 10; i >= 0; i--) 
    {
        printf("Restarting in %d seconds...\n", i);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }

    fflush(stdout);
    printf("Restarting now.\n");
    esp_restart();
    */
}

void func2()
{
    // Blinking example
    gpio_pad_select_gpio(BLINK_GPIO);
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);

    while(1) 
    {
	    printf("Turning off the LED...\n");
        gpio_set_level(BLINK_GPIO, 0);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
	    printf("Turning on the LED...\n");
        gpio_set_level(BLINK_GPIO, 1);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void app_main()
{
    printf("Hello World....!\n");

    xTaskCreatePinnedToCore(func1, "Task1", 10000, NULL, 1, &Task1, 0);
    xTaskCreatePinnedToCore(func2, "Task2", 10000, NULL, 1, &Task2, 1);
}