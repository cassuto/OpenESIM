
INCLUDES += -Id-mcu-avr/thirdparty/cores \
            -Id-mcu-avr/thirdparty/sim

libdsimmodels_la_LIBADD += \
            $(LIBELF_LIBS) \
            $(LIBSOCKET_LD)

libdsimmodels_la_SOURCES += d-mcu-avr/dmcuavr.c \
                    d-mcu-avr/dmcuavrcreate.c \
                    d-mcu-avr/dmcuavrinit.c \
                    d-mcu-avr/dmcuavrevent.c \
                    d-mcu-avr/dmcuavrconfig.c \
                    d-mcu-avr/dmcuavrreset.c \
                    d-mcu-avr/dmcuavruninit.c \
                    d-mcu-avr/dmcuavrclock.c \
                    d-mcu-avr/dmcuavrmmcus.c \
                    d-mcu-avr/thirdparty/cores/sim_mega128.c \
                    d-mcu-avr/thirdparty/cores/sim_mega16m1.c \
                    d-mcu-avr/thirdparty/cores/sim_mega8.c \
                    d-mcu-avr/thirdparty/cores/sim_tiny25.c \
                    d-mcu-avr/thirdparty/cores/sim_mega1280.c \
                    d-mcu-avr/thirdparty/cores/sim_mega2560.c \
                    d-mcu-avr/thirdparty/cores/sim_mega88.c \
                    d-mcu-avr/thirdparty/cores/sim_tiny4313.c \
                    d-mcu-avr/thirdparty/cores/sim_mega1281.c \
                    d-mcu-avr/thirdparty/cores/sim_mega32.c \
                    d-mcu-avr/thirdparty/cores/sim_megax.c \
                    d-mcu-avr/thirdparty/cores/sim_tiny44.c \
                    d-mcu-avr/thirdparty/cores/sim_mega1284.c \
                    d-mcu-avr/thirdparty/cores/sim_mega324.c \
                    d-mcu-avr/thirdparty/cores/sim_megax4.c \
                    d-mcu-avr/thirdparty/cores/sim_tiny45.c \
                    d-mcu-avr/thirdparty/cores/sim_mega128rfa1.c \
                    d-mcu-avr/thirdparty/cores/sim_mega324a.c \
                    d-mcu-avr/thirdparty/cores/sim_megax8.c \
                    d-mcu-avr/thirdparty/cores/sim_tiny84.c \
                    d-mcu-avr/thirdparty/cores/sim_mega128rfr2.c \
                    d-mcu-avr/thirdparty/cores/sim_mega328.c \
                    d-mcu-avr/thirdparty/cores/sim_megaxm1.c \
                    d-mcu-avr/thirdparty/cores/sim_tiny85.c \
                    d-mcu-avr/thirdparty/cores/sim_mega16.c \
                    d-mcu-avr/thirdparty/cores/sim_mega32u4.c \
                    d-mcu-avr/thirdparty/cores/sim_tiny13.c \
                    d-mcu-avr/thirdparty/cores/sim_tinyx4.c \
                    d-mcu-avr/thirdparty/cores/sim_mega164.c \
                    d-mcu-avr/thirdparty/cores/sim_mega48.c \
                    d-mcu-avr/thirdparty/cores/sim_tiny2313.c \
                    d-mcu-avr/thirdparty/cores/sim_tinyx5.c \
                    d-mcu-avr/thirdparty/cores/sim_mega168.c \
                    d-mcu-avr/thirdparty/cores/sim_mega64.c \
                    d-mcu-avr/thirdparty/cores/sim_tiny2313a.c \
                    d-mcu-avr/thirdparty/cores/sim_usb162.c \
                    d-mcu-avr/thirdparty/cores/sim_mega169.c \
                    d-mcu-avr/thirdparty/cores/sim_mega644.c \
                    d-mcu-avr/thirdparty/cores/sim_tiny24.c \
                    d-mcu-avr/thirdparty/sim/avr_acomp.c \
                    d-mcu-avr/thirdparty/sim/avr_ioport.c \
                    d-mcu-avr/thirdparty/sim/avr_usb.c \
                    d-mcu-avr/thirdparty/sim/sim_cycle_timers.c \
                    d-mcu-avr/thirdparty/sim/sim_irq.c \
                    d-mcu-avr/thirdparty/sim/avr_adc.c \
                    d-mcu-avr/thirdparty/sim/avr_lin.c \
                    d-mcu-avr/thirdparty/sim/avr_watchdog.c \
                    d-mcu-avr/thirdparty/sim/sim_elf.c \
                    d-mcu-avr/thirdparty/sim/sim_utils.c \
                    d-mcu-avr/thirdparty/sim/avr_bitbang.c \
                    d-mcu-avr/thirdparty/sim/avr_spi.c \
                    d-mcu-avr/thirdparty/sim/read_elf.c \
                    d-mcu-avr/thirdparty/sim/sim_gdb.c \
                    d-mcu-avr/thirdparty/sim/sim_vcd_file.c \
                    d-mcu-avr/thirdparty/sim/avr_eeprom.c \
                    d-mcu-avr/thirdparty/sim/avr_timer.c \
                    d-mcu-avr/thirdparty/sim/sim_avr.c \
                    d-mcu-avr/thirdparty/sim/sim_hex.c \
                    d-mcu-avr/thirdparty/sim/avr_extint.c \
                    d-mcu-avr/thirdparty/sim/avr_twi.c \
                    d-mcu-avr/thirdparty/sim/sim_cmds.c \
                    d-mcu-avr/thirdparty/sim/sim_interrupts.c \
                    d-mcu-avr/thirdparty/sim/avr_flash.c \
                    d-mcu-avr/thirdparty/sim/avr_uart.c \
                    d-mcu-avr/thirdparty/sim/sim_core.c \
                    d-mcu-avr/thirdparty/sim/sim_io.c
                    
