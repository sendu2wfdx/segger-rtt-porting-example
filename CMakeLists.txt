cmake_minimum_required(VERSION 3.15)

add_library(segger_rtt STATIC
    SEGGER_RTT/SEGGER_RTT.c
)

target_include_directories(segger_rtt PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/SEGGER_RTT
)

target_compile_features(segger_rtt PUBLIC c_std_99)
