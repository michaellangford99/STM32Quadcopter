#include "Wprogram.h"
#include "Debug.h"
#include "unit_tests.h"
#ifdef TEST_DEBUG_LED

/*void test_led_state_high(void) {
    digitalWrite(LED_BUILTIN, HIGH);
    TEST_ASSERT_EQUAL(HIGH, digitalRead(LED_BUILTIN));
}

void test_led_state_low(void) {
    digitalWrite(LED_BUILTIN, LOW);
    TEST_ASSERT_EQUAL(LOW, digitalRead(LED_BUILTIN));
}*/

void setup() {
    // NOTE!!! Wait for >2 secs
    // if board doesn't support software reset via Serial.DTR/RTS
    delay(2000);

    /*UNITY_BEGIN();    // IMPORTANT LINE!    
    RUN_TEST(test_led_builtin_pin_number);*/

    init_Debug();
}

void loop() {
    update_Debug_SAFE();

    delay(500);
    
    update_Debug_ARMED();

    delay(500);
}

#endif