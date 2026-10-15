cmake_minimum_required(VERSION 3.16)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/stm32_gcc.cmake)

project(stm32-fetch-cube C ASM)

stm32_fetch_cube(G4)
stm32_fetch_hal(G4)

find_package(CMSIS 
    #1.2.6 
    COMPONENTS STM32G474CE REQUIRED)
find_package(HAL 
    #1.2.7 
    REQUIRED COMPONENTS STM32G4)

set(PROJECT_SOURCES
    src/adc.c  
    src/dac.c  
    src/gpio.c  
    src/main.c  
    src/opamp.c  
    src/stm32g4xx_hal_msp.c  
    src/stm32g4xx_it.c
)

add_executable(stm32-fetch-g4 ${PROJECT_SOURCES})

target_include_directories(stm32-fetch-g4 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(stm32-fetch-g4
    CMSIS::STM32::G474CE
    HAL::STM32::G4::GPIO
    HAL::STM32::G4::ADCEx
    HAL::STM32::G4::RCCEx
    HAL::STM32::G4::DAC
    HAL::STM32::G4::PWREx
    HAL::STM32::G4::OPAMP
    HAL::STM32::G4::CORTEX
    STM32::NoSys)


stm32_print_size_of_target(stm32-fetch-g4)
