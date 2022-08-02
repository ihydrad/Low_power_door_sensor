
void nrf_init(){
  //Конфигурируем SPI************************
  //SPI.setHwCs(true);
  SPI.begin();
  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(MSBFIRST);
  //*****************************************
  radio.begin();                                        // Инициируем работу nRF24L01+
  radio.setChannel      (Channel);                      // Указываем канал приёма данных (от 0 до 127), 5 - значит приём данных осуществляется на частоте 2,405 ГГц (на одном канале может быть только 1 приёмник и до 6 передатчиков)
  radio.setDataRate     (RF24_2MBPS);                 // Указываем скорость передачи данных (RF24_250KBPS, RF24_1MBPS, RF24_2MBPS), RF24_1MBPS - 1Мбит/сек
  radio.setPALevel      (RF24_PA_MAX);                 // Указываем мощность передатчика (RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm, RF24_PA_HIGH=-6dBm, RF24_PA_MAX=0dBm)
  //radio.enableDynamicPayloads();
  radio.setAutoAck(false);
  radio.setRetries(0, 0);
  radio.setAddressWidth(3);
  radio.openWritingPipe (rx_adrr);
  radio.setCRCLength    (RF24_CRC_8);
  radio.stopListening();
}

bool nrfrun(){
  if(radio.write(&nrf_data, PayloadSize)){
    #ifdef DEBUG
    Serial.println("...tx success");
    #endif
    return 1;
  }else{
      timer_wdt = 7*(SLEEP_DEL_MIN - 1);
    #ifdef DEBUG
      Serial.println("...tx fail");
    #endif
    return 0;
  }
}
