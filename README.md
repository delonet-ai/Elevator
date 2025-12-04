Project migrateg on ESP32, now available here: https://github.com/delonet-ai/Elevator-ESP32 **WIREELESS REMOTE!!**

Проект переехал на ESP32, доступен вот тут: https://github.com/delonet-ai/Elevator-ESP32

**English version below**
# 🚀 Макет лифта на Arduino
BOM и STL будут внизу.
Фотки в папке pics

Этот проект реализует управление шаговым двигателем с помощью Arduino Nano для перемещения лифта между тремя этажами. Прошивка поддерживает автоматическую калибровку, ручное управление и вызовы на этажи. 
Roadmap:
- Хранение калибровки и текущей позиции в EEPROM
- плавный разгон-торможение
- вывод на дисплей 128х32 точки информации о движении и этаже
- Переделка пульта с 14 жил на 4 через еще одну Arduino Nano
- Перевод на питание через 4S 18650 и BMS
- Экстренный режим (если нажат концевик)
- Перевод в режим рекалибровки и сброс всех данных EEPROM
## ⚙️ Возможности

* Поддержка 3 этажей
* Калибровка верхней и нижней границ
* Режим ручного управления (вверх/вниз)
* Вызов на этажи кнопками
* Световая индикация кнопок
* Отображение статуса в Serial Monitor
* Плавное ускорение и торможение в авто-режиме
* Регулируемая скорость с помощью потенциометра

## 🔌 Подключение компонентов

| Компонент              | Пин Arduino | Описание                 |
| ---------------------- | ----------- | ------------------------ |
| Кнопка "Вверх"         | D10         | Режим INPUT\_PULLUP      |
| Кнопка "Вниз"          | D11         | Режим INPUT\_PULLUP      |
| Кнопка "Этаж 1"        | D7          | Режим INPUT\_PULLUP      |
| Кнопка "Этаж 2"        | D8          | Режим INPUT\_PULLUP      |
| Кнопка "Этаж 3"        | D9          | Режим INPUT\_PULLUP      |
| Подсветка "Вверх"      | D5          | LOW = включено           |
| Подсветка "Вниз"       | D6          | LOW = включено           |
| Подсветка Этаж 1       | D2          | LOW = включено           |
| Подсветка Этаж 2       | D3          | LOW = включено           |
| Подсветка Этаж 3       | D4          | LOW = включено           |
| Шаговый двигатель STEP | D13         | Сигнал шагов             |
| Шаговый двигатель DIR  | A3          | Направление вращения     |
| Концевик (верхний)     | A1          | Режим INPUT\_PULLUP      |
| Потенциометр           | A2          | Задаёт скорость движения |

Если вы не поняли про водсветку - кнопки у меня с резистром на подсветке, поэтому подключены к +5, а пины дают землю при включении.

## 🛠 Установка
Если вы здесь - вы знаете как загрузить .ino в вашу ардуино

## 📏 Калибровка

1. Кнопками "Вверх" и "Вниз" подведите кабину к верхней точке.
2. Нажмите концевик → верхняя граница будет установлена.
3. Кнопкой "Вниз" опустите кабину до низа.
4. Коротким нажатием кнопки "Этаж 1" установите нижнюю границу.
5. После этого доступен авто-режим: кнопки этажей начинают работать.

## 🚦 Управление

* Нажатие кнопки этажа вызывает лифт к нужному уровню.
* Подсветка кнопки загорается на время движения.
* При движении вверх/вниз вручную используется минимальная скорость.
* В авто-режиме скорость — в зависимости от положения потенциометра (1–15×).

## ⚡ Конфигурация

```cpp
const unsigned int STEP_DELAY = 1000;  // Минимальная задержка (медленное движение)
```

Скорость регулируется через потенциометр на A2 (map в диапазон от 1 до 15).

## 🧪 Отладка

Открывайте Serial Monitor (9600 бод), чтобы видеть:

* Текущую позицию
* Режим работы (MANUAL / AUTO)
* Целевую позицию

---

