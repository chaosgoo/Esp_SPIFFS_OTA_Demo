# ESP32 SPIFFS 分区 OTA 示例

# 前言

大部分资料都是介绍如何在ESP32上为程序进行OTA,而实际上SPIFFS分区也是可以进行OTA的.
为SPIFFS分区OTA比较适用于只想更新资源部分,而不更新运行代码的场景,比如主题里面的图片资源,字体库之类的东西.

## 