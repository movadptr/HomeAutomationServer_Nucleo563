################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/ST/netxduo/addons/dns/nxd_dns.c 

OBJS += \
./Middlewares/ST/netxduo/addons/dns/nxd_dns.o 

C_DEPS += \
./Middlewares/ST/netxduo/addons/dns/nxd_dns.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/ST/netxduo/addons/dns/%.o Middlewares/ST/netxduo/addons/dns/%.su Middlewares/ST/netxduo/addons/dns/%.cyclo: ../Middlewares/ST/netxduo/addons/dns/%.c Middlewares/ST/netxduo/addons/dns/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m33 -std=gnu11 -DNX_INCLUDE_USER_DEFINE_FILE -DNX_SECURE_INCLUDE_USER_DEFINE_FILE -DTX_INCLUDE_USER_DEFINE_FILE -DTX_SINGLE_MODE_NON_SECURE=1 -DUSE_NUCLEO_64 -DUSE_HAL_DRIVER -DSTM32H563xx -DSTM32_THREAD_SAFE_STRATEGY=2 -c -I../NetXDuo/App -I../NetXDuo/Target -I../Core/Inc -I../AZURE_RTOS/App -I../Drivers/STM32H5xx_HAL_Driver/Inc -I../Drivers/STM32H5xx_HAL_Driver/Inc/Legacy -I../Drivers/BSP/Components/lan8742 -I../Middlewares/ST/netxduo/addons/dhcp -I../Middlewares/ST/netxduo/addons/web -I../Middlewares/ST/netxduo/common/drivers/ethernet -I../Middlewares/ST/threadx/common/inc -I../Drivers/BSP/STM32H5xx_Nucleo -I../Drivers/CMSIS/Device/ST/STM32H5xx/Include -I../Middlewares/ST/netxduo/common/inc -I../Middlewares/ST/netxduo/ports/cortex_m33/gnu/inc -I../Middlewares/ST/netxduo/addons/dns -I../Middlewares/ST/netxduo/addons/sntp -I../Middlewares/ST/netxduo/nx_secure/inc -I../Middlewares/ST/netxduo/nx_secure/ports -I../Middlewares/ST/netxduo/crypto_libraries/inc -I../Middlewares/ST/netxduo/crypto_libraries/ports/cortex_m4/gnu/inc -I../Middlewares/ST/threadx/ports/cortex_m33/gnu/inc -I../Drivers/CMSIS/Include -I../Core/ThreadSafe -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middlewares-2f-ST-2f-netxduo-2f-addons-2f-dns

clean-Middlewares-2f-ST-2f-netxduo-2f-addons-2f-dns:
	-$(RM) ./Middlewares/ST/netxduo/addons/dns/nxd_dns.cyclo ./Middlewares/ST/netxduo/addons/dns/nxd_dns.d ./Middlewares/ST/netxduo/addons/dns/nxd_dns.o ./Middlewares/ST/netxduo/addons/dns/nxd_dns.su

.PHONY: clean-Middlewares-2f-ST-2f-netxduo-2f-addons-2f-dns

