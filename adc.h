#ifndef ADC_H
#define ADC_H

#include <msp430.h>
#include <stdint.h>
#include <stddef.h>

#ifndef ADC_CHANNEL_COUNT
#define ADC_CHANNEL_COUNT 1
#endif

// ADC's resolution values.
#define RES12BIT ADC12RES_2
#define RES10BIT ADC12RES_1
#define RES8BIT  ADC12RES_0

#define P12 ADC12INCH_2 // P1.2
#define P13 ADC12INCH_3 // P1.3

#define VREF12 REFVSEL_0 // 1.2V
#define VREF20 REFVSEL_1 // 2.0V
#define VREF25 REFVSEL_2 // 2.5V

#define ADC12MCTLx_BASE (uint16_t*)(ADC12_B_BASE + OFS_ADC12MCTL0)
#define ADC12MCTLx(channel) (uint16_t*)(ADC12MCTLx_BASE + (2 * channel))

typedef void (*adc_cb_t)(void);

/**
 * Configure ADC module and make it ready for use. A few options have been chosen
 * to reduce power consumption: not turning on the REF_A module yet (only when requested by the ADC) 
 * and sourcing the ADC's clock from the ADC12OSC (MODOSC), which is enabled only when used.
 *
 * @param resolution ADC's resolution. Possible values: RES8BIT, RES10BIT, RES12BIT.
 * @param interrupts_0, _1, _2 Value of OR'ed selected interrupt sources.
 * @param start_address Channel to start sending conversion values to.
 */
void configure_adc(uint16_t resolution, uint16_t interrupts_0, uint16_t interrupts_1, 
                   uint16_t interrupts_2, uint16_t start_address);

/**
 * Selects the reference voltage that is to be used for ADC readings.
 *
 * @param voltage Voltage reference value. Possible values: VREF12, VREF20, VREF25
 */
void set_vref(uint16_t voltage);

/**
 * Requests readings from the selected ADC channels. The reading will be available in ADC12MEMx,
 * which should be accessed from the selected ADC callback.
 */
void request_adc_values(void);

/**
 * Selects the ADC callback, which will be called whenever an interrupt occurs.
 */
void set_adc_callback(adc_cb_t callback);

/** 
 * Sets upper and lower voltage thresholds for generating window comparator interrupts.
 */
void set_window_comparator_voltage(uint16_t low, uint16_t high);

/**
 * Enables an ADC channel for sourcing data.
 * @param channel ADC channel, where results will be returned, goes from 0 to 31.
 * @param input_select Select input source for ADC reading. This should have been 
 * initialized properly, by selecting the pin's module function.
 * @param vref_select Select reference voltage.
 */
void enable_channel(uint8_t channel, uint16_t input_select, uint16_t vref_select);

/**
 * Enables the voltage window comparator to use the current source voltage divided by 2
 * (Vhalf). 
 */
void enable_V_half_comparator_window(uint8_t channel);

/**
 * Enables the interrupt on a given channel's conversion memory address. 
 *
 * @param channel Value of the channel, possible values are 0 up to 31. If
 * it extrapolates this range, the function will do nothing and return -1.
 */
int enable_channel_interrupt(uint8_t channel);

#endif
