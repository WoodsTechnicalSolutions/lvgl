========================================
Support for Pixel Processing Accelerator
========================================

Some ESP32 chip series, like the ESP32-P4 support the Pixel Processing Accelerator hardware (PPA), which is capable of
speeding-up the filling and image blending operations, this peripheral works with the
DMA-2D hardware which is responsible to move the input/output buffers into/from the PPA processing engine.

Supported devices
-----------------

The Espressif targets that supports the PPA are:

- ESP32-P4 series.


Using the PPA on your ESP-IDF project
-------------------------------------

LVGL supports, in experimental level, the filling and the image blending
acceleration through the PPA, the user can enable it inside their ``sdkconfig.default`` by
adding the following option to enable the PPA draw unit in conjunction to the software render:

   .. code:: c

      CONFIG_LV_USE_PPA=y

Save the file and then rebuild the project, this will be sufficient to add the PPA code and it will start to run automatically, so
no further steps are required from the user code perspective.

Limitations
-----------

Please notice that the PPA is at experimental level where some performance gains are expected on drawing tasks related
to rectangle copy or filling, while for image blending, even though it is operational, there is no significant gains,
the initial cause for that according to the PPA section from reference manual is due to the DMA-2D memory bandwidth.

API
***

.. API startswith:  lv_draw_ppa_
