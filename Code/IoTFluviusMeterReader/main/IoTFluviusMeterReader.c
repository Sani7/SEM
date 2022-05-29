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

  uart_init();

  ESP_LOGI(TAG, "ESP_WIFI_MODE_AP");
  wifi_init_sta(wifi_ssid, wifi_pswd);
  ESP_LOGI(TAG, "Connected");
  
  xTaskCreate(MeterTask, "MeterTask", 1024*8, NULL, 5, NULL);
}

void MeterTask(void* arg)
{
  long startMillis;
  char uartData[RX_BUF_SIZE] = {0};

  fluviusData data;

  while (true)
  {
    startMillis = esp_timer_get_time();
    memset(uartData, 0, RX_BUF_SIZE);
    // Request data
    ESP_ERROR_CHECK(gpio_set_level(LED_ACT, false));
    ESP_LOGI(TAG, "Enabling meter");
    ESP_ERROR_CHECK(gpio_set_level(METER_REQ, true));

    ESP_LOGI(TAG, "Reading Data");
    // Read data
    if (read_datagram(uartData))
    {
      ESP_LOGI(TAG, "Bad datagram read");
      gpio_set_level(METER_REQ, false);
      continue;
    }

    // Stop requesting data
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
    ESP_LOGI("DEBUG", "%s %d %s", info.ip, info.port, info.uri);
    publish_received_data(data, info);

    // Ready for next request
    gpio_set_level(LED_ACT, false);

    while (esp_timer_get_time() < (startMillis + period * 1000))
    {
      vTaskDelay(1);
    }
  }
}

void configure_pin(gpio_num_t pin, gpio_mode_t mode, gpio_pull_mode_t pull, gpio_int_type_t inter)
{
  ESP_LOGI(TAG, "GPIO[%d] configured as an %s", pin, mode < 2 ? "imput" : "output");
  gpio_reset_pin(pin);
  gpio_set_direction(pin, mode);
  gpio_set_pull_mode(pin, pull);
  gpio_set_intr_type(pin, inter);
}