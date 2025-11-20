# 📘 Sistema IoT para Home Office – GS 2025

## 🔎 Descrição do Problema
Ambientes de home office muitas vezes carecem de controle adequado de temperatura, umidade e organização.  
Isso pode gerar desconforto térmico, perda de produtividade e riscos como objetos próximos demais da área de trabalho.

Este projeto resolve esse problema com um sistema IoT simples, eficiente e conectado.

---

## ✅ Solução Proposta
Foi desenvolvido um sistema baseado em **ESP32**, utilizando sensores e comunicação MQTT para monitoramento e alertas.

### Funcionalidades:
- 🌡 Monitoramento de **temperatura e umidade** com DHT22  
- 📏 Medição de **distância** com HC-SR04  
- 🔔 Alertas por **buzzer**  
- 💡 LED indicando temperatura fora do ideal  
- ☁ Envio de dados para a nuvem via **MQTT**  
- 🛰 Controle remoto do LED e do buzzer via MQTT  
- ⚠ Alertas automáticos quando:
  - Temperatura < 20°C  
  - Temperatura > 26°C  
  - Objeto muito próximo (< 20 cm)

---

## 🛰 Comunicação MQTT

### Envio de dados:

### Formato:
```json
{
  "temp": 24.5,
  "umidade": 61,
  "distancia": 113
}
```
gs2025/alerta
gs2025/comando


| Comando        | Ação                    |
| -------------- | ----------------------- |
| `ligar_led`    | Liga o LED              |
| `desligar_led` | Desliga o LED           |
| `beep`         | Emite um beep no buzzer |
-------------------------------------------
| Condição      | Ação                     |
| ------------- | ------------------------ |
| `< 20°C`      | LED ligado + alerta MQTT |
| `20°C a 26°C` | LED desligado            |
| `> 26°C`      | LED ligado + alerta MQTT |
-------------------------------------------
| Condição | Ação                      |
| -------- | ------------------------- |
| `< 20cm` | Buzzer toca + alerta MQTT |
-------------------------------------------



🛠 Componentes Utilizados

- ESP32 DevKit  
- Sensor DHT22  
- Sensor ultrassônico HC-SR04  
- LED  
- Resistor 220 Ω
- Jumpers  
- Protoboard
  
---

Ligações do Circuito

DHT22

| Pino do Sensor | ESP32   |
| -------------- | ------- |
| VCC            | 3.3V    |
| DATA           | GPIO 15 |
| GND            | GND     |

HC-SR04

| Pino do Sensor | ESP32                           |
| -------------- | ------------------------------- |
| VCC            | 5V                              |
| TRIG           | GPIO 5                          |
| ECHO           | GPIO 18 (com divisor resistivo) |
| GND            | GND                             |

LED

| Componente           | Pino                  |
| -------------------- | --------------------- |
| Perna longa (anodo)  | GPIO 2                |
| Perna curta (catodo) | GND via resistor 220Ω |


Link Wokwi - https://wokwi.com/projects/448161179566778369
  
