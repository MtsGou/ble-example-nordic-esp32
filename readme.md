Repository with example of Bluetooth Low Energy (BLE) application for nordic nRF 51822 module, and scanning with ESP32 to periodically scan the nordic module and near devices. 

The application also contains a serial UART communication to get the advertised names of the current detected devices and a RSSI filter to choose the nearest devices.

This application is an example for a basic BLE application, to use it properly, download the proper softdevice for your module and make sure that the right includes are being made. To check main application, see in nRF5_SDK_12.3.0/ble_peripheral/ble_app/main.c.

For ESP32, check in platformIO that all needed libraries were installed, and check main in BLE_scan_esp32/src/main.cpp.