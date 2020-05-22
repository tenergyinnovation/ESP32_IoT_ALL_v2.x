#include "esp32_iot_all_lib.h"
#include "modbusRTU.h"

void setup() {
  Serial.begin(9600);
  rs485.begin(9600, SERIAL_8N1, RXD2, TXD2);
}

void loop() {
  
  int id = 1;
  float temp = sht20ReadTemp_modbusRTU(id);
  float humi = sht20ReadHumi_modbusRTU(id);
 
  Serial.printf("Info: sht20[0x01] temperature = %.1f\r\n",temp);
  vTaskDelay(500);
  Serial.printf("Info: sht20[0x01] humidity = %.1f\r\n",humi);
  vTaskDelay(500);

}
