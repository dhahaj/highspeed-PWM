#include "include.h"

// Configure the PWM clock
// The argument is one of the 5 previously defined modes
void pwm91011configure(int mode)
{
  // TCCR1A configuration
  //  00 : Channel A disabled D9
  //  00 : Channel B disabled D10
  //  00 : Channel C disabled D11
  //  01 : Fast PWM 8 bit
  TCCR1A = 1;

  // TCCR1B configuration
  // Clock mode and Fast PWM 8 bit
  TCCR1B = mode | 0x08;

  // TCCR1C configuration
  TCCR1C = 0;
}

// Set PWM to D9
// Argument is PWM between 0 and 255
void pwmSet9(int value)
{
  OCR1A = value; // Set PWM value
  DDRB |= 1 << 5; // Set Output Mode B5
  TCCR1A |= 0x80; // Activate channel
}

// Set PWM to D10
// Argument is PWM between 0 and 255
void pwmSet10(int value)
{
  OCR1B = value; // Set PWM value
  DDRB |= 1 << 6; // Set Output Mode B6
  TCCR1A |= 0x20; // Set PWM value
}

// Set PWM to D11
// Argument is PWM between 0 and 255
void pwmSet11(int value)
{
  OCR1C = value; // Set PWM value
  DDRB |= 1 << 7; // Set Output Mode B7
  TCCR1A |= 0x08; // Set PWM value
}


/**********************************************************
   Fast PWM on pins 6, 13 (High Speed TIMER 4)

   Do not use analogWrite to pins 6 or 13 if using
   this functions as they use the same timer.

   Those functions will conflict with the
   MSTIMER2 library.
   Uses 7 PWM frequencies between 2930Hz and 187.5kHz

   Timer 4 uses a PLL as input so that its clock frequency
   can be up to 96MHz on standard Arduino Leonardo.
   We limit imput frequency to 48MHz to generate 187.5kHz PWM
   If needed, we can double that up to 375kHz
**********************************************************/



// Configure the PWM clock
// The argument is one of the 7 previously defined modes
void pwm613configure(int mode)
{
  // TCCR4A configuration
  TCCR4A = 0;

  // TCCR4B configuration
  TCCR4B = mode;

  // TCCR4C configuration
  TCCR4C = 0;

  // TCCR4D configuration
  TCCR4D = 0;

  // TCCR4D configuration
  TCCR4D = 0;

  // PLL Configuration
  // Use 96MHz / 2 = 48MHz
  PLLFRQ = (PLLFRQ & 0xCF) | 0x30;
  // PLLFRQ = (PLLFRQ & 0xCF) | 0x10; // Will double all frequencies

  // Terminal count for Timer 4 PWM
  OCR4C = 255;
}

// Set PWM to D6 (Timer4 D)
// Argument is PWM between 0 and 255
void pwmSet6(int value)
{
  OCR4D = value; // Set PWM value
  DDRD |= 1 << 7; // Set Output Mode D7
  TCCR4C |= 0x09; // Activate channel D
}

// Set PWM to D13 (Timer4 A)
// Argument is PWM between 0 and 255
void pwmSet13(int value)
{
  OCR4A = value; // Set PWM value
  DDRC |= 1 << 7; // Set Output Mode C7
  TCCR4A = 0x82; // Activate channel A
}

// Macro to converts from duty (0..100) to PWM (0..255)
#define DUTY2PWM(x)  ((255*(x))/100)


// Start of demostration code
// Generates 4 PWM signals, two constant and two variable
void setup()
{
  //  pinMode(2, OUTPUT);
  //  pinMode(3, OUTPUT);
  // Configure Timer 1 (Pins 9, 10 and 11)
  // It will operate at 62kHz
  // Valid options are:
  //      PWM62k, PWM8k, PWM1k, PWM244 and PWM61
  pwm91011configure(PWM62k);

  // Configure Timer 4 (Pins 6 and 13)
  // It will operate at 187kHz
  // Valid options are:
  //     PWM187k, PWM94k, PWM47k, PWM23k, PWM12k, PWM6k and PWM3k
  pwm613configure(PWM47k);

  // Generate PWM at pin 11 with 30% duty
  // We need to call pwm91011configure before
  // We use here the DUTY2PWM macro
  pwmSet11(DUTY2PWM(50));

  // Generate PWM at pin 13 with 75% duty
  // We need to call pwm613configure
  // We don't here the DUTY2PWM macro
  // We set a PWM value (0..255) instead
  pwmSet13(128);

  // Pins 9 and 6 will change values in the loop function

  // Prepare pin 9 to use PWM
  // We need to call pwm91011configure before
  // For now, we set it at 0%
  pwmSet9(0);

  // Prepare pin 6 to use PWM
  // We need to call pwm613configure
  // For now, we set it at 0%
  pwmSet6(0);
}

void loop()
{
  static uint8_t value;
  /*
     Set PWM at pins 9, 6
    Those fast macros require a previous configuration
    of the channels using the pwmSet9 and pwmSet6
    functions
    The functions pwmSet9 and pwmSet6 can be used
    instead, but the PWM9 and PWM6 macros are faster
    to execute
  */
  PWM9 = value;
  //  OCR1A = value; // Pin9 PWM
  PWM6 = value;
  //  OCR4D = value; // Pin6 PWM

  // Increment PWM value and return to 0 after 255
  //value = (value + 1) % 256;
  value++;

  // Small 10ms delay
  delay(10);
}
