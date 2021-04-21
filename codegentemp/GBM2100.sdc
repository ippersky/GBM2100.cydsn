# THIS FILE IS AUTOMATICALLY GENERATED
# Project: C:\Users\fatim\Desktop\Polytechnique\GBM2100.cydsn\GBM2100.cyprj
# Date: Wed, 21 Apr 2021 22:59:11 GMT
#set_units -time ns
create_clock -name {CyILO} -period 31250 -waveform {0 15625} [list [get_pins {ClockBlock/ilo}]]
create_clock -name {CyClk_LF} -period 31250 -waveform {0 15625} [list [get_pins {ClockBlock/lfclk}]]
create_clock -name {CyFLL} -period 10 -waveform {0 5} [list [get_pins {ClockBlock/fll}]]
create_clock -name {CyClk_HF0} -period 10 -waveform {0 5} [list [get_pins {ClockBlock/hfclk_0}]]
create_clock -name {CyClk_Fast} -period 10 -waveform {0 5} [list [get_pins {ClockBlock/fastclk}]]
create_clock -name {CyClk_Peri} -period 10 -waveform {0 5} [list [get_pins {ClockBlock/periclk}]]
create_generated_clock -name {CyClk_Slow} -source [get_pins {ClockBlock/periclk}] -edges {1 3 5} [list [get_pins {ClockBlock/slowclk}]]
create_generated_clock -name {CapSense_ModClk} -source [get_pins {ClockBlock/periclk}] -edges {1 255 511} [list [get_pins {ClockBlock/ff_div_43}]]
create_generated_clock -name {I2C_MAX_SCBCLK} -source [get_pins {ClockBlock/periclk}] -edges {1 13 27} [list [get_pins {ClockBlock/ff_div_2}]]
create_generated_clock -name {I2C_BMI_SCBCLK} -source [get_pins {ClockBlock/periclk}] -edges {1 13 27} [list [get_pins {ClockBlock/ff_div_3}]]
create_generated_clock -name {EINK_Clock} -source [get_pins {ClockBlock/periclk}] -edges {1 50001 100001} [list [get_pins {ClockBlock/ff_div_11}]]
create_generated_clock -name {UART_1_SCBCLK} -source [get_pins {ClockBlock/periclk}] -edges {1 73 145} [list [get_pins {ClockBlock/ff_div_5}]]
create_generated_clock -name {CY_EINK_SPIM_SCBCLK} -source [get_pins {ClockBlock/periclk}] -edges {1 2 3} [list [get_pins {ClockBlock/ff_div_6}]]
create_clock -name {CyPeriClk_App} -period 10 -waveform {0 5} [list [get_pins {ClockBlock/periclk_App}]]
create_clock -name {CyIMO} -period 125 -waveform {0 62.5} [list [get_pins {ClockBlock/imo}]]


# Component constraints for C:\Users\fatim\Desktop\Polytechnique\GBM2100.cydsn\TopDesign\TopDesign.cysch
# Project: C:\Users\fatim\Desktop\Polytechnique\GBM2100.cydsn\GBM2100.cyprj
# Date: Wed, 21 Apr 2021 22:58:49 GMT
