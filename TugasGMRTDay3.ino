#include <ESP32Servo.h> //deklarasi library servo esp32
#include <Adafruit_MPU6050.h> //deklarasi library mpu
#include <Adafruit_Sensor.h> //deklarasi library sensor
#include <Wire.h> //deklarasi library wire h

//deklarasi objek-objek
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;
Adafruit_MPU6050 mpu;

//deklarasi pin dan value
#define SERVO1_PIN 14
#define SERVO2_PIN 12
#define SERVO3_PIN 26
#define SERVO4_PIN 25
#define SERVO5_PIN 33
#define SDA_PIN 21
#define SCL_PIN 22
#define PIR_PIN 27


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN); //Mengaktifkan komunikasi I2c antara ESP32 dan sensor MPU.
  pinMode(PIR_PIN,INPUT); //menyatakan bahwa pin sensor pir akan menerima input

  //menghubungkan servo dengan pin-pinnya
  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);
  servo3.attach(SERVO3_PIN);
  servo4.attach(SERVO4_PIN);
  servo5.attach(SERVO5_PIN);

  while (!mpu.begin()) { //kalau misal tidak terdekesi MPUnya
    Serial.println("MPU6050-nya dereng konek");
    delay(1000);
  }
  Serial.println("MPU6050 Sampun siap");//kalau sudah terdeteksiMPUnya
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G); //Mengatur range sensitivitas akselerometer di dalam sensor MPU.
  mpu.setGyroRange(MPU6050_RANGE_500_DEG); //Mengatur range kecepatan rotasi maksimum (range gyronya lah).
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ); //Mengaturbandwidth untuk low-pass filter (LPF) internal sensor supaya bisa menyaring noise data sensor agar lebih halus.
  Serial.print("Yosh semua ready king");
  delay(1000);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  long long cekpir = digitalRead(PIR_PIN); //menyatakan variabel cekpir yang membaca digital pin sensor PIRnya

  sensors_event_t a, g; //Deklarasi tiga struct a(akselerometer), g(gyroscope) yang masing-masing berisi data sensor hasil pembacaan dari MPU.
  mpu.getEvent(&a, &g);

  //deklarasi sumbu roll,pitch,yaw dari akselerasi tiap sumbu
  double roll = a.acceleration.x;
  double pitch = a.acceleration.y;
  double yaw = a.acceleration.z;

  //mengoutput pembacaan gerak tiap sumbu beserta PIR sensor
  Serial.print("Roll: "); Serial.print(roll);
  Serial.print("Pitch: "); Serial.print(pitch);
  Serial.print("Yaw: "); Serial.print(yaw);
  Serial.print("PIR: "); Serial.println(cekpir);

  //sensor Roll (Servo 1 & 2)
  if (roll > 1.0) { // rotasi ke arah positif
    servo1.write(0-90); 
    servo2.write(0-90);
  } 
  else if (roll < -1.0) { // rotasi ke arah negatif
    servo1.write(90);
    servo2.write(90);
  } 
  else {
    servo1.write(0);
    servo2.write(0);
  }

  // sensorPitch (Servo 3 & 4)
  if (pitch > 1.0) { // rotasi ke arah positif
    servo3.write(90);
    servo4.write(90);
  } 
  else if (pitch < -1.0) { // rotasi ke arah negatif
    servo3.write(0-90);
    servo4.write(0-90);
  } 
  else {
    servo3.write(0);
    servo4.write(0);
  }

  // Sensor Yaw (Servo 5)
  if (abs(yaw) > 1.0) {
    if (yaw > 0.00) servo5.write(45);
    else servo5.write(0-45);
    delay(1000); //diam 1 detik
    servo5.write(0); // kembali ke posisi awal
  }

  // sensor external dari PIR (semua servo)
  if (cekpir == HIGH) {
    Serial.println("Gerakan terdeteksi! Semua servo bergerak bersamaan.");
    for (int i = 1; i <= 5; i++) {
      //semuya servo bergerak serentak
      if(i == 1)servo1.write(60);
      else if(i == 2)servo2.write(60); 
      else if(i == 3)servo3.write(60);
      else if(i == 4)servo4.write(60);
      else if(i == 5)servo5.write(60);
    }
    delay(1000);//tunggu sedetik
    // Kembali ke posisi awal
    servo1.write(0);
    servo2.write(0);
    servo3.write(0);
    servo4.write(0);
    servo5.write(0);
  }

  delay(1000);
  
}
