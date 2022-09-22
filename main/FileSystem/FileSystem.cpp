#include "FileSystem.h"

static const char *TAG = "FileSystem";

bool init_spiffs(bool formatIfFailed) {
  ESP_LOGI(TAG, "Initializing SPIFFS");
  esp_vfs_spiffs_conf_t conf = {.base_path = "/spiffs",
                                .partition_label = NULL,
                                .max_files = 5,
                                .format_if_mount_failed = formatIfFailed};
  esp_err_t ret = esp_vfs_spiffs_register(&conf);
  if (ret != ESP_OK) {
    if (ret == ESP_FAIL) {
      ESP_LOGE(TAG, "Failed to mount or format filesystem");
    } else if (ret == ESP_ERR_NOT_FOUND) {
      ESP_LOGE(TAG, "Failed to find SPIFFS partition");
    } else {
      ESP_LOGE(TAG, "Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
    }
    return false;
  }

  size_t total = 0, used = 0;
  ret = esp_spiffs_info(NULL, &total, &used);
  if (ret != ESP_OK) {
    ESP_LOGE(TAG, "Failed to get SPIFFS partition information (%s)",
             esp_err_to_name(ret));
    return false;
  } else {
    ESP_LOGI(TAG, "Partition size: total: %d, used: %d", total, used);
  }
  return true;
}

void deinit_spiffs() {
  // All done, unmount partition and disable SPIFFS
  esp_vfs_spiffs_unregister(NULL);
  ESP_LOGI(TAG, "SPIFFS unmounted");
}

/**
 * @brief 载入文本
 * @param path 文件路径
 **/
void load_string(const char *path, std::string &prgSource) {
  init_spiffs();
  ESP_LOGI(TAG, "load_string %s", path);
  // Open file
  FILE *f = fopen(path, "r");
  if (f == NULL) {
    ESP_LOGE(TAG, "Failed to open %s", path);
    return;
  }

  char buf[64];
  size_t alreadyRead = 0;
  size_t read = 0;
  do {
    memset(buf, 0, sizeof(buf));
    read = fread((void *)buf, 1, sizeof(buf), f);
    prgSource.append(buf);
  } while (read == sizeof(buf));
  fclose(f);
  // Print read content from the file
  ESP_LOGI(TAG, "Read:");
  ESP_LOGW(TAG, "\n%s", prgSource.c_str());
  deinit_spiffs();
}
