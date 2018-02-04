//
// Functions of timer 4
//
#include "Arduino.h"

void set_timer1_mode(){
  // initialize the timer 1 for adjusting PWM frequency
  // See data sheet page 133

  //first clean it all:
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;


  // Sets to desired PWM mode (see page 145,151)
  // WGM14:0=8 -> TOP is defined by the ICR1 register
  TCCR1B |= (1 << WGM13) | (1 << WGM12);   //FAstPWM mode
  TCCR1A |= (1 << WGM11) ;                 //FAstPWM mode

}

void set_timer1_output(){

  // Hardware A Timer Output is used.
  TCCR1A |=  _BV(COM1A1);// | _BV(COM4A0) ;

}
void unset_timer1_output(){

  // Hardware A Timer Output is used.
  TCCR1A &=  ~_BV(COM1A1);// | _BV(COM4A0) ;

}


uint8_t set_timer1_prescaler(uint16_t prescaler){
  //
  // Set the prescaler : https://oscarliang.com/arduino-timer-and-interrupt-tutorial/
  //

  switch (prescaler){
  case 1024 :
    //sets Arduino Mega's timer 1 prescaler : 1024
    TCCR1B |= (1<<CS12) | (1<<CS10);
    //TCCR1B &= (0<<CS11);
    break;

  case 256 :
    //sets Arduino Mega's timer 1 prescaler : 256
    TCCR1B |= (1<<CS12);
    //TCCR1B &= (0<<CS11) & (0<<CS10);
    break;

  case 64 :
    //sets Arduino Mega's timer 1 prescaler : 64
    TCCR1B |= (1<<CS11) | (1<<CS10);
    //TCCR1B &= (0<<CS10);
    break;

  case 8 :
    //sets Arduino Mega's timer 1 prescaler : 8
    TCCR1B |= (1<<CS11);
    //TCCR1B &= (0<<CS12) & (0<<CS10);
    break;

  case 1 :
    //sets Arduino Mega's timer 1 prescaler : 1
    TCCR1B |= (1<<CS10);
    //TCCR1B &= (0<<CS12) & (0<<CS11);
    break;

  default :
    return -1;
    break;
  }

  return 1;

}

void set_timer1_DC(float DC){
    OCR1A = uint16_t (DC*ICR1);
}

void set_timer4_freqDC(float freq, float DC){
  // Set the duty-cycle of output A,B and C of timer 1
  // The counter will update to reach ICR4 and if a match is found with OCR4x
  // the OC4x will be set or cleared

  uint16_t prescaler = 1;
  //uint32_t Ncount = uint32_t (F_CPU/prescaler/2/freq);
  uint32_t Ncount = uint32_t (F_CPU/prescaler/freq - 1);
  while(Ncount > 65535){
    if (prescaler <= 8){
      prescaler = prescaler*8;
    }
    else {
      prescaler = prescaler*4;
    }
    Ncount = uint32_t (F_CPU/prescaler/freq - 1);
  }

  if (set_timer1_prescaler(prescaler)<0){
    //Serial.println("error setting prescaler");
    while(1){;}
  }

  // Set the TIMER 4 registers to the corresponding values
  // to obtain the correct PWM
  ICR4  = uint16_t (Ncount);
  OCR4A = uint16_t (DC*Ncount);

  // Serial.print("µDAQ. freq :");
  // Serial.print(freq);
  // Serial.print(", µDAQ DC :");
  // Serial.println(DC);
  //  delay(100);
}
