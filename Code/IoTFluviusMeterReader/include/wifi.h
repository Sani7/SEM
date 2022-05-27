#pragma once

static void wifi_event_handler(void *arg, esp_event_base_t event_base,
                               int32_t event_id, void *event_data);
void wifi_init_sta(char *wifi_ssid, char *wifi_pswd);