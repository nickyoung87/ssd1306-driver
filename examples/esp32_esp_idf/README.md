# ESP-IDF Example
This example is when using the C library esp-idf. 

## Setup
You need to include the path to the library in your project folder (not inside main) CMakeLists.txt:

`set(EXTRA_COMPONENT_DIRS "../ssd1306-driver")`

Then inside the main/CMakeLists.txt you may need to add:

`REQUIRES ssd1306-driver`