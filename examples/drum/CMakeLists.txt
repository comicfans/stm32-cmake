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



set(TINYUSB_SOURCES
    tinyusb/src/common/tusb_common.h
    tinyusb/src/common/tusb_compiler.h
    tinyusb/src/common/tusb_debug.h
    tinyusb/src/common/tusb_fifo.h
    tinyusb/src/common/tusb_fifo.c
    tinyusb/src/common/tusb_mcu.h
    tinyusb/src/common/tusb_private.h
    tinyusb/src/common/tusb_types.h
    tinyusb/src/common/tusb_verify.h

    tinyusb/src/device/dcd.h
    tinyusb/src/device/usbd.c
    tinyusb/src/device/usbd.h
    tinyusb/src/device/usbd_pvt.h

    tinyusb/src/osal/osal.h
    tinyusb/src/osal/osal_none.h

    tinyusb/src/portable/st/stm32_fsdev/dcd_stm32_fsdev.c
    tinyusb/src/portable/st/stm32_fsdev/fsdev_apm32.h
    tinyusb/src/portable/st/stm32_fsdev/fsdev_at32.h
    tinyusb/src/portable/st/stm32_fsdev/fsdev_ch32.h
    tinyusb/src/portable/st/stm32_fsdev/fsdev_common.h
    tinyusb/src/portable/st/stm32_fsdev/fsdev_common.c
    tinyusb/src/portable/st/stm32_fsdev/fsdev_stm32.h
    tinyusb/src/portable/st/stm32_fsdev/hcd_stm32_fsdev.c

    tinyusb/src/tusb.c
    tinyusb/src/tusb.h
    tinyusb/src/tusb_option.h


    tinyusb/src/class/midi/midi_device.c
    tinyusb/src/class/midi/midi_device.h
    tinyusb/src/class/cdc/cdc.h
    tinyusb/src/class/cdc/cdc_device.h
    tinyusb/src/class/cdc/cdc_device.c

    #tinyusb/hw/bsp/stm32g4/family.c
    #tinyusb/hw/bsp/board.c
    tinyusb/hw/bsp/board_api.h

)


set(PROJECT_SOURCES
    Src/stm32g4xx_hal_msp.c
    Src/stm32g4xx_it.c
    Src/usb_descriptors.c
    #Src/system_stm32g4xx.c
)


set(DEPS CMSIS::STM32::G474CE
    HAL::STM32::G4::GPIO
    HAL::STM32::G4::ADCEx
    HAL::STM32::G4::RCCEx
    HAL::STM32::G4::DAC
    HAL::STM32::G4::PWREx
    HAL::STM32::G4::OPAMP
    HAL::STM32::G4::CORTEX
    HAL::STM32::G4::PCDEx
    HAL::STM32::G4::TIMEx
    HAL::STM32::G4::DMA
    HAL::STM32::G4::LL_USB
    STM32::NoSys)


add_executable(stream_adc ${PROJECT_SOURCES} ${TINYUSB_SOURCES} Src/main.c)
add_executable(midi ${PROJECT_SOURCES} ${TINYUSB_SOURCES} Src/midi.c)

foreach(exe stream_adc midi)
    target_include_directories(${exe}
        PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/Inc 
        ${CMAKE_CURRENT_SOURCE_DIR}/config 
        ${CMAKE_CURRENT_SOURCE_DIR}/tinyusb/src 
        ${CMAKE_CURRENT_SOURCE_DIR}/tinyusb/hw)


    target_link_libraries(${exe}
        ${DEPS})


    stm32_print_size_of_target(${exe})
endforeach()


