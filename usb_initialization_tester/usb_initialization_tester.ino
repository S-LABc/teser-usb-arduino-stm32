/**
 * STM32F1 USB Init Tester
 * 
 * ########## В А Ж Н О ##########
 * Скетч для проверки инициализации USB (не заниженных сопротивлений!)
 * Основой для проверки является создание клавиатуры, но можно любого другого устройства
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

#include <USBComposite.h> // Подключили библиотеку для USB
/* Выводы светодиода и пищалки */
#define LED               PC13
#define BUZZER            PA3
/* Задержка для миганий и писка */
#define LED_INIT_MS       500
#define LED_START_MS      100
#define BUZZER_MS         50
/* Для настройки параметров HID */
#define USB_MANUFACTURER  "Sklyar Roman S-LAB"
#define USB_PRODUCT       "STM32F1 USB Init Tester"
#define USB_SERIAL        __DATE__
/* Создание HID и Keyboard на основе HID */
USBHID HID;
HIDKeyboard Keyboard(HID);

void setup() {
  enableDebugPorts(); // Отключает JTAG, оставляет SWD. Иначе PB3,PB4,PA15 будут заняты JTAG
  USBComposite.setManufacturerString(USB_MANUFACTURER); // Производитель устройства
  USBComposite.setProductString(USB_PRODUCT); // Название устройства
  USBComposite.setSerialString(USB_SERIAL); // Серийный номер устройства
  initLED();               // Настроили светодиод
  initBuzzer();            // Настроили пищалку
  HID.begin(HID_KEYBOARD); // запуск инициализации
  while (!USBComposite) {  // Ждем успешной инициализации, медленно мигая
    blinkLED(LED_INIT_MS);
  }
  squeakBuzzer(BUZZER_MS); // Писк после успешной инициализации
}

void loop() {
  blinkLED(LED_START_MS); // Мигаем быстро при успешной инициализации
}

void initLED() {
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
}
void initBuzzer() {
  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, HIGH);
}
void blinkLED(uint32_t length) {
  delay(length);
  digitalWrite(LED, !digitalRead(LED));
}
void squeakBuzzer(uint32_t length) {
  digitalWrite(BUZZER, !digitalRead(BUZZER));
  delay(length);
  digitalWrite(BUZZER, !digitalRead(BUZZER));
}
