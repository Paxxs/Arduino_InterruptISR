#include <avr/iom168p.h>

// cspell:words PCICR PCMSK2 PCINT vect PIND
// pins
#define PIN_BTN_4 4
#define PIN_BTN_5 5
#define PIN_BTN_6 6

#define PIN_LED_9 9
#define PIN_LED_10 10
#define PIN_LED_11 11

// state of Btn
volatile bool g_state_btn_4 = false;
volatile bool g_state_btn_5 = false;
volatile bool g_state_btn_6 = false;
volatile unsigned int g_count4, g_count5, g_count6;

// 抖动计算
#define debounce 250 // time to wait in milli secs
volatile unsigned long g_pre_btn_timer_4, g_pre_btn_timer_5, g_pre_btn_timer_6;
volatile unsigned long g_milli_time;

void setup()
{
    noInterrupts();
    Serial.begin(9600);
    pinMode(PIN_BTN_4, INPUT_PULLUP);
    pinMode(PIN_BTN_5, INPUT_PULLUP);
    pinMode(PIN_BTN_6, INPUT_PULLUP);

    pinMode(PIN_LED_9, OUTPUT);
    pinMode(PIN_LED_10, OUTPUT);
    pinMode(PIN_LED_11, OUTPUT);

    // Bit2 = 1 -> "PCIE2" enabled
    PCICR |= B00000100;

    // PCINT 20,21,22 enabled -> D4, D5, D6 将会触发中断
    PCMSK2 |= B01110000;
    interrupts();
}

void loop()
{
    Serial.println(F("Delay 2s"));
    Serial.print("Btn:");
    Serial.print(g_state_btn_4);
    Serial.print(F(" - "));
    Serial.print(g_state_btn_5);
    Serial.print(F(" - "));
    Serial.println(g_state_btn_6);
    Serial.print("Tim:");
    Serial.print(g_pre_btn_timer_4);
    Serial.print(F(" - "));
    Serial.print(g_pre_btn_timer_5);
    Serial.print(F(" - "));
    Serial.println(g_pre_btn_timer_6);
    Serial.print("Num:");
    Serial.print(g_count4);
    Serial.print(F(" - "));
    Serial.print(g_count5);
    Serial.print(F(" - "));
    Serial.println(g_count6);
    Serial.println(F("-------"));
    delay(2000);
}

ISR(PCINT2_vect)
{
    cli();
    g_milli_time = millis();
    // PD4( Pin4) is High
    if (digitalRead(PIN_BTN_4) == LOW && (g_milli_time - g_pre_btn_timer_4 > debounce))
    {
        g_pre_btn_timer_4 = g_milli_time;
        g_state_btn_4 = !g_state_btn_4;
        g_count4++;
        digitalWrite(PIN_LED_9, g_state_btn_4);
    }
    // PD5 (pin5)
    if (digitalRead(PIN_BTN_5) == LOW && (g_milli_time - g_pre_btn_timer_5 > debounce))
    {
        g_pre_btn_timer_5 = g_milli_time;
        g_state_btn_5 = !g_state_btn_5;
        g_count5++;
        digitalWrite(PIN_LED_10, g_state_btn_5);
    }
    // PD6 (pin6)
    if (digitalRead(PIN_BTN_6) == LOW && (g_milli_time - g_pre_btn_timer_6 > debounce))
    {
        g_pre_btn_timer_6 = g_milli_time;
        g_state_btn_6 = !g_state_btn_6;
        g_count6++;
        digitalWrite(PIN_LED_11, g_state_btn_6);
    }
    sei();
}
