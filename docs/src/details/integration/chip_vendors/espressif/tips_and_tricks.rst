Improving LVGL speed of execution
---------------------------------

Configuring the PSRAM on ESP32 supported devices
------------------------------------------------


Using the File System under ESP-IDF
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ESP-IDF uses the standard C file operation functions (``fopen``, ``fread``) in all its storage related APIs.
This allows seamless interoperability with LVGL when enabling the :c:macro:`LV_USE_FS_STDIO` configuration.
The process is described in details below, using ``SPIFFS`` as demonstration.

- **Decide what storage system you want to use**

   ESP-IDF has many, ready-to-use examples like
   `SPIFFS <https://github.com/espressif/esp-idf/tree/master/examples/storage/spiffsgen>`__
   ,
   `SD Card <https://github.com/espressif/esp-idf/tree/master/examples/storage/sd_card/sdspi>`__
   and
   `LittleFS <https://github.com/espressif/esp-idf/tree/master/examples/storage/littlefs>`__
   .

- **Re-configure your own project**

   The example project should be examined for details, but in general the changes involve:

   - Enabling LVGL's STDIO file system in the configuration

      You can use ``menuconfig``:

         - ``Component config → LVGL configuration → 3rd Party Libraries``: enable ``File system on top of stdio API``
         - Then select ``Set an upper cased letter on which the drive will accessible`` and set it to ``65`` (ASCII **A**)
         - You can also set ``Default driver letter`` to 65 to skip the prefix in file paths.

   - Modifying the partition table

      The exact configuration depends on your flash size and existing partitions,
      but the new final result should look something like this:

      .. csv-table:: Partition Table

         nvs,      data, nvs,     0x9000,  0x6000
         phy_init, data, phy,     0xf000,  0x1000
         factory,  app,  factory, 0x10000, 1400k
         storage,  data, spiffs,         ,  400k


      .. note::

         If you are not using a custom ``partition.csv`` yet, it can be added
         via ``menuconfig`` (``Partition Table → Partition Table → Custom partition table CSV``).

   - Apply changes to the build system

      Some ESP file systems provide automatic generation from a host folder using CMake. The proper line(s) must be copied to ``main/CMakeLists.txt``

      .. note::

         ``LittleFS`` has extra dependencies that should be added to ``main/idf_component.yml``

- **Prepare the image files**

   LVGL's ``LVGLImage.py`` Python tool can be used to convert images to binary pixel map files.
   It supports various formats and compression.

   Meanwhile 3rd party libraries
   (like :ref:`LodePNG<lodepng_rst>` and :ref:`Tiny JPEG<tjpgd>`)
   allow using image files without conversion.

   After preparing the files, they should be moved to the target device:

   - If properly activated a **SPIFFS** file system based on the ``spiffs_image`` folder should be automatically generated and later flashed to the target
   - Similar mechanism for **LittleFS** uses the ``flash_data`` folder, but it's only available for Linux hosts
   - For the **SD Card**, a traditional file browser can be used

- **Invoke proper API calls in the application code**

   The core functionality requires only a few lines. The following example draws the image as well.

   .. code:: c

      #include "esp_spiffs.h"

      void lv_example_image_from_esp_fs(void) {

         esp_vfs_spiffs_conf_t conf = {
            .base_path = "/spiffs",
            .partition_label = NULL,
            .max_files = 5,
            .format_if_mount_failed = false
         };

         esp_err_t ret = esp_vfs_spiffs_register(&conf);

         if (ret != ESP_OK) {
            ESP_LOGE(TAG, "Failed to register SPIFF filesystem");
            return;
         }

         lv_obj_t * obj = lv_image_create(lv_screen_active());
         lv_image_set_src(widget, "A:/spiffs/logo.bin");
         lv_obj_center(widget);
      }

- **Build and flash**

   After calling ``idf.py build flash`` the picture should be displayed on the screen.


.. note::

   Changes made by ``menuconfig`` are not being tracked in the repository if the ``sdkconfig`` file is added to ``.gitignore``, which is the default for many ESP-IDF projects.
   To make your configuration permanent, add the following lines to ``sdkconfig.defaults``:

   .. code:: c

      CONFIG_PARTITION_TABLE_CUSTOM=y
      CONFIG_LV_USE_FS_STDIO=y
      CONFIG_LV_FS_STDIO_LETTER=65
      CONFIG_LV_FS_DEFAULT_DRIVER_LETTER=65
