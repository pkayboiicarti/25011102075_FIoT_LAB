#ifndef ESPLIB_H
#define ESPLIB_H

#include "stm32f4xx_hal.h"
#include <stdint.h>

#define StationMode   1
#define SoftAPMode    2
#define CleanSession  0x02
#define QoS0          0

typedef struct {
    uint16_t keepAlive;
    uint8_t  connectFlag;
    const char *clientID;
} MQTT_InitTypeDef_t;

extern uint8_t uartRxData;

void ESP8266_Init(uint8_t mode, UART_HandleTypeDef huart);
void ESP8266_wifiConnect(const char *ssid, const char *password);
void ESP8266_portConnect(const char *proto, const char *host, const char *port);

void MQTT_connectBroker(MQTT_InitTypeDef_t *mqtt);
void MQTT_publishTopic(const char *topic, const char *payload, uint8_t qos);
void MQTT_subscribeTopic(const char *topic, uint8_t qos);
void MQTTDataHandler(char *topicOut, char *dataOut);
void MQTTTimer(void);

#endif /* ESPLIB_H */
