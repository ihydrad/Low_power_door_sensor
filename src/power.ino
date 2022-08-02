void sleep(){
  radio.powerDown();
  sleep_enable();
  ADCSRA = 0;
  PCICR = 0x01;     //PCIE0: Pin Change Interrupt Enable 0
  PCMSK0 = 0x01;    //Bit 7..0 – PCINT7..0: Pin Change Enable Mask 7..0
  DDRB = 0XFE;
  DDRD = 0XFF;
  DDRC = 0XCF; // SDA, SCL HARD PULLED UP to VCC, set pin to HI-Z
  PORTB  = 0x00;
  PORTD  = 0x00;
  PORTC  = 0x30;// SDA, SCL HARD PULLED UP to VCC
  // turn off brown-out enable in software
  MCUCR = bit (6) | bit (5);
  MCUCR = bit (6);
  sleep_cpu ();
}

void wakeup(){
  sleep_disable();
  radio.powerUp();
  delay(250);
  pinMode(ADCBATPIN, INPUT);
  pinMode(ADCVCC, OUTPUT);digitalWrite(ADCVCC, 1);
  ADCSRA = 0x87; //62Khz adc
}
