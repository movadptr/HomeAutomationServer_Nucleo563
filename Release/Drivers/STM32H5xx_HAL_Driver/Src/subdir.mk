################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal.c \
../Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_cortex.c \
../Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_dma.c \
../Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_dma_ex.c \
../Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_eth.c \
../Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_eth_ex.c \
../Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_exti.c \
../Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_flash.c \
../Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_flash_ex.c \
../Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_gpio.c \
../Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_i2c.c \
../Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_i2c_ex.c \
../Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_icache.c \
../Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_pwr.c \
../Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_pwr_ex.c \
../Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rcc.c \
../Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rcc_ex.c \
../Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rng.c \
../Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rng_ex.c \
../Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rtc.c \
../Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rtc_ex.c \
../Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_spi.c \
../Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_spi_ex.c \
../Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_tim.c \
../Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_tim_ex.c \
../Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_uart.c \
../Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_uart_ex.c \
../Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_usart.c \
../Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_usart_ex.c \
../Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_ll_rng.c 

OBJS += \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal.o \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_cortex.o \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_dma.o \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_dma_ex.o \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_eth.o \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_eth_ex.o \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_exti.o \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_flash.o \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_flash_ex.o \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_gpio.o \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_i2c.o \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_i2c_ex.o \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_icache.o \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_pwr.o \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_pwr_ex.o \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rcc.o \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rcc_ex.o \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rng.o \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rng_ex.o \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rtc.o \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rtc_ex.o \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_spi.o \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_spi_ex.o \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_tim.o \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_tim_ex.o \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_uart.o \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_uart_ex.o \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_usart.o \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_usart_ex.o \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_ll_rng.o 

C_DEPS += \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal.d \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_cortex.d \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_dma.d \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_dma_ex.d \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_eth.d \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_eth_ex.d \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_exti.d \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_flash.d \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_flash_ex.d \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_gpio.d \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_i2c.d \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_i2c_ex.d \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_icache.d \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_pwr.d \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_pwr_ex.d \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rcc.d \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rcc_ex.d \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rng.d \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rng_ex.d \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rtc.d \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rtc_ex.d \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_spi.d \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_spi_ex.d \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_tim.d \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_tim_ex.d \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_uart.d \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_uart_ex.d \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_usart.d \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_usart_ex.d \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_ll_rng.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/STM32H5xx_HAL_Driver/Src/%.o Drivers/STM32H5xx_HAL_Driver/Src/%.su Drivers/STM32H5xx_HAL_Driver/Src/%.cyclo: ../Drivers/STM32H5xx_HAL_Driver/Src/%.c Drivers/STM32H5xx_HAL_Driver/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m33 -std=gnu11 -DNX_INCLUDE_USER_DEFINE_FILE -DNX_SECURE_INCLUDE_USER_DEFINE_FILE -DTX_INCLUDE_USER_DEFINE_FILE -DTX_SINGLE_MODE_NON_SECURE=1 -DUSE_NUCLEO_64 -DUSE_HAL_DRIVER -DSTM32H563xx -DSTM32_THREAD_SAFE_STRATEGY=2 -c -I../NetXDuo/App -I../NetXDuo/Target -I../Core/Inc -I../AZURE_RTOS/App -I../Drivers/STM32H5xx_HAL_Driver/Inc -I../Drivers/STM32H5xx_HAL_Driver/Inc/Legacy -I../Drivers/BSP/Components/lan8742 -I../Middlewares/ST/netxduo/addons/dhcp -I../Middlewares/ST/netxduo/addons/web -I../Middlewares/ST/netxduo/common/drivers/ethernet -I../Middlewares/ST/threadx/common/inc -I../Drivers/BSP/STM32H5xx_Nucleo -I../Drivers/CMSIS/Device/ST/STM32H5xx/Include -I../Middlewares/ST/netxduo/common/inc -I../Middlewares/ST/netxduo/ports/cortex_m33/gnu/inc -I../Middlewares/ST/netxduo/addons/dns -I../Middlewares/ST/netxduo/addons/sntp -I../Middlewares/ST/netxduo/nx_secure/inc -I../Middlewares/ST/netxduo/nx_secure/ports -I../Middlewares/ST/netxduo/crypto_libraries/inc -I../Middlewares/ST/netxduo/crypto_libraries/ports/cortex_m4/gnu/inc -I../Middlewares/ST/threadx/ports/cortex_m33/gnu/inc -I../Drivers/CMSIS/Include -I../Core/ThreadSafe -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-STM32H5xx_HAL_Driver-2f-Src

clean-Drivers-2f-STM32H5xx_HAL_Driver-2f-Src:
	-$(RM) ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal.cyclo ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal.d ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal.o ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal.su ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_cortex.cyclo ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_cortex.d ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_cortex.o ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_cortex.su ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_dma.cyclo ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_dma.d ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_dma.o ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_dma.su ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_dma_ex.cyclo ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_dma_ex.d ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_dma_ex.o ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_dma_ex.su ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_eth.cyclo ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_eth.d ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_eth.o ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_eth.su ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_eth_ex.cyclo ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_eth_ex.d ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_eth_ex.o ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_eth_ex.su ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_exti.cyclo ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_exti.d ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_exti.o ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_exti.su ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_flash.cyclo ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_flash.d ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_flash.o ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_flash.su ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_flash_ex.cyclo ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_flash_ex.d ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_flash_ex.o ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_flash_ex.su ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_gpio.cyclo ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_gpio.d ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_gpio.o ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_gpio.su ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_i2c.cyclo ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_i2c.d ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_i2c.o ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_i2c.su ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_i2c_ex.cyclo ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_i2c_ex.d ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_i2c_ex.o ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_i2c_ex.su ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_icache.cyclo ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_icache.d ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_icache.o ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_icache.su ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_pwr.cyclo ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_pwr.d ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_pwr.o ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_pwr.su ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_pwr_ex.cyclo ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_pwr_ex.d ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_pwr_ex.o ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_pwr_ex.su ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rcc.cyclo ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rcc.d ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rcc.o ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rcc.su ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rcc_ex.cyclo ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rcc_ex.d ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rcc_ex.o ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rcc_ex.su ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rng.cyclo ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rng.d ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rng.o ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rng.su ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rng_ex.cyclo ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rng_ex.d ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rng_ex.o ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rng_ex.su ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rtc.cyclo ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rtc.d ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rtc.o ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rtc.su ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rtc_ex.cyclo ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rtc_ex.d ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rtc_ex.o ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rtc_ex.su ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_spi.cyclo ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_spi.d ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_spi.o ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_spi.su ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_spi_ex.cyclo ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_spi_ex.d ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_spi_ex.o ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_spi_ex.su ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_tim.cyclo ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_tim.d ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_tim.o ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_tim.su ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_tim_ex.cyclo ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_tim_ex.d ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_tim_ex.o ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_tim_ex.su ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_uart.cyclo ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_uart.d ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_uart.o
	-$(RM) ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_uart.su ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_uart_ex.cyclo ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_uart_ex.d ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_uart_ex.o ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_uart_ex.su ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_usart.cyclo ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_usart.d ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_usart.o ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_usart.su ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_usart_ex.cyclo ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_usart_ex.d ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_usart_ex.o ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_usart_ex.su ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_ll_rng.cyclo ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_ll_rng.d ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_ll_rng.o ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_ll_rng.su

.PHONY: clean-Drivers-2f-STM32H5xx_HAL_Driver-2f-Src

