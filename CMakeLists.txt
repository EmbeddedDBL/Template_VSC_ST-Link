cmake_minimum_required(VERSION 3.23.1)

# Optional: print out extra messages to see what is going on. Comment it to have less verbose messages
set(CMAKE_VERBOSE_MAKEFILE ON)

# Path to toolchain file. This one has to be before 'project()' below
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/arm-none-eabi-gcc.cmake)

# Setup project, output and linker file
project(Template_STM32F401)
set(EXECUTABLE ${PROJECT_NAME}.elf)
set(LINKER_FILE ${CMAKE_SOURCE_DIR}/Device/STM32F401VEHx_FLASH.ld)

enable_language(C ASM)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Optional: issue a message to be sure it uses the correct toolchain file.
message(STATUS "CMAKE_TOOLCHAIN_FILE is: ${CMAKE_TOOLCHAIN_FILE}")

# List of source files
set(SRC_FILES
        main/main.c

        Device/stm32f4xx_it.c
        Device/system_stm32f4xx.c
        Device/startup_stm32f401xe.s
        
        Drivers/Src/stm32f4xx_hal.c
        Drivers/Src/stm32f4xx_hal_adc.c
        Drivers/Src/stm32f4xx_hal_adc_ex.c
        Drivers/Src/stm32f4xx_hal_cortex.c
        Drivers/Src/stm32f4xx_hal_dac.c
        Drivers/Src/stm32f4xx_hal_dac_ex.c
        Drivers/Src/stm32f4xx_hal_dma.c
        Drivers/Src/stm32f4xx_hal_flash.c
        Drivers/Src/stm32f4xx_hal_flash_ex.c
        Drivers/Src/stm32f4xx_hal_gpio.c
        Drivers/Src/stm32f4xx_hal_i2c.c
        Drivers/Src/stm32f4xx_hal_pwr.c
        Drivers/Src/stm32f4xx_hal_rcc.c
        Drivers/Src/stm32f4xx_hal_rcc_ex.c
        Drivers/Src/stm32f4xx_hal_tim.c
        Drivers/Src/stm32f4xx_hal_tim_ex.c
        )

# Build the executable based on the source files
add_executable(${EXECUTABLE} ${SRC_FILES})

# List of compiler defines, prefix with -D compiler option
target_compile_definitions(${EXECUTABLE} PRIVATE
        -DSTM32F401xE
        -DUSE_HAL_DRIVER
        )

# List of includ directories
target_include_directories(${EXECUTABLE} PRIVATE
        main
        CMSIS/Include
        CMSIS/Device/ST/STM32F4xx
        Device
        Drivers/Inc
        )

# Compiler options
target_compile_options(${EXECUTABLE} PRIVATE
        -mcpu=cortex-m4
        -mthumb
        -mfloat-abi=soft

        -fdata-sections
        -ffunction-sections

        -Wall
        -O0
        -g3
        )

# Linker options
target_link_options(${EXECUTABLE} PRIVATE
        -T${LINKER_FILE}
        -mcpu=cortex-m4
        -mthumb
        -mfloat-abi=soft
        --specs=rdimon.specs
        --specs=nano.specs
        -lc
        -lm
        -lnosys
        -Wl,-Map=${PROJECT_NAME}.map,--cref
        -Wl,--gc-sections
        -Xlinker -print-memory-usage -Xlinker
        )

# Optional: Print executable size as part of the post build process
add_custom_command(TARGET ${EXECUTABLE}
        POST_BUILD
        COMMAND ${CMAKE_SIZE_UTIL} ${EXECUTABLE})

# Optional: Create hex, bin and S-Record files after the build
add_custom_command(TARGET ${EXECUTABLE}
        POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -O srec --srec-len=64 ${EXECUTABLE} ${PROJECT_NAME}.s19
        COMMAND ${CMAKE_OBJCOPY} -O ihex ${EXECUTABLE} ${PROJECT_NAME}.hex
        COMMAND ${CMAKE_OBJCOPY} -O binary ${EXECUTABLE} ${PROJECT_NAME}.bin)