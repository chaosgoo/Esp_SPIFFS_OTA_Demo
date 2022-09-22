idf_component_register(SRCS "spiffs_ota_demo.cpp"
                        "FileSystem/FileSystem.cpp"
                    INCLUDE_DIRS "."
                    EMBED_TXTFILES 
                        "spiffs1.bin"
                        "spiffs2.bin"
                    )
spiffs_create_partition_image(storage ../data FLASH_IN_PROJECT) 
