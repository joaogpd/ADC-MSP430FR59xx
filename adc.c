#include <adc.h>

adc_cb_t adc_callback = NULL;

#define ADC12MCTLx_BASE (ADC12_B_BASE + OFS_ADC12MCTL0)
#define ADC12MCTLx(channel) (ADC12MCTLx_BASE + (2 * channel))

void configure_adc(uint16_t resolution, uint16_t interrupts_0, uint16_t interrupts_1,
                   uint16_t interrupts_2, uint8_t start_address)
{
    ADC12IFGR0 = 0x00;
    ADC12IFGR1 = 0x00;
    ADC12IFGR2 = 0x00;

    ADC12CTL0 = ADC12SHT0_2 | ADC12ON;        // Sampling time, S&H=16, ADC12 on
    ADC12CTL1 = ADC12SHP | ADC12SSEL_3;      // Use sampling timer, source SMCLK
    ADC12CTL2 |= resolution | ADC12PWRMD; // Select ADC resolution, run in low-power mode
    ADC12CTL3 |= start_address;
    ADC12IER0 |= interrupts_0; // Enable selected ADC interrupts
    ADC12IER1 |= interrupts_1;
    ADC12IER2 |= interrupts_2;
}

void set_vref(uint16_t voltage)
{
    while (REFCTL0 & REFGENBUSY)
        ;              // If reference generator busy, WAIT
    REFCTL0 |= voltage; // Select internal reference, leave it turned off, will only be enabled when requested
}

void request_adc_values(void)
{
    ADC12CTL0 |= ADC12ENC | ADC12SC;           // Start sampling/conversion
}

void set_adc_callback(adc_cb_t callback)
{
    adc_callback = callback;
}

void set_window_comparator_voltage(uint16_t low, uint16_t high)
{
    ADC12HI = high;
    ADC12LO = low;
}

void enable_channel(uint8_t channel, uint16_t input_select, uint16_t vref_select) {
    ADC12MCTLx(channel) = |= (input_select | vref_select);
}

void enable_V_half_comparator_window(uint8_t channel)
{
    ADC12CTL3 |= ADC12BATMAP; // Internal V 1/2 (V half) selected
    
    ADC12MCTLx(channel) |= (ADC12WINC | ADC12VRSEL_1 | ADC12INCH_31); // Enable Window Comparator
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = ADC12_VECTOR
__interrupt void ADC12_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(ADC12_VECTOR))) ADC12_ISR (void)
#else
#error Compiler not supported!
#endif
{
    if (adc_callback != NULL)
    {
        adc_callback();
    }
}

