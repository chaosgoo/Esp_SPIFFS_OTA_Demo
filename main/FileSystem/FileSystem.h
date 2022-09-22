#pragma once

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/unistd.h>

#include <string>

#include "esp_err.h"
#include "esp_log.h"
#include "esp_spiffs.h"
#include "ff.h"

bool init_spiffs(bool formatIfFailed = false);

void deinit_spiffs();

error_t format_spiffs();

/**
 * @brief 载入文本数据
 * @param path 文件路径
 **/
void load_string(const char *path, std::string &prgSource);