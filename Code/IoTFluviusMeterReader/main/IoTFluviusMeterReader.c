#include "IoTFluviusMeterReader.h"

static const char *TAG = "IoT Module";

void app_main(void)
{
  ESP_ERROR_CHECK(nvs_flash_init());
  ESP_ERROR_CHECK(esp_netif_init());
  ESP_ERROR_CHECK(esp_event_loop_create_default());

  char wifi_ssid[sizeof(((wifi_config_t *)0)->sta.ssid)] = {0}; // Used to store the SSID for the wifi connection
  char wifi_pswd[sizeof(((wifi_config_t *)0)->sta.password)] = {0};

  http_server_info info;

  uartData uart_data;

  fluviusData data;

  long currentMillis, startMillis;

#if ESP_USE_SD_CARD
  strncpy(wifi_ssid, ESP_WIFI_SSID, sizeof(wifi_ssid));
  strncpy(wifi_pswd, ESP_WIFI_PSWD, sizeof(wifi_pswd));
  strncpy(info.ip, ESP_SERVER_IP, sizeof(info.ip));
  info.port = atoi(ESP_SERVER_PORT);
  strncpy(info.uri, ESP_SERVER_URI, sizeof(info.uri));
  strncpy(info.api_key, ESP_SERVER_API_KEY, sizeof(info.api_key));
#else
char sd_content[200];
readFileContents("config.txt", sd_content, sizeof(sd_content));
  readSetting(sd_content, sizeof(sd_content), "SSID", wifi_ssid, sizeof(wifi_ssid));
  readSetting(sd_content, sizeof(sd_content), "PSWD", wifi_pswd, sizeof(wifi_pswd));
  readSetting(sd_content, sizeof(sd_content), "IP", info.ip, sizeof(info.ip));
  readSetting(sd_content, sizeof(sd_content), "POST", info.uri, sizeof(info.uri));
  readSetting(sd_content, sizeof(sd_content), "APIKey", info.api_key, sizeof(info.api_key));
#endif

  //Pin mode setups for the status led and the meter enable line
  configure_output(LED_ACT);
  configure_output(METER_REQ);

  ESP_LOGI(TAG, "ESP_WIFI_MODE_AP");
  wifi_init_sta(wifi_ssid, wifi_pswd);

  startMillis = esp_timer_get_time() / 1000;
  while (true)
  {
    currentMillis = esp_timer_get_time() / 1000;

    if ((currentMillis - startMillis) >= period)
    {
      switch (uart_data.currentState)
      {
      case IDLE:
         // Request data
        set_output(LED_ACT, false);
        ESP_LOGI(TAG, "Enabling meter");
        set_output(METER_REQ, true);
        uart_data.currentState = READING_DATAGRAM;
        break;

      case READING_DATAGRAM:
        // Read data
        read_datagram(&uart_data);
        break;

      case DATAGRAM_READY:
        // Stop requesting data
        set_output(LED_ACT, true);
        ESP_LOGI(TAG, "We have a DATAGRAM ready for processing");
        set_output(METER_REQ, false);
        uart_data.currentState = PROCESSING_DATA_GRAM;
        break;

      case PROCESSING_DATA_GRAM:
        // Decode data
        if (decode_datagram(uart_data.data, &data))
        {
          uart_data.currentState = DATAGRAM_DECODED;
        }
        else
        {
          // Reset to the IDLE state if datagram is invalid
          uart_data.currentState = IDLE;
        }
        break;

      case DATAGRAM_DECODED:
        // Publish data to MariaDB
        publish_received_data(data, info);

        // Ready for next request
        uart_data.currentState = IDLE;
        set_output(LED_ACT, false);

        // reset timer
        startMillis = currentMillis;
        break;

      default:
        ESP_LOGI(TAG, "Something went wrong on %s:%d", __FILE__,  __LINE__);
        break;
      }
    }
  }
}

void set_output(gpio_num_t pin, bool state)
{
  /* Set the GPIO level according to the state (LOW or HIGH)*/
  gpio_set_level(pin, state);
}

void configure_output(gpio_num_t pin)
{
  ESP_LOGI(TAG, "Example configured to blink GPIO LED!");
  gpio_reset_pin(pin);
  /* Set the GPIO as a push/pull output */
  gpio_set_direction(pin, GPIO_MODE_OUTPUT);
}