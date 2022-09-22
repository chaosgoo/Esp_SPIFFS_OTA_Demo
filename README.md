# 前言
为SPIFFS分区OTA适用于只想更新资源部分,而不更新运行代码的场景,比如主题里面的图片资源,字体库之类的东西.


## 分区表

````csv
# Name,   Type, SubType, Offset,  Size, Flags
nvs,      data, nvs,     0x9000,  0x5000,
otadata,  data, ota,     0xe000,  0x2000,
app0,     app,  ota_0,   0x10000, 0x300000,
storage,   data, spiffs,  0x310000, 0xC000,
````

这里为了加快OTA速度,我将SPIFFS分区设置的非常小,只有49152字节(48KB)
同时app分区只有app0分区.

## OTA涉及到的函数

````cpp
// 获取当前的SPIFFS分区
const esp_partition_t *spiffs_patition = esp_partition_find_first(
        ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_DATA_SPIFFS, NULL);
// ESP_PARTITION_TYPE_DATA 和分区表内的data对应
// ESP_PARTITION_SUBTYPE_DATA_SPIFFS 对应SPIFFS类型
// 最后一个参数label传递NULL,表示是个SPIFFS分区就行,叫啥都没关系.

// 擦除找到的SPIFFS分区内容.从头开始擦除offset=0, 擦除全部内容size=spiffs_patition->size
esp_partition_erase_range(spiffs_patition, 0, spiffs_patition->size);

// spiffs2_start spiffs2_end是我用到的新的spiffs文件内容.使用embed_file方式嵌入在程序中.
// 写入新的spiffs2的内容到刚才找到的SPIFFS分区,由于我这时候新的spiffs已经是完整的了,所以offset=0,
// 写入尺寸就是此时具有的新spiffs内容的大小size=spiffs2_end - spiffs2_start - 1
esp_partition_write(spiffs_patition, 0, spiffs2_start,
                                  spiffs2_end - spiffs2_start - 1);

// 重启后,SPIFFS内容已经变成刚才新写入的了.
esp_restart();
````