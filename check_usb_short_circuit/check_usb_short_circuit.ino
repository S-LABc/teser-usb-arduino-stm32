/**
 * Check USB Short Circuit
 * Доплнение к этим видео https://youtu.be/fyskn_VxkL4 https://youtu.be/9jlpHW8rlkc
 * 
 * ########## В А Ж Н О ##########
 * Второй способ проверки заниженных сопротивлений на линиях D+ и D- USB
 * ЭТО ТОЛЬКО ПРИМЕР, ИСПОЛЬЗОВАТЬ КАК ЕСТЬ НЕ РЕКОМЕНДУЮ И ОТВЕТСТВЕННОСТИ НЕ НЕСУ!
 * ИСПОЛЬОВАТЬ ПИТАНИЕ ОТ USB НЕЛЬЗЯ! ИСПОЛЬЗУЙТЕ ОТДЕЛЬНОЕ ПИТАНИЕ НА 3.3В
 * Обратите внимание на #define FULL_TEST
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
 * Variant            STM32F103C8(20k RAM. 64k Flash)
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

// Выбор одно из двух вариантов проверки.
// Закоментировать #define FULL_TEST если нужен простой вариант
//#define FULL_TEST

#define LED PC13 // Вывод светодиода
#define D_P PA12 // Вывод Data +
#define D_M PA11 // Вывод Data -

void setup() {
  Serial.end(); // "Отключил USB" от выводов D+ и D-
  pinMode(LED, OUTPUT); // Настроил светодиод
  pinMode(D_P, OUTPUT); // Настроил вывод D+
  pinMode(D_M, OUTPUT); // Настроил вывод D-
  digitalWrite(LED, HIGH); // Выключил светодиод
  digitalWrite(D_P, HIGH); // Подал +3.3В на линию D+
  digitalWrite(D_M, HIGH); // Подал +3.3В на линию D-
}

void loop() {
#ifdef FULL_TEST // Выбран понлный вариант
  if (digitalRead(D_P) == LOW && digitalRead(D_M) == LOW) { // Если замкнуты оба
    digitalWrite(LED, !digitalRead(LED));
    delay(50); // Очень часто мигает (0.05 сек)
  }
  else if (digitalRead(D_P) == LOW) { // Если замкнут D+
    digitalWrite(LED, !digitalRead(LED));
    delay(1000); // Мигает с интервалом 1 сек
  }
  else if (digitalRead(D_M) == LOW) { // Если замкнут D-
    digitalWrite(LED, !digitalRead(LED));
    delay(500); // Мигает с интервалом 0.5 сек
  }
  else { // Если замыканий нет
    digitalWrite(LED, LOW); // Постоянно светит
  }
#else // Выбран не полный вариант
  if (digitalRead(D_P) == LOW || digitalRead(D_M) == LOW) { // Если замыкание(я) есть
    digitalWrite(LED, !digitalRead(LED));
    delay(50); // Очень часто мигает (0.05 сек)
  }
  else { // Если замыканий нет
    digitalWrite(LED, LOW); // Постоянно светит
  }
#endif
}
