
LVGL can be used and configured as standard `ESP-IDF <https://github.com/espressif/esp-idf>`__ component.

If you are new to ESP-IDF, follow the instructions in the `ESP-IDF Programming guide <https://docs.espressif.com/projects/esp-idf/en/stable/esp32/get-started/index.html>`__ to install and set up ESP-IDF on your machine.


Using LVGL in Your ESP-IDF Project
----------------------------------

The simplest way to integrate LVGL into your ESP-IDF project is via the `esp_lvgl_port <https://components.espressif.com/components/espressif/esp_lvgl_port>`__ component. This component, used in the demo projects mentioned above, provides helper functions for easy installation of LVGL and display drivers. Moreover, it can add support for touch, rotary encoders, button or USB HID inputs. It simplifies power savings, screen rotation and other platform specific nuances.

The esp_lvgl_port supports LVGL versions 8 and 9 and is compatible with ESP-IDF v4.4 and above. To add it to your project, use the following command:

.. code:: sh

   idf.py add-dependency "espressif/esp_lvgl_port^2.3.0"

By default, esp_lvgl_port depends on the latest stable version of LVGL, so no additional steps are needed for new projects. If a specific LVGL version is required, specify this in your project to avoid automatic updates. LVGL can also be used without esp_lvgl_port, as described below.

Obtaining LVGL
~~~~~~~~~~~~~~

LVGL is distributed through `ESP Registry <https://components.espressif.com/>`__, where all LVGL releases are uploaded.
In case you do not want to use esp_lvgl_port, you can add `LVGL component <https://components.espressif.com/component/lvgl/lvgl>`__ into your project with following command:

.. code-block:: sh

   idf.py add-dependency "lvgl/lvgl^9.*"

Adjust the ``^9.*`` part to match your LVGL version requirement. More information on version specifications can be found in the `IDF Component Manager documentation <https://docs.espressif.com/projects/idf-component-manager/en/latest/reference/versioning.html#range-specifications>`__. During the next build, the LVGL component will be fetched from the component registry and added to the project.

**Advanced usage: Use LVGL as local component**

For LVGL development and testing, it may be useful to use LVGL as a local component instead of from the ESP Registry, which offers only released versions and does not allow local modifications. To do this, clone LVGL to your project with the following command:

.. code-block:: sh

   git submodule add https://github.com/lvgl/lvgl.git components/lvgl

.. note::

   All components from ``${project_dir}/components`` are automatically added to build.

Configuration
~~~~~~~~~~~~~

To configure LVGL, launch the configuration menu with ``idf.py menuconfig`` in your project root directory. Navigate to ``Component config`` and then ``LVGL configuration``.
