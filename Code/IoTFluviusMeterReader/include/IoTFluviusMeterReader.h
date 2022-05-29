#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"

#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"

#include "driver/uart.h"
#include "driver/gpio.h"

#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"

#include "esp_http_client.h"
#include "esp_tls.h"
#include "esp_crt_bundle.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* The configuration that you can set via project configuration menu
*/
#define ESP_USE_SD_CARD CONFIG_USE_CRED_SD_CARD
#define ESP_WIFI_SSID CONFIG_ESP_WIFI_SSID
#define ESP_WIFI_PSWD CONFIG_ESP_WIFI_PASSWORD
#define ESP_MAXIMUM_RETRY CONFIG_ESP_MAXIMUM_RETRY

#define ESP_SERVER_IP CONFIG_ESP_SERVER_IP
#define ESP_SERVER_PORT CONFIG_ESP_SERVER_PORT
#define ESP_SERVER_URI CONFIG_ESP_SERVER_URI
#define ESP_SERVER_API_KEY CONFIG_ESP_SERVER_API_KEY

#define RX_BUF_SIZE 1400
#define period CONFIG_ESP_PERIOD

#define MAX_HTTP_RECV_BUFFER 512
#define MAX_HTTP_OUTPUT_BUFFER 1024

#define MOUNT_POINT "/sdcard"
#define SPI_DMA_CHAN    1

/*
 * Pin defines
 */
#define LED_ACT (GPIO_NUM_2)

#define VSPI_CLK (GPIO_NUM_18)
#define VSPI_MISO (GPIO_NUM_19)
#define VSPI_MOSI (GPIO_NUM_23)

#define SD_CS (GPIO_NUM_17)

#define EPD_CS (GPIO_NUM_32)
#define EPD_DC (GPIO_NUM_21)
#define EPD_RST (GPIO_NUM_22)
#define EPD_BUSY (GPIO_NUM_5)

#define ROT_SW (GPIO_NUM_25)
#define ROT_A (GPIO_NUM_26)
#define ROT_B (GPIO_NUM_27)

#define METER_REQ (GPIO_NUM_13)
#define TXD_PIN (GPIO_NUM_15)
#define RXD_PIN (GPIO_NUM_14)

#include "http.h"
#include "sd.h"
#include "uart.h"
#include "wifi.h"

static const char *TAG = "IoT Module";
/* FreeRTOS event group to signal when we are connected*/
static EventGroupHandle_t s_wifi_event_group;

void MeterTask(void* arg);
void configure_pin(gpio_num_t pin, gpio_mode_t mode, gpio_pull_mode_t pull, gpio_int_type_t inter);