!!! BOM:
- 4 трубы 25мм - 1 метр (или любая высота)
- 3д принтер и PLA пластик (1-1,5 кг)
- Название	Кол-во	Примечание
- Arduino Nano	https://sl.aliexpress.ru/p?key=4FCD3xc
- Плата для разводки https://sl.aliexpress.ru/p?key=JFCD3Y3
- Шаговый мотор NEMA17
- Плата для драйвера https://sl.aliexpress.ru/p?key=DnCD3wO
- Драйвер A4988	1	Обязательно радиатор и термопрокладка https://sl.aliexpress.ru/p?key=tFCD3EM
- БП 12 В (2–5 А)	или BMS + 4s 16650 + dc-dc на 12В
- Потенциометр 10 кОм	1	
- OLED-дисплей SSD1306 (0.91")	1	I2C, 128×32, SSD1306
- Кнопки с подстветкой https://sl.aliexpress.ru/p?key=mFCD3h2 

!!! STL + Fusion 360 в папке /CAD и по ссылке
https://www.printables.com/model/1353500-elevator-lift-arduino
https://www.thingiverse.com/thing:7089874

Если используете этот проект — ⭐ на GitHub приветствуется!

*Удачных проектов!*

**English version**

🚀 Arduino Elevator Model

BOM and STL are at the bottom.
Photos are in the pics folder.

This project implements a stepper-motor–driven elevator controller based on an Arduino Nano, supporting movement between three floors.
The firmware includes automatic calibration, manual control, and floor call buttons.

Roadmap:

Store calibration and current position in EEPROM

Smooth acceleration/braking

Display movement status and current floor on a 128×32 OLED

Rework the remote controller from 14 wires down to 4 using an additional Arduino Nano

Power the entire system using a 4S 18650 pack and BMS

Emergency mode (triggered by the upper limit switch)

Recalibration mode and EEPROM wipe

⚙️ Features

3-floor support

Calibration of top and bottom limits

Manual control mode (up/down)

Floor-call buttons

Button illumination

Status output via Serial Monitor

Smooth acceleration and braking in auto mode

Adjustable speed via potentiometer

🔌 Component Connections
Component	Arduino Pin	Description
“Up” button	D10	INPUT_PULLUP mode
“Down” button	D11	INPUT_PULLUP mode
Floor 1 button	D7	INPUT_PULLUP mode
Floor 2 button	D8	INPUT_PULLUP mode
Floor 3 button	D9	INPUT_PULLUP mode
Up indicator LED	D5	LOW = on
Down indicator LED	D6	LOW = on
Floor 1 LED	D2	LOW = on
Floor 2 LED	D3	LOW = on
Floor 3 LED	D4	LOW = on
Stepper STEP	D13	Step signal
Stepper DIR	A3	Direction signal
Limit switch (top)	A1	INPUT_PULLUP mode
Potentiometer	A2	Controls movement speed

If you're confused about the LED wiring — the buttons I use have built-in resistors for LEDs.
They are connected to +5V, and Arduino pins provide ground when turning illumination on.

🛠 Installation

If you’re here, you already know how to upload a .ino to your Arduino.

📏 Calibration

Use the “Up” / “Down” buttons to move the cabin to the top position.

Press the limit switch → the upper boundary is saved.

Use the “Down” button to lower the cabin to the bottom.

Short-press the “Floor 1” button to save the bottom boundary.

After this, auto mode becomes available: floor buttons start working.

🚦 Operation

Pressing a floor button moves the elevator to that floor.

The button’s LED lights up during movement.

Manual up/down movement always uses minimum speed.

In auto mode, speed depends on the potentiometer position (1–15×).

⚡ Configuration
const unsigned int STEP_DELAY = 1000;  // Minimum delay (slow movement)


Speed is controlled by the potentiometer on A2 (mapped from 1 to 15).

🧪 Debugging

Open Serial Monitor (9600 baud) to see:

Current position

Mode (MANUAL / AUTO)

Target position

!!! BOM:

25mm pipes ×4 — 1 meter (or any height)

3D printer + PLA filament (1–1.5 kg)

Name | Qty | Notes

Arduino Nano — https://sl.aliexpress.ru/p?key=4FCD3xc

Breakout board — https://sl.aliexpress.ru/p?key=JFCD3Y3

Stepper motor NEMA17

Driver carrier board — https://sl.aliexpress.ru/p?key=DnCD3wO

Driver A4988 ×1 — requires heatsink + thermal pad https://sl.aliexpress.ru/p?key=tFCD3EM

PSU 12 V (2–5 A) or BMS + 4s 16650 + DC-DC to 12 V

Potentiometer 10 kΩ ×1

OLED display SSD1306 (0.91") ×1 — I2C, 128×32, SSD1306

Lighted push buttons — https://sl.aliexpress.ru/p?key=mFCD3h2

!!! STL + Fusion 360 in /CAD and here:
https://www.printables.com/model/1353500-elevator-lift-arduino

https://www.thingiverse.com/thing:7089874

If you use this project — ⭐ on GitHub is appreciated!

Happy building!


