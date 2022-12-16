cmake_minimum_required(VERSION 3.20)

message("==== Main ====")
message("=")
    
set(MAIN_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR})
message("= Root Directory: ${MAIN_ROOT_DIR}")
message("=")

set(MAIN_INCLUDES_DIR ${MAIN_ROOT_DIR}/include)
file(GLOB MAIN_INCLUDES "${MAIN_INCLUDES_DIR}/*.h" "${MAIN_INCLUDES_DIR}/*.hpp" "${MAIN_INCLUDES_DIR}/fonts/*.h")

message("= Headers Directory: ${MAIN_INCLUDES_DIR}")
message("= Header Files:")
foreach(file ${MAIN_INCLUDES})
    message("=  - ${file}")
endforeach()
message("=")


# Source
set(MAIN_SOURCE_DIR ${MAIN_ROOT_DIR}/src)
file(GLOB MAIN_SOURCES "${MAIN_SOURCE_DIR}/.c" "${MAIN_SOURCE_DIR}/*.cpp")

message("= Source Directory: ${MAIN_SOURCE_DIR}")
message("= Source Files:")
foreach(file ${MAIN_SOURCES})
    message("=  - ${file}")
endforeach()
message("=")

# Libraries
set(MAIN_INCLUDE_DIRS)
set(MAIN_LIBRARIES)

list(APPEND MAIN_LIBRARIES pico_stdlib)
list(APPEND MAIN_LIBRARIES hardware_i2c)
list(APPEND MAIN_LIBRARIES hardware_flash)
list(APPEND MAIN_LIBRARIES hardware_sync)

# Create Library
include_directories(${MAIN_INCLUDES_DIR})
add_executable(${PROJECT_NAME}_MAIN ${MAIN_SOURCES})

set_target_properties(${PROJECT_NAME}_MAIN PROPERTIES PUBLIC_HEADER MAIN_INCLUDES_DIR)

# Link Libraries
target_include_directories(${PROJECT_NAME}_MAIN PRIVATE ${MAIN_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME}_MAIN PRIVATE ${MAIN_LIBRARIES})

pico_enable_stdio_usb(${PROJECT_NAME}_MAIN 1)
pico_enable_stdio_uart(${PROJECT_NAME}_MAIN 0)

pico_add_extra_outputs(${PROJECT_NAME}_MAIN)