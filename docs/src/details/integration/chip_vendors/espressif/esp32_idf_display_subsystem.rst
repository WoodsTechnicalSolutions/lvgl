For successful LVGL project you will need a display driver and optionally a touch driver. Espressif provides these drivers that are built on its `esp_lcd <https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/peripherals/lcd/index.html>`__ component.

-  esp_lcd natively supports for some `basic displays <https://github.com/espressif/esp-idf/tree/master/components/esp_lcd/src>`__
-  Other displays are maintained in `esp-bsp repository <https://github.com/espressif/esp-bsp/tree/master/components/lcd>`__ and are uploaded to ESP Registry
-  Touch drivers are maintained in `esp-bsp repository <https://github.com/espressif/esp-bsp/tree/master/components/lcd_touch>`__ and are uploaded to ESP Registry

These components share a common public API, making it easy to migrate your projects across different display and touch drivers.

To add a display or touch driver to your project, use a command like:

.. code-block:: sh

   idf.py add-dependency "espressif/esp_lcd_gc9a01^2.0.0"
