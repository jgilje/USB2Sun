cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)
project(usb2sun C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(usb2sun
  main.c
  hid_app.c
  mouse.c
  keyboard.c
)
target_include_directories(usb2sun PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(usb2sun PUBLIC pico_stdlib tinyusb_host tinyusb_board)
pico_enable_stdio_usb(usb2sun 0)
pico_enable_stdio_uart(usb2sun 0)
pico_add_extra_outputs(usb2sun)
