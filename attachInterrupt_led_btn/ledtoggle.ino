
// 定义要使用的引脚
#define LED_PIN 9
#define BTN_PIN 2

// 抖动计算
float pre_btn_time_2;
volatile bool btn_state_2 = 0;
#define debounce 100 // time to wait in milli secs

void setup()
{
    noInterrupts(); // alias cli();

    Serial.begin(9600);
    // 设置 led pin 为输出
    pinMode(LED_PIN, OUTPUT);
    // 设置 button pin 为上拉
    pinMode(BTN_PIN, INPUT_PULLUP);
    // 配置硬件中断
    attachInterrupt(digitalPinToInterrupt(BTN_PIN), CheckBtnIsr, RISING);

    interrupts(); // alias sei();
}

void loop()
{
    Serial.println(F("Delay 3s"));
    Serial.print(F(" "));
    Serial.print("Btn:");
    Serial.print(digitalRead(BTN_PIN));
    Serial.print(F("-"));
    Serial.print(btn_state_2);
    Serial.print(F(" "));
    Serial.println(pre_btn_time_2);
    delay(3000);
    Serial.println("----------");
}

void CheckBtnIsr()
{
    cli();
    // ISR 中使用 millis() 函数获取到的值不会改变
    // ISR 函数，更改 led 状态
    if (digitalRead(BTN_PIN) == HIGH)
    {
        if (btn_state_2 == 0) // 按钮为关
        {
            pre_btn_time_2 = millis(); // 重置计时器
            btn_state_2 = 1;
        }
        else if (btn_state_2 == 1 && (millis() - pre_btn_time_2 > debounce))
            btn_state_2 = 0;
        digitalWrite(LED_PIN, btn_state_2);
    }
    sei();
}
