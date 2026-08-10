#include <Servo.h>

Servo myServo;

const int trigPin = 11; 
const int echoPin = 10; 
const int servoPin = 9; 
const int ledPin = 13;   // تعريف بين الليد

long duration;
int distance;

int currentAngle = 0;
bool gateOpen = false;

unsigned long noObjectStart = 0;
bool waitingToClose = false;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT); // تفعيل بين الليد كمخرج

  myServo.attach(servoPin);
  myServo.write(currentAngle);
  digitalWrite(ledPin, LOW); // يبدأ الليد طافياً

  Serial.begin(9600);
}

void loop() {
  // إرسال نبضة الحساس
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH, 30000);

  if (duration == 0) {
    distance = 999;
  } else {
    distance = duration * 0.034 / 2;
  }

  // إذا اكتشف جسم على بعد 10 سم أو أقل
  if (distance > 0 && distance <= 10) {
    waitingToClose = false;

    if (!gateOpen) {
      digitalWrite(ledPin, HIGH); // يضيء الليد عند حركة السرفو
      for (int angle = currentAngle; angle <= 90; angle++) {
        myServo.write(angle);
        delay(20);
      }
      currentAngle = 90;
      gateOpen = true;
    }
  } 
  // إذا ابتعد الجسم
  else if (gateOpen) {
    if (!waitingToClose) {
      noObjectStart = millis();
      waitingToClose = true;
    }

    // ينتظر ثانيتين بعد ابتعاد الجسم ثم يعود للوضع الأصلي
    if (millis() - noObjectStart >= 2000) {
      for (int angle = currentAngle; angle >= 0; angle--) {
        myServo.write(angle);
        delay(20);
      }
      digitalWrite(ledPin, LOW); // يطفئ الليد عند إغلاق البوابة
      currentAngle = 0;
      gateOpen = false;
      waitingToClose = false;
    }
  }

  delay(100);
}
