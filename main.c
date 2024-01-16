//same code but using c

//using c

#ifdef __cplusplus
extern "C" {
#endif
  uint8_t temprature_sens_read();
#ifdef __cplusplus
}
#endif



#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/gpio.h"
#include "esp_thermal.h"


// #include "driver/temp_sensor.h"


#define led GPIO_NUM_2

uint8_t temprature_sens_read() {
    // Initialize touch pad peripheral, it will start a timer to run a filter
    ESP_LOGI(TAG, "Initializing Temperature sensor");
    float tsens_out;
    temp_sensor_config_t temp_sensor = TSENS_CONFIG_DEFAULT();
    temp_sensor_get_config(&temp_sensor);
    ESP_LOGI(TAG, "default dac %d, clk_div %d", temp_sensor.dac_offset, temp_sensor.clk_div);
    temp_sensor.dac_offset = TSENS_DAC_DEFAULT;  // DEFAULT: range:-10℃ ~  80℃, error < 1℃.
    temp_sensor_set_config(temp_sensor);
    temp_sensor_start();
    ESP_LOGI(TAG, "Temperature sensor started");
    while (1) {
      vTaskDelay(1000 / portTICK_RATE_MS);
      temp_sensor_read_celsius(&tsens_out);
      ESP_LOGI(TAG, "Temperature out celsius %f°C", tsens_out);
    }
    vTaskDelete(NULL);
  return (uint8_t)tsens_out;
}

volatile bool flag = 0;

// LED pins
const int led = 2;

void TemperatureTask(void* pvParameters) {
  (void)pvParameters;
  Serial.print("TemperatureTask running on core ");
  Serial.println(xPortGetCoreID());
  while (1) {
    uint8_t temprature = ((temprature_sens_read() - 32) / 1.8);
    Serial.print(String(temprature));
    Serial.println(" C");
    flag = 1;
    vTaskDelay(pdMS_TO_TICS(1000));
  }
}

//Task2code: blinks an LED every 700 ms
void BlinkLed(void* pvParameters) {
  (void)pvParameters;
  Serial.print("Task2 running on core ");
  Serial.println(xPortGetCoreID());
gpio_config_t io_config = {
  .pin_bit_mask = (1ULL <<LED_GPIO_PIN),
  .mode = GPIO_MODE_OUTPUT,
};
  gpio_config(&io_conf);
  
  while (1) {
    while (!flag) {
gpio_set_level(led,1);
      vTaskDelay(pdMS_TO_TICS(50));
      gpio_set_level(led,0);
    }
  }
}




void app_main() {

  //create a task that will be executed in the Task1code() function, with priority 1 and executed on core 0
  xTaskCreate(
    TemperatureTask,   /* Task function. */
    "TemperatureTask", /* name of task. */
    10000,             /* Stack size of task */
    NULL,              /* parameter of the task */
    1,                 /* priority of the task */
    NULL,              /* Task handle to keep track of created task */
    0);                /* pin task to core 0 */


  //create a task that will be executed in the Task2code() function, with priority 1 and executed on core 1
  xTaskCreate(
    BlinkLed,   /* Task function. */
    "BlinkLed", /* name of task. */
    10000,      /* Stack size of task */
    NULL,       /* parameter of the task */
    1,          /* priority of the task */
    NULL,       /* Task handle to keep track of created task */
    1);         /* pin task to core 1 */

}

