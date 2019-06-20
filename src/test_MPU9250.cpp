#include "Wprogram.h"
#include "MPU9250.h"

#include "unit_tests.h"
#ifdef TEST_MPU9250

void setup() {
    Serial.begin(9600);
	Serial.clear();

    init_MPU9250();

    delay(500);
}

void loop() {
    update_MPU9250();
}

#endif