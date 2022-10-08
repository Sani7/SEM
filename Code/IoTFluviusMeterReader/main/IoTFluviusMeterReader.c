#include "IoTFluviusMeterReader.h"

http_server_info info;

void app_main(void)
{
  ESP_ERROR_CHECK(nvs_flash_init());

  char wifi_ssid[sizeof(((wifi_config_t *)0)->sta.ssid)] = {0}; // Used to store the SSID for the wifi connection
  char wifi_pswd[sizeof(((wifi_config_t *)0)->sta.password)] = {0};

#if ESP_USE_SD_CARD == 0
  ESP_LOGI(TAG, "Using config from menuconfig");
  strncpy(wifi_ssid, ESP_WIFI_SSID, sizeof(wifi_ssid));
  strncpy(wifi_pswd, ESP_WIFI_PSWD, sizeof(wifi_pswd));
  strncpy(info.ip, ESP_SERVER_IP, sizeof(info.ip));
  info.port = atoi(ESP_SERVER_PORT);
  strncpy(info.uri, ESP_SERVER_URI, sizeof(info.uri));
  strncpy(info.api_key, ESP_SERVER_API_KEY, sizeof(info.api_key));
#else
  ESP_LOGI(TAG, "Using config from SD card");
  char sd_content[200];
  readFileContents("config.txt", sd_content, sizeof(sd_content));
  readSetting(sd_content, sizeof(sd_content), "SSID", wifi_ssid, sizeof(wifi_ssid));
  readSetting(sd_content, sizeof(sd_content), "PSWD", wifi_pswd, sizeof(wifi_pswd));
  readSetting(sd_content, sizeof(sd_content), "IP", info.ip, sizeof(info.ip));
  readSetting(sd_content, sizeof(sd_content), "POST", info.uri, sizeof(info.uri));
  readSetting(sd_content, sizeof(sd_content), "APIKey", info.api_key, sizeof(info.api_key));
#endif

  // Pin mode setups for the status led and the meter enable line
  configure_pin(LED_ACT, GPIO_MODE_OUTPUT, GPIO_FLOATING, GPIO_INTR_DISABLE);
  configure_pin(METER_REQ, GPIO_MODE_OUTPUT, GPIO_FLOATING, GPIO_INTR_DISABLE);
  gpio_set_level(LED_ACT, false);
  gpio_set_level(METER_REQ, false);

  uart_init(TXD_PIN, RXD_PIN, 115200, true);

  ESP_LOGI(TAG, "ESP_WIFI_MODE_AP");
  wifi_init_sta(wifi_ssid, wifi_pswd);
  ESP_LOGI(TAG, "Connected");

  xTaskCreate(MeterTask, "MeterTask", 1024 * 8, NULL, 5, NULL);
}

/**
 * @brief Main task read UART, decode and Post
 *
 * @param arg no arguments necessary
 */
void MeterTask(void *arg)
{
  int64_t startMicros, endMicros;
  char uartData[RX_BUF_SIZE] = {0};

  fluviusData data;

  while (true)
  {
    startMicros = esp_timer_get_time();
    memset(uartData, 0, RX_BUF_SIZE);
    // Request data
    ESP_ERROR_CHECK(gpio_set_level(LED_ACT, false));
    ESP_LOGI(TAG, "Enabling meter");
    ESP_ERROR_CHECK(gpio_set_level(METER_REQ, true));

    ESP_LOGI(TAG, "Reading Data");
    // Read data
    if (read_datagram(uartData, sizeof(uartData)))
    {
      ESP_LOGI(TAG, "Bad datagram read");
      continue;
    }

    // Stop requesting data
    ESP_LOGI(TAG, "Disabling Meter");
    gpio_set_level(METER_REQ, false);
    gpio_set_level(LED_ACT, true);
    ESP_LOGI(TAG, "We have a DATAGRAM ready for processing");

    // Decode data
    if (decode_datagram(uartData, &data))
    {
      ESP_LOGI(TAG, "Bad datagram decoded");
      continue;
    }

    // Publish data to MariaDB

    // Prepare the data
    char post_data[500]; // The request will be build in this String
    sprintf(post_data, "api_key=%s&UL1=%.1f&UL2=%.1f&UL3=%.1f&IL1=%.1f&IL2=%.1f&IL3=%.1f&Pcl=%.3f&Pch=%.3f&Ppl=%.3f&Pph=%.3f&Pc=%.3f&Pp=%.3f&Vg=%.3f&Vw=%.3f",
            info.api_key, data.UL1, data.UL2, data.UL3, data.IL1, data.IL2,
            data.IL3, data.CONSUMPTION_LOW_TARIF,
            data.CONSUMPTION_HIGH_TARIF, data.PRODUCTION_HIGH_TARIF,
            data.PRODUCTION_LOW_TARIF, data.TOTAL_POWER_CONSUMPTION,
            data.TOTAL_POWER_PRODUCTION, data.GAS_METER_M3, data.WATER_METER_M3);

    // Post
    HTTP_Post(info, post_data);

    // Ready for next request
    gpio_set_level(LED_ACT, false);

    // Delay for period
    do
    {
      if (esp_timer_get_time() > startMicros)
        endMicros = esp_timer_get_time() - startMicros;

      else
        endMicros = esp_timer_get_time() + startMicros;
      vTaskDelay(1);
    } while (endMicros < period * 1000);
    ESP_LOGI("Timer", "%lld", endMicros);
    // vTaskDelay(period / portTICK_PERIOD_MS);
  }
}

/**
 * @brief setup a gpio pin
 *
 * @param pin which pin? (number)
 * @param mode Input, Output, Input/Output
 * @param pull Pull-up/ Pull-down/ Floating
 * @param inter Interrupt on this pin?
 */
void configure_pin(gpio_num_t pin, gpio_mode_t mode, gpio_pull_mode_t pull, gpio_int_type_t inter)
{
  ESP_LOGI(TAG, "GPIO[%d] configured as an %s", pin, mode < 2 ? "imput" : "output");
  gpio_reset_pin(pin);
  gpio_set_direction(pin, mode);
  gpio_set_pull_mode(pin, pull);
  gpio_set_intr_type(pin, inter);
}