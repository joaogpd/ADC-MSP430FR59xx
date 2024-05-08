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

typedef void (*adc_cb_t)(void);

/**
 * Configure ADC module and make it ready for use. A few options have been chosen
 * to reduce power consumption: not turning on the REF_A module yet (only when requested by the ADC) 
 * and sourcing the ADC's clock from the ADC12OSC (MODOSC), which is enabled only when used.
 *
 * @param resolution ADC's resolution. Possible values: RES8BIT, RES10BIT, RES12BIT.
 * @param input_select Selection for the analog input pin. This should have been 
 * initialized properly, by selecting the pin's module function.
 * @param interrupts Value of OR'ed selected interrupt sources.
 * @param start_address Channel to start sending conversion values to.
 */
void configure_adc(uint16_t resolution, uint16_t input_select,
                   uint16_t interrupts_2, uint16_t interrupts_1,
                   uint16_t interrupts_0, uint8_t start_address);

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

void set_window_comparator_voltage(uint16_t low, uint16_t high);

void enable_V_half_comparator_window(void);

#endif
