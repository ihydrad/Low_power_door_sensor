void initdev(){
  CLKPR=(1<<CLKPCE);
  CLKPR=(0<<CLKPCE) | (0<<CLKPS3) | (0<<CLKPS2) | (1<<CLKPS1) | (1<<CLKPS0);
  nrf_init();
  analogReference(INTERNAL);
  ADCSRA = 0;
  ACSR = 0x80;
  PCICR = 0x01;     //PCIE0: Pin Change Interrupt Enable 0
  PCMSK0 = 0x01;    //Bit 7..0 – PCINT7..0: Pin Change Enable Mask 7..0*/
  #ifdef DEBUG
    UCSR0A=0x02;
    UCSR0B=0x08;
    UCSR0C=0x06;
    UBRR0H=0x00;
    UBRR0L=0x0C;
    //printf_begin();
    //radio.printDetails();
  #endif
  WDT_enable();
  set_sleep_mode (SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  MCUCR = bit (6) | bit (5);
  MCUCR = bit (6);
  sleep_cpu ();
}

void WDT_enable(){
  cli();
  wdt_reset();
  MCUSR &= ~(1<<WDRF);
  WDTCSR |= (1<<WDCE) | (1<<WDE); //WDT ISR 8sec
  WDTCSR = (1<<WDP3)| (1<<WDP0);
  WDTCSR |= bit (WDIE);
  sei();
  #ifdef DEBUG
    Serial.println("WDT-EN");
  #endif
}

void WDT_disable(){
  cli();
  wdt_reset();
  MCUSR &= ~(1<<WDRF);
  WDTCSR |= (1<<WDCE) | (1<<WDE); //WDT ISR 8sec
  WDTCSR = (1<<WDP3)| (1<<WDP0);
  WDTCSR  = 0;
  sei();
  #ifdef DEBUG
    Serial.println("WDT-DIS");
  #endif
}
