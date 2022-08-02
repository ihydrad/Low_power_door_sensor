ISR(WDT_vect){
  timer_wdt++;
}

ISR(PCINT0_vect) {
  PCICR = 0x00;
  intEvent = 1;
  #ifdef DEBUG
    Serial.print("PCINT0 INTERRUPT, ");
    Serial.print("STATE:");
    Serial.println(digitalRead(8));
  #endif
  nrf_data[0] = digitalRead(SENSPIN)? 200: 100;
}
