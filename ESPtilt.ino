#include <Wire.h>
#define BLYNK_PRINT Serial
#include <Blynk.h>
#include <BlynkSimpleEsp32.h>
// #include <ESP8266WiFi.h>
// #include <BlynkSimpleEsp8266.h>

//#include <ESP8266_Lib.h>
// #include <BlynkSimpleShieldEsp8266.h>

#include <WiFiClientSecure.h>

// #define BLYNK_TEMPLATE_ID "TMPL3tH0o21Kf"
// #define BLYNK_TEMPLATE_NAME "Quickstart Template"

char auth[] = "aMQIyyJFNxdQZhvLM5hqfsJL_jY9c8BQ";  // You should get Auth Token in the Blynk App.
char ssid[] = "AndroidAP3ca6";                       // Your WiFi credentials. CSR_IOT_LAB
char pass[] = "Anindita@1997";                          //csr10t1ab1369

const int MPU_addr = 0x68;
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;

int minVal = 265;
int maxVal = 402;

double x;
double y;
double z;

// void myTimerEvent()
// {
//   // You can send any value at any time.
//   // Please don't send more that 10 values per second.
//   int angle_adc = analogRead(angle);
//   Serial.println("Angle ADC: " + String(angle_adc));
//   Blynk.virtualWrite(V1, angle_adc);

//   if (angle_adc < 90 &&
//       eventTrigger == false) {
//     eventTrigger = true;

//     Blynk.logEvent("angle", "Angle ADC is less than 90");
//   }
//   else if (angle_adc == 90) {
//     eventTrigger = false;
//   }
// }

void setup() {
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
}
void loop() {
  Blynk.run();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 14, true);
  AcX = Wire.read() << 8 | Wire.read();
  AcY = Wire.read() << 8 | Wire.read();
  AcZ = Wire.read() << 8 | Wire.read();
  int xAng = map(AcX, minVal, maxVal, -90, 90);
  int yAng = map(AcY, minVal, maxVal, -90, 90);
  int zAng = map(AcZ, minVal, maxVal, -90, 90);

  x = RAD_TO_DEG * (atan2(-yAng, -zAng) + PI);
  y = RAD_TO_DEG * (atan2(-xAng, -zAng) + PI);
  z = RAD_TO_DEG * (atan2(-yAng, -xAng) + PI);

  Serial.print("AngleX= ");
  Serial.println(x);

  Serial.print("AngleY= ");
  Serial.println(y);

  Serial.print("AngleZ= ");
  Serial.println(z);
  Serial.println("-----------------------------------------");

  Blynk.virtualWrite(V0, x);
  Blynk.virtualWrite(V1, y);
  Blynk.virtualWrite(V2, z);

  delay(3000);
}