#include "IoTFluviusMeterReader.h"

bool readSetting(char *fileContent, int contentLength, const char *settingName, char *returnBuff, int returnLength)
{
  // strncpy(buf, key, l);
  char ContentBuf[contentLength];
  strncpy(ContentBuf, fileContent, sizeof(ContentBuf));
  char *saveptr1;
  char *saveptr2;

  // loop over \n tokens
  char *tok1 = strtok_r(ContentBuf, "\n", &saveptr1);
  while (tok1 != NULL)
  {
    // loop over = token
    char *tok2 = strtok_r(tok1, "=", &saveptr2);
    if (tok2 != NULL)
    {
      if (strcmp(tok2, settingName) == 0)
      {
        tok2 = strtok_r(NULL, "=", &saveptr2);
        if (tok2 != NULL)
        {
          strncpy(returnBuff, tok2, returnLength - 1);
        }
        return 0;
      }
    }

    tok1 = strtok_r(NULL, "\n", &saveptr1);
  }
  return true;
}

bool readFileContents(const char *fileName, char *content, int length)
{
  static const char *TAG = "SD Card";
  esp_err_t ret;

  // Options for mounting the filesystem.
  // If format_if_mount_failed is set to true, SD card will be partitioned and
  // formatted in case when mounting fails.
  esp_vfs_fat_sdmmc_mount_config_t mount_config = {
      .format_if_mount_failed = false,
      .max_files = 5,
      .allocation_unit_size = 16 * 1024};

  sdmmc_card_t *card;
  const char mount_point[] = MOUNT_POINT;
  ESP_LOGI(TAG, "Initializing SD card");

  // Use settings defined above to initialize SD card and mount FAT filesystem.
  // Note: esp_vfs_fat_sdmmc/sdspi_mount is all-in-one convenience functions.
  // Please check its source code and implement error recovery when developing
  // production applications.
  ESP_LOGI(TAG, "Using SPI peripheral");

  sdmmc_host_t host = SDSPI_HOST_DEFAULT();
  spi_bus_config_t bus_cfg = {
      .mosi_io_num = VSPI_MOSI,
      .miso_io_num = VSPI_MISO,
      .sclk_io_num = VSPI_CLK,
      .quadwp_io_num = -1,
      .quadhd_io_num = -1,
      .max_transfer_sz = 4000,
  };
  ret = spi_bus_initialize(host.slot, &bus_cfg, SPI_DMA_CHAN);
  if (ret != ESP_OK)
  {
    ESP_LOGE(TAG, "Failed to initialize bus.");
    return -1;
  }

  // This initializes the slot without card detect (CD) and write protect (WP) signals.
  // Modify slot_config.gpio_cd and slot_config.gpio_wp if your board has these signals.
  sdspi_device_config_t slot_config = SDSPI_DEVICE_CONFIG_DEFAULT();
  slot_config.gpio_cs = SD_CS;
  slot_config.host_id = host.slot;

  ESP_LOGI(TAG, "Mounting filesystem");
  ret = esp_vfs_fat_sdspi_mount(mount_point, &host, &slot_config, &mount_config, &card);

  if (ret != ESP_OK)
  {
    if (ret == ESP_FAIL)
    {
      ESP_LOGE(TAG, "Failed to mount filesystem. "
                    "If you want the card to be formatted, set the EXAMPLE_FORMAT_IF_MOUNT_FAILED menuconfig option.");
    }
    else
    {
      ESP_LOGE(TAG, "Failed to initialize the card (%s). "
                    "Make sure SD card lines have pull-up resistors in place.",
               esp_err_to_name(ret));
    }
    return -1;
  }
  ESP_LOGI(TAG, "Filesystem mounted");

  // Card has been initialized, print its properties
  sdmmc_card_print_info(stdout, card);

  // Use POSIX and C standard library functions to work with files.

  // Open provided file
  char fullPath[50];
  sprintf(fullPath, "%s/%s", mount_point, fileName);
  ESP_LOGI(TAG, "Opening file %s", fullPath);
  FILE *fp = fopen(fullPath, "r");
  if (fp == NULL)
  {
    ESP_LOGE(TAG, "Failed to open file for writing");
    return -2;
  }

  int i = 0;
  char c;
  // first make the array all zeros
  memset(content, 0, length);
  c = fgetc(fp);
  while (c != EOF && i < length - 1)
  {
    content[i] = c;
    i++;
    c = fgetc(fp);
  }

  // done reading content so close the file
  fclose(fp);
  // All done, unmount partition and disable SPI peripheral
  esp_vfs_fat_sdcard_unmount(mount_point, card);
  ESP_LOGI(TAG, "Card unmounted");

  // deinitialize the bus after all devices are removed
  spi_bus_free(host.slot);
  return 0;
}