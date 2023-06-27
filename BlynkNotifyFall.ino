/************************************************************************************

 *  My Channel:https://www.youtube.com/c/ImpulseTech/featured Visit for More Project Videos
 *  
 *  *********************************************************************************
 *  Preferences--> Aditional boards Manager URLs : 
 *  For ESP32:
 *  https://dl.espressif.com/dl/package_esp32_index.json
 *  https://blynk.cloud/external/api/logEvent?token=YourAuthToken&code=hello for loging the events using rest API
 *  You can send only 100 events per devices per day
    When the limit is reached you'll see the notification on the UI in the Device Timeline section
    The maximum description length for the event is 300 chars
 *  *********************************************************************************/
#include <Wire.h>
#include <Blynk.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <WiFiClientSecure.h>

#define BLYNK_PRINT Serial
// #define BLYNK_TEMPLATE_ID "TMPL3tH0o21Kf"
// #define BLYNK_TEMPLATE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "aMQIyyJFNxdQZhvLM5hqfsJL_jY9c8BQ"

// #include <WiFi.h>
#include <WiFiClient.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>


Adafruit_MPU6050 mpu;

char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "POCO M5 Pro"; //roomies406, CSR_IOT_LAB, AndroidAP3ca6
char pass[] = "12345678";   //Kru@1234 , csr10t1ab1369, Anindita@1997

const int MPU_addr = 0x68;
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;

int minVal = 265;
int maxVal = 402;

double x;
double y;
double z;

BlynkTimer timer;

void sendSensor() {
  // {
  //   if(mpu.getMotionInterruptStatus()) {
  //     /* Get new sensor events with the readings */
  //     sensors_event_t a, g, temp;
  //     mpu.getEvent(&a, &g, &temp);

  //     /* Print out the values */
  //     // Serial.print("AccelX:");
  //     // Serial.print(a.acceleration.x);
  //     // Serial.print(",");
  //     // Serial.print("AccelY:");
  //     // Serial.print(a.acceleration.y);
  //     // Serial.print(",");
  //     // Serial.print("AccelZ:");
  //     // Serial.print(a.acceleration.z);
  //     // Serial.print(", ");
  //     Serial.print("GyroX:");
  //     Serial.print(g.gyro.x);
  //     Serial.print(",");
  //     Serial.print("GyroY:");
  //     Serial.print(g.gyro.y);
  //     Serial.print(",");
  //     Serial.print("GyroZ:");
  //     Serial.print(g.gyro.z);
  //     Serial.println(",");
  //   // You can send any value at any time.
  //   // Please don't send more that 10 values per second.
  //     // Blynk.virtualWrite(V0, a.acceleration.x);
  //     // Blynk.virtualWrite(V1, a.acceleration.y);
  //     // Blynk.virtualWrite(V2, a.acceleration.z);
  //     Blynk.virtualWrite(V0,g.gyro.x);
  //     Blynk.virtualWrite(V1,g.gyro.y);
  //     Blynk.virtualWrite(V2,g.gyro.z);
  //    delay(200);
  //   }
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
  //Serial.println("-----------------------------------------");

  Blynk.virtualWrite(V0, x);
  Blynk.virtualWrite(V1, y);
  Blynk.virtualWrite(V2, z);

  delay(1500);
}

void setup() {

  Serial.begin(115200);
  while (!Serial)
    delay(10);  // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit MPU6050 test!");

  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  //setupt motion detection
  mpu.setHighPassFilter(MPU6050_HIGHPASS_0_63_HZ);
  mpu.setMotionDetectionThreshold(1);
  mpu.setMotionDetectionDuration(20);
  mpu.setInterruptPinLatch(true);  // Keep it latched.  Will turn off when reinitialized.
  mpu.setInterruptPinPolarity(true);
  mpu.setMotionInterrupt(true);

  Serial.println("");
  delay(500);

  Blynk.begin(auth, ssid, pass);
  timer.setInterval(100L, sendSensor);
}

void loop() {
  Blynk.run();
  timer.run();
}