/**
 * STM32F1 USB Low Resistance Tester
 * Доплнение к этому видео https://youtu.be/9jlpHW8rlkc
 * 
 * ########## В А Ж Н О ##########
 * Скетч для проверки заниженных сопротивлений на линиях D+ и D- USB
 * ЭТО ТОЛЬКО ПРИМЕР, ИСПОЛЬЗОВАТЬ КАК ЕСТЬ НЕ РЕКОМЕНДУЮ И ОТВЕТСТВЕННОСТИ НЕ НЕСУ!
 * РАБОТАТЬ ТОЛЬКО С НАПРЯЖЕНИЕМ 3.3 ВОЛЬТА, НИКАКИХ 5 ВОЛЬТ!
 * Обратите внимание на #define __DEBUG
 * 
 * Форум STM32Duino https://www.stm32duino.com/
 * 
 ** Ядро для поддержки плат STM32 от rogerclarkmelbourne https://github.com/rogerclarkmelbourne/Arduino_STM32
 ** Для его работы нужно:
 *** Установить "Arduino SAM boards (Cortex-M3)" через менеджер плат
 *** Скопировать содержимое скаченного архива в папку "hardware" по пути C:\Users\[Имя пользователя]\Documents\Arduino
 *** Если папки "hardware" нет, ее нужно создать самостоятельно
 * 
 * Настройки платы: (которые использовал я)
 * Board              Generic STM32F103C Series
 * Variant            STM32F103CB(20k RAM. 128k Flash)
 * Uploud Method      STLink
 * CPU Speed          48Mhz (Slow - With USB)
 * Optimize           Smallest (Default)
 * 
 ** Связь со мной:
 ** YouTube https://www.youtube.com/channel/UCbkE52YKRphgkvQtdwzQbZQ
 ** Telegram https://www.t.me/slabyt
 ** Instagram https://www.instagram.com/romasklr
 ** VK https://vk.com/id395646965
 ** Facebook https://www.facebook.com/romasklyar94
 ** Twitter https://twitter.com/_SklyarRoman
 ** GitHub https://github.com/S-LABc
 * 
 ** COMPILED IN ARDUINO v1.8.13
 ** Copyright (C) 2021. v1.0 / Скляр Роман S-LAB
*/
/* Раскоментировать, если нужно посмотреть значения в Мониторе порта
Например для калибровки DELTA_VOLTAGE_DROP */
//#define __DEBUG
/* Выводы светодиода и АЦП для USB
НЕ ИСПУЛЬЗУЙТЕ ВЫВОДЫ ВСТРОЕЕНОГО USB PA11, PA12 */
#define DATA_PLUS          PA0 // Можно PA0-PA7, PB0-PB1
#define DATA_MINUS         PA1 // Можно PA0-PA7, PB0-PB1
#define LED                PC13
/* Задержки для мигания */
#define BLINK_PERIOD_1      100
#define BLINK_PERIOD_2      200
/* Отклонение падений напряжений на линиях D+ и D-
В видео я забыл сказать, что это число нужно откалибровать на основе живых и убитых плат
Мое значение (100) чисто по одной живой плате */
#define DELTA_VOLTAGE_DROP 100
/* Структура для удобства работы с перемеными */
struct Voltage {
  uint16_t data_plus;
  uint16_t data_minus;
  int16_t difference;
} voltage;

void setup() {
  /* Отключает JTAG, оставляет SWD. Иначе PB3,PB4,PA15 будут заняты JTAG */
  enableDebugPorts();
  initLines();
  initLED();
#ifdef __DEBUG
Serial.begin();
while (!Serial);
#endif
}

void loop() {
  /* Проверяем линии D+ и D- */
  testLines();
}

void testLines() {
  /* Получение напряжения на каждом выводе АЦП */
  voltage.data_plus =  analogRead(DATA_PLUS);
  voltage.data_minus = analogRead(DATA_MINUS);
  /* Нахождение разности, т.е. отличий в падении напряжений на каждой линии данных USB */
  voltage.difference = voltage.data_plus - voltage.data_minus;
#ifdef __DEBUG
Serial.print(voltage.data_plus);
Serial.print("\t");
Serial.print(voltage.data_minus);
Serial.print("\t");
Serial.print(voltage.difference);
Serial.print("\t");
#endif
  /* Проверка на наличие существенного отклонения между падениями */
  if (voltage.difference > DELTA_VOLTAGE_DROP) {
    blinkLED(BLINK_PERIOD_1);
#ifdef __DEBUG
Serial.print("Voltage at D- LOW");
Serial.print("\n");
#endif
  }
  else if (voltage.difference < -DELTA_VOLTAGE_DROP) {
    blinkLED(BLINK_PERIOD_2);
#ifdef __DEBUG
Serial.print("Voltage at D+ LOW");
Serial.print("\n");
#endif
  }
  else {
#ifdef __DEBUG
Serial.print("\n");
#endif
  }
  delay(10);
}
/* Настройка выводов для АЦП */
void initLines() {
  pinMode(DATA_PLUS, INPUT_ANALOG);
  pinMode(DATA_MINUS, INPUT_ANALOG);
}
/* Настройка вывода и начального состояния для LED */
void initLED() {
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);
}
/* Мигалка */
void blinkLED (uint16_t time) {
  digitalWrite(LED, LOW);
  delay(time);
  digitalWrite(LED, HIGH);
  delay(time);
}
