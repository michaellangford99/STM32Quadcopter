#include "Wprogram.h"
#include "Debug.h"
#include "unit_tests.h"
#include "Motors.h"
#ifdef TEST_MOTORS

void setup() {
    init_motors();

    Serial.begin(9600);
	Serial.clear();
}

String s;

void loop() {
    Serial.setTimeout(100000);
    s = Serial.readStringUntil(',');
    float a = s.toFloat();
    Serial.printf("a=%f\n", a);

    s = Serial.readStringUntil(',');
    float b = s.toFloat();
    Serial.printf("b=%f\n", b);

    s = Serial.readStringUntil(',');
    float c = s.toFloat();
    Serial.printf("c=%f\n", c);

    s = Serial.readStringUntil(',');
    float d = s.toFloat();
    Serial.printf("d=%f\n", d);

    Serial.printf("%f, %f, %f, %f\n", a, b, c, d);   

    update_motors(a, b, c, d); 
}

#endif