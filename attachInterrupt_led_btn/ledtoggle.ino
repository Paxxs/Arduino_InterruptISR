// 定义要使用的引脚
#define LED_PIN 9
#define BTN_PIN 2

volatile bool btn_state_2 = 0;
volatile unsigned int btn_count;

// 抖动计算
volatile unsigned long ignore_time;
#define debounce 250 // time to wait in milli secs

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
    Serial.println(F("Delay 1s"));
    Serial.print(F(" "));
    Serial.print("Btn:");
    Serial.print(digitalRead(BTN_PIN));
    Serial.print(F("-"));
    Serial.print(btn_state_2);
    Serial.print(F("-"));
    Serial.print(btn_count);
    Serial.print(F(" "));
    Serial.print(ignore_time);
    Serial.print(F(" "));
    Serial.println(millis());
    Serial.println("----------");
    delay(1000);
}

void CheckBtnIsr()
{
    // ISR 中使用 millis() 函数获取到的值不会改变
    // ISR 函数，更改 led 状态
    if (ignore_time > millis()) // 跳过抖动时间
        return;
    if (digitalRead(BTN_PIN) == HIGH)
    {
        ignore_time = millis() + debounce; // reset timer
        btn_state_2 = !btn_state_2;
        digitalWrite(LED_PIN, btn_state_2);
        btn_count++;
    }
}