################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccsv6/tools/compiler/ti-cgt-msp430_4.4.3

ORDERED_OBJS += \
"./clocks.obj" \
"./main.obj" \
"./utils.obj" \
"./Touch/touch_module.obj" \
"./Display/display.obj" \
"./Display/display_utils.obj" \
"./CircularBuffer/circularBuffer.obj" \
"./Buzzer/buzzer.obj" \
"./AFE/ADS1291_module.obj" \
"../lnk_msp430fr5972.cmd" \
$(GEN_CMDS__FLAG) \
-llibmpu_init.a \
-l"libmath.a" \
-l"libc.a" \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include Touch/subdir_vars.mk
-include Display/subdir_vars.mk
-include CircularBuffer/subdir_vars.mk
-include Buzzer/subdir_vars.mk
-include AFE/subdir_vars.mk
-include subdir_rules.mk
-include Touch/subdir_rules.mk
-include Display/subdir_rules.mk
-include CircularBuffer/subdir_rules.mk
-include Buzzer/subdir_rules.mk
-include AFE/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
tiic_2015_code.out \

EXE_OUTPUTS__QUOTED += \
"tiic_2015_code.out" \

BIN_OUTPUTS += \
tiic_2015_code.hex \

BIN_OUTPUTS__QUOTED += \
"tiic_2015_code.hex" \


# All Target
all: tiic_2015_code.out

# Tool invocations
tiic_2015_code.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: $@'
	@echo 'Invoking: MSP430 Linker'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-msp430_4.4.3/bin/cl430" -vmspx --abi=eabi --data_model=restricted --use_hw_mpy=F5 --advice:power="all" --advice:hw_config="all" -g --define=__MSP430FR5972__ --define=_MPU_ENABLE --diag_warning=225 --display_error_number --diag_wrap=off --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --printf_support=minimal -z -m"tiic_2015_code.map" --heap_size=160 --stack_size=160 --cinit_hold_wdt=on -i"C:/ti/ccsv6/ccs_base/msp430/include" -i"C:/ti/ccsv6/tools/compiler/ti-cgt-msp430_4.4.3/lib" -i"C:/ti/ccsv6/tools/compiler/ti-cgt-msp430_4.4.3/include" -i"C:/ti/ccsv6/ccs_base/msp430/lib/5xx_6xx_FRxx" -i"C:/ti/ccsv6/ccs_base/msp430/lib/FR59xx" --reread_libs --priority --define=_MPU_ENABLE --warn_sections --display_error_number --diag_wrap=off --xml_link_info="tiic_2015_code_linkInfo.xml" --use_hw_mpy=F5 --rom_model -o "tiic_2015_code.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

tiic_2015_code.hex: $(EXE_OUTPUTS)
	@echo 'Invoking: MSP430 Hex Utility'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-msp430_4.4.3/bin/hex430" --memwidth=8 --romwidth=8 -o "tiic_2015_code.hex" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXE_OUTPUTS__QUOTED)$(BIN_OUTPUTS__QUOTED)
	-$(RM) "clocks.pp" "main.pp" "utils.pp" "Touch\touch_module.pp" "Display\display.pp" "Display\display_utils.pp" "CircularBuffer\circularBuffer.pp" "Buzzer\buzzer.pp" "AFE\ADS1291_module.pp" 
	-$(RM) "clocks.obj" "main.obj" "utils.obj" "Touch\touch_module.obj" "Display\display.obj" "Display\display_utils.obj" "CircularBuffer\circularBuffer.obj" "Buzzer\buzzer.obj" "AFE\ADS1291_module.obj" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

