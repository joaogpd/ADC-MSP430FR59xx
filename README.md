# ADC-MSP430FR59xx

Hardware abstraction layer (HAL) for accessing the ADC12B module in the MSP430FR59xx family of boards, by Texas Instruments.

There are commodity functions for enabling interrupts, configuring the ADC for operation, etc. This abstraction
seeks higher energy efficiency, so the ADC is configured to use the least amount of power possible. Moreover, 
it offers a callback that can be inserted into the ISR when it's called, but that needs to be user defined.

One example is offered, which also makes use of the RTC abstraction located in [this repository](https://github.com/joaogpd/RTC-MSP430FR59xx.git)
for reading the temperature from an analog sensor and printing the value to standard output.
