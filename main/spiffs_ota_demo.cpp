#include <stdio.h>
#include <string.h>

#include "FileSystem/FileSystem.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_ota_ops.h"
#include "esp_partition.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/task.h"
#include "sdkconfig.h"
static const char *TAG = "spiffs_ota_example";

extern const uint8_t spiffs1_start[] asm("_binary_spiffs1_bin_start");
extern const uint8_t spiffs1_end[] asm("_binary_spiffs1_bin_end");

extern const uint8_t spiffs2_start[] asm("_binary_spiffs2_bin_start");
extern const uint8_t spiffs2_end[] asm("_binary_spiffs2_bin_end");

extern "C" {

void app_main(void) {
  std::string version;
  while (1) {
    load_string("/spiffs/hello.txt", version);
    char label[32];
    // 获取当前的SPIFFS分区
    const esp_partition_t *spiffs_patition = esp_partition_find_first(
        ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_DATA_SPIFFS, NULL);
    if (spiffs_patition != NULL) {
      ESP_LOGI(TAG, "find partition: addr:%x size:%d", spiffs_patition->address,
               spiffs_patition->size);
    } else {
      ESP_LOGE(TAG, "not found otadata");
    }
    ESP_LOGI(TAG, "read file content:%s", version.c_str());
    vTaskDelay(3000 / portTICK_PERIOD_MS);
    ESP_LOGI(TAG, "esp_partition_erase_range");
    esp_err_t error;
    // 根据SPIFFS内储存文件的内容判断出当前用的是哪一个spiffs.bin
    // 然后更新成另外一个spiffs.bin.这里就不优化重复的代码了.
    if (version.c_str()[version.length() - 1] == '1') {
      // 擦除
      error =
          esp_partition_erase_range(spiffs_patition, 0, spiffs_patition->size);
      if (error != ESP_OK) {
        ESP_LOGI(TAG, "esp_partition_erase_range failure %d", error);
      }
      ESP_LOGW(
          TAG,
          "Current is spiffs1.bin.\tWrite spiffs2.bin to spiffs, size is %d",
          spiffs2_end - spiffs2_start - 1);
      // 写入新的bin到SPIFFS分区
      error = esp_partition_write(spiffs_patition, 0, spiffs2_start,
                                  spiffs2_end - spiffs2_start - 1);
      if (error != ESP_OK) {
        ESP_LOGI(TAG, "esp_partition_erase_range failure %d", error);
      } else {
        ESP_LOGI(TAG, "esp_partition_write done.");
      }
    } else if (version.c_str()[version.length() - 1] == '2') {
      // 擦除
      error =
          esp_partition_erase_range(spiffs_patition, 0, spiffs_patition->size);
      if (error != ESP_OK) {
        ESP_LOGI(TAG, "esp_partition_erase_range failure %d", error);
      }
      ESP_LOGW(
          TAG,
          "Current is spiffs2.bin.\tWrite spiffs1.bin to spiffs, size is %d",
          spiffs1_end - spiffs1_start - 1);
      // 写入新的bin到SPIFFS分区
      error = esp_partition_write(spiffs_patition, 0, spiffs1_start,
                                  spiffs1_end - spiffs1_start - 1);
      if (error != ESP_OK) {
        ESP_LOGI(TAG, "esp_partition_erase_range failure %d", error);
      } else {
        ESP_LOGI(TAG, "esp_partition_write done.");
      }
    } else {
      ESP_LOGW(TAG, "Read Failed. Wait For Correct Content.");
    }
    // 重启
    esp_restart();
  }
}
}
