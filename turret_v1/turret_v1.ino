// #include <Servo.h>

#include <Arduino_APDS9960.h>
#include <Arduino_HTS221.h>
#include <Arduino_LPS22HB.h>
#include <Arduino_LSM9DS1.h>
#include <PDM.h>

char axis[][2] = {"X", "Y", "Z"};

// Servo servoX;
const int servoRangeX = 180;
int servoPosX = servoRangeX / 2;
int servoOffsetX = 0; // Offset based on the position of the arduino board, used to set it manually to 0

//* Logs a title and it's value or a value and it's type, very ez + beautiful spacer
void cs() { Serial.print(" - "); }; void clog(String title, int val = false, bool spacer = false) { Serial.print(title); if (val) { Serial.print(": "); Serial.print(val); } if (spacer) { cs(); } }; void cl(int val, String type, bool spacer = false) { Serial.print(val); Serial.print(type); if (spacer) { cs(); } }; void clr() { Serial.println(); };

// void setPosition(int pos) { servoX.write(pos); };

void setup() {
  Serial.begin(9600);

  clog("wassup");
  clr();
  
  // servo.attach(5);

  if (!IMU.begin()) {
      Serial.println("IMU init failed");
      while (1) {}
  }

  // setPosition(pos);
  Serial.println("Good luck");
}


void loop() {

  bool accelAvailable;
  float accelX = -1;
  float accelY = -1;
  float accelZ = -1;
  if ((accelAvailable = IMU.accelerationAvailable())) {
      IMU.readAcceleration(accelX, accelY, accelZ);
  }

  // bool gyroAvailable;
  // float gyroX = -1;
  // float gyroY = -1;
  // float gyroZ = -1;
  // if ((gyroAvailable = IMU.gyroscopeAvailable())) {
  //     IMU.readGyroscope(gyroX, gyroY, gyroZ);
  // }

  // bool magAvailable;
  // float magX = -1;
  // float magY = -1;
  // float magZ = -1;
  // if ((magAvailable = IMU.magneticFieldAvailable())) {
  //     IMU.readMagneticField(magX, magY, magZ);
  // }
  
  // clog("aX", accelX, true);
  // clog("GX", gyroX, true);
  // clog("mgX", magX, true);
  // clr();
  
  // clog("aY", accelY, true);
  // clog("GY", gyroY, true);
  // clog("mgY", magY, true);
  // clr();
  
  // clog("aZ", accelZ, true);
  // clog("GZ", gyroZ, true);
  // clog("mgZ", magZ, true);
  // clr();

  // magX = up +0 / down -0 

  double roll = 0.00, pitch = 0.00;
  int rollRange = 150 * 2, pitchRange = 50 * 2; // Maximum distance on one side, *2 cause 2 sides ukwim
  double x_Buff = float(accelX);
  double y_Buff = float(accelY);
  double z_Buff = float(accelZ);
  roll = atan2(y_Buff , z_Buff) * 57.3;
  pitch = atan2((- x_Buff) , sqrt(y_Buff * y_Buff + z_Buff * z_Buff)) * 57.3;

  // clog("Roll", roll);
  // clr();

  // clog("Pitch", pitch);
  // clr();

  // pitch Y (pivot)  = up   / -1..    0    1..    / down
  // roll  X (rotate) = left / -179.. -180 -181..  / right

  if (roll > rollRange) {
    roll = rollRange;
  }

  if (roll < -rollRange) {
    roll = -rollRange;
  }

  if (!servoOffsetX) {
    servoOffsetX = roll;
  }


  // Spoilers: it doesn't work
  const int servoPosX = ((roll - servoOffsetX) / rollRange) * servoRangeX; // Converts the roll value into servo position (pls work im so tired)

  clog("ServoPosX", servoPosX);
  clr();





  const int d = 500;
  delay(d);
}

// void _loop() {
//   const int x = analogRead(joyX);
//   const int y = analogRead(joyY);

//   clog("JoyX", x, true);
//   clog("JoyY", y);
//   clr();

//   if (x < 300 && pos > 2) {
//     servo.write(pos -= 2);
//   }

//   if (x > 700 && pos < 178) {
//     servo.write(pos += 2);
//   }
// }