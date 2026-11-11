#include "ESPLib.h"
#include <stdio.h>
#include <string.h>

static UART_HandleTypeDef esp_uart;
static volatile uint32_t mqtt_ms_tick = 0;
static volatile uint32_t mqtt_keepalive_ms = 60000;
static uint8_t rx_ring[256];
static volatile uint16_t rx_head = 0;
static volatile uint16_t rx_tail = 0;
uint8_t uartRxData = 0;

static void esp_send_at(const char *cmd, uint32_t wait_ms) {
    HAL_UART_Transmit(&esp_uart, (uint8_t *)cmd, (uint16_t)strlen(cmd), 1000);
    HAL_Delay(wait_ms);
}

static void esp_send_raw(const uint8_t *buf, uint16_t len) {
    char cmd[32];
    snprintf(cmd, sizeof(cmd), "AT+CIPSEND=%u\r\n", (unsigned int)len);
    esp_send_at(cmd, 150);
    HAL_UART_Transmit(&esp_uart, (uint8_t *)buf, len, 1000);
    HAL_Delay(200);
}

void ESP8266_Init(uint8_t mode, UART_HandleTypeDef huart) {
    char cmd[32];
    esp_uart = huart;
    esp_send_at("AT\r\n", 300);
    esp_send_at("ATE0\r\n", 200);
    snprintf(cmd, sizeof(cmd), "AT+CWMODE=%u\r\n", (unsigned int)mode);
    esp_send_at(cmd, 400);
}

void ESP8266_wifiConnect(const char *ssid, const char *password) {
    char cmd[128];
    snprintf(cmd, sizeof(cmd), "AT+CWJAP=\"%s\",\"%s\"\r\n", ssid, password);
    esp_send_at(cmd, 4000);
}

void ESP8266_portConnect(const char *proto, const char *host, const char *port) {
    char cmd[128];
    snprintf(cmd, sizeof(cmd), "AT+CIPSTART=\"%s\",\"%s\",%s\r\n", proto, host, port);
    esp_send_at(cmd, 1500);
}

void MQTT_connectBroker(MQTT_InitTypeDef_t *mqtt) {
    uint8_t pkt[64];
    uint16_t idx = 0;
    const char *cid = (mqtt && mqtt->clientID) ? mqtt->clientID : "STM32Node";
    uint16_t cid_len = (uint16_t)strlen(cid);
    uint16_t keep_alive = mqtt ? mqtt->keepAlive : 60;
    uint8_t flags = mqtt ? mqtt->connectFlag : CleanSession;
    uint16_t rem_len = (uint16_t)(10 + 2 + cid_len);

    mqtt_keepalive_ms = (uint32_t)keep_alive * 1000UL;

    pkt[idx++] = 0x10; /* CONNECT */
    pkt[idx++] = (uint8_t)rem_len;
    pkt[idx++] = 0x00; pkt[idx++] = 0x04;
    pkt[idx++] = 'M'; pkt[idx++] = 'Q'; pkt[idx++] = 'T'; pkt[idx++] = 'T';
    pkt[idx++] = 0x04; /* Protocol Level 4 (MQTT 3.1.1) */
    pkt[idx++] = flags;
    pkt[idx++] = (uint8_t)(keep_alive >> 8);
    pkt[idx++] = (uint8_t)(keep_alive & 0xFF);
    pkt[idx++] = (uint8_t)(cid_len >> 8);
    pkt[idx++] = (uint8_t)(cid_len & 0xFF);
    memcpy(&pkt[idx], cid, cid_len);
    idx += cid_len;

    esp_send_raw(pkt, idx);
}

void MQTT_publishTopic(const char *topic, const char *payload, uint8_t qos) {
    uint8_t pkt[256];
    uint16_t idx = 0;
    uint16_t tlen = (uint16_t)strlen(topic);
    uint16_t plen = (uint16_t)strlen(payload);
    uint16_t rem_len = (uint16_t)(2 + tlen + plen);

    (void)qos;
    pkt[idx++] = 0x30; /* PUBLISH QoS0 */
    if (rem_len < 128) {
        pkt[idx++] = (uint8_t)rem_len;
    } else {
        pkt[idx++] = (uint8_t)((rem_len % 128) | 0x80);
        pkt[idx++] = (uint8_t)(rem_len / 128);
    }
    pkt[idx++] = (uint8_t)(tlen >> 8);
    pkt[idx++] = (uint8_t)(tlen & 0xFF);
    memcpy(&pkt[idx], topic, tlen);
    idx += tlen;
    memcpy(&pkt[idx], payload, plen);
    idx += plen;

    esp_send_raw(pkt, idx);
}

void MQTT_subscribeTopic(const char *topic, uint8_t qos) {
    uint8_t pkt[64];
    uint16_t idx = 0;
    uint16_t tlen = (uint16_t)strlen(topic);
    uint16_t rem_len = (uint16_t)(2 + 2 + tlen + 1);

    pkt[idx++] = 0x82; /* SUBSCRIBE */
    pkt[idx++] = (uint8_t)rem_len;
    pkt[idx++] = 0x00; pkt[idx++] = 0x01; /* Packet ID */
    pkt[idx++] = (uint8_t)(tlen >> 8);
    pkt[idx++] = (uint8_t)(tlen & 0xFF);
    memcpy(&pkt[idx], topic, tlen);
    idx += tlen;
    pkt[idx++] = qos;

    esp_send_raw(pkt, idx);
}

void MQTTDataHandler(char *topicOut, char *dataOut) {
    if (rx_tail != rx_head) {
        uint16_t pos = 0;
        while (rx_tail != rx_head && pos < 48) {
            dataOut[pos++] = (char)rx_ring[rx_tail];
            rx_tail = (uint16_t)((rx_tail + 1) % sizeof(rx_ring));
        }
        dataOut[pos] = '\0';
        (void)topicOut;
    }
}

void MQTTTimer(void) {
    mqtt_ms_tick++;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == esp_uart.Instance) {
        uint16_t next = (uint16_t)((rx_head + 1) % sizeof(rx_ring));
        if (next != rx_tail) {
            rx_ring[rx_head] = uartRxData;
            rx_head = next;
        }
        HAL_UART_Receive_IT(&esp_uart, &uartRxData, 1);
    }
}
