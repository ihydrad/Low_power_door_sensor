#include <Arduino.h>
#include  <avr/sleep.h>
#include  <avr/wdt.h>
#include <RF24.h>
#include <Wire.h>
#include <Adafruit_BME280.h>

#define DEBUG

#ifdef DEBUG
  //#include <printf.h>
#endif

#define SBR(port, bit)        port |= (1<<bit)
#define CBR(port, bit)        port &= (~(1<<bit))
#define INV(port, bit)        port ^= (1<<bit)
#define SBRC(port, bit)      ((port & (1<<bit)) == 0)
#define SBRS(port, bit)      ((port & (1<<bit)) != 0)

#define SLEEP_DEL_MIN        3
#define SENSPIN              8  //геркон
#define ADCGND               A3
#define ADCBATPIN            A2
#define ADCVCC               A1
#define DIV_ADC              2.17   //делитель АЦП для расчета напряжения
#define PayloadSize          10    //размер посылки в байтах для NRF
#define Channel              125
#define I2C_BME_ADR          0x76
//******************************************************
Adafruit_BME280 bme;
RF24 radio(9, 10); //4 for CE and 15 for CSN

uint8_t nrf_data[PayloadSize],
        rx_adrr[]="2Node",
        timer_wdt=255;
bool    intEvent;

void setup(){
  initdev();
}

void preparedata(){
  bme.begin(I2C_BME_ADR);
  bme.setSampling(Adafruit_BME280::MODE_FORCED,     /* Operating Mode. */
                  Adafruit_BME280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BME280::SAMPLING_X2,
                  Adafruit_BME280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BME280::FILTER_X16,      /* Filtering. */
                  Adafruit_BME280::STANDBY_MS_0_5); /* Standby time. */

  nrf_data[0] = digitalRead(SENSPIN)? 200: 100;
  int16_t temp = bme.readTemperature()*100;
  nrf_data[1] = temp>>8;
  nrf_data[2] = temp&0xFF;
  uint16_t bat = float(analogRead(ADCBATPIN))/DIV_ADC;
  nrf_data[3] = bat>>8;
  nrf_data[4] = bat&0xFF;
  uint32_t press = bme.readPressure();
  nrf_data[5] = press >> 24;
  nrf_data[6] = (press >> 16)&0xFF;
  nrf_data[7] = (press >> 8)&0xFF;
  nrf_data[8] = press&0xFF;
  nrf_data[9] = bme.readHumidity();

  #ifdef DEBUG
    for(uint8_t i=0;i<32;i++){
      Serial.print(nrf_data[i], HEX);
      Serial.print(";");
    }
    Serial.println();
  #endif
}

void loop() {
  if( (timer_wdt > 7*SLEEP_DEL_MIN) || intEvent){ // WDT interrupt период 8 сек, 8*(7*SLEEP_DEL_MIN)
    if(intEvent)
      intEvent = 0;
    timer_wdt = 0;
    wakeup();
    preparedata();
    nrfrun();
    #ifdef DEBUG
      delay(1000);
    #endif
    sleep();
  }else
    sleep();
}
