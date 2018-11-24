# ThermalPrinter
Simple sketch for bitmap printing without library compatible with Arduino/ESP8266/ESP32

NO LIBRARY image printing for standard ESC/POS printer also known as EPSON script.
This sketch is using just standard Arduino.h library and raw commands described
by ESC/POS® Command Reference
https://reference.epson-biz.com/modules/ref_escpos/index.php?content_id=2
 
Some printers like SparkFun or AdaFruit thermal printers are using newer commands
which should not work on older ESP/POS printers. This is reason why libraries doesnt work
for all printers. This sketch is written for GSerial GP-5890XIII but it should be 
compatible with all ESC/POS printers as it is using only standart and some obsolete commands.

![](printing.gif)
