/// MKS MINI FOC Open loop speed control example; Test Library：SimpleFOC 2.1.1 ; Tested hardware：MKS MINI FOC V1.0
/// Enter "T+number" in the serial port to set the speed of the two motors.For example, to set the motor to rotate at a speed of 10rad/s, input "T10"
/// When the motor is powered on, it will rotate at 5rad/s by default
/// When using your own motor, do remember to modify the default number of pole pairs, the value in BLDCMotor(7).
/// The default power supply voltage of the program is 12V.
/// Please remember to modify the voltage_power_supply , voltage_limit variable values when using other voltages for power supply

#define SIMPLEFOC_DISABLE_ATMEGA2560_MCU_WARNING 
/* 
eliminate warnning of below :
 "note: #pragma message: SimpleFOC: compiling for Arduino/ATmega2560 or Arduino/ATmega1280
 #pragma message("SimpleFOC: compiling for Arduino/ATmega2560 or Arduino/ATmega1280")"
 */
 
#define EN  2  // 驱动器使能
#define IN1 3  // U相 (PWM支持)
#define IN2 4  // V相 
#define IN3 5  // W相 (PWM支持)

#define  NumPolePairs 2 //12个定子线圈

#include <SimpleFOC.h>

BLDCMotor motor = BLDCMotor(NumPolePairs);                               //According to the selected motor, modify the number of pole pairs here, the value in BLDCMotor()
BLDCDriver3PWM driver = BLDCDriver3PWM(IN1,IN2,IN3,EN);
 
/// Target Variable
float target_velocity = 5;

/// Serial Command Setting
Commander command = Commander(Serial);
void doTarget(char* cmd) { command.scalar(&target_velocity, cmd); }

void setup() {
  
  driver.voltage_power_supply = 12 ; //24 ;                   //According to the supply voltage, modify the value of voltage_power_supply here
  driver.init();
  motor.linkDriver(&driver);
  motor.voltage_limit = driver.voltage_power_supply;   // [V]                   //According to the supply voltage, modify the value of voltage_limit here
  motor.velocity_limit = 160; // [rad/s]
   
  // Open Loop Control Mode Setting
  motor.controller = MotionControlType::velocity_openloop;
  
  // Initialize the Hardware
  motor.init();

  // Add T Command
  // Enter "T+number" in the serial port to set the speed of the two motors.For example, to set the motor to rotate at a speed of 10rad/s, input "T10".
  command.add('T', doTarget, "target velocity");

  Serial.begin(115200);
  Serial.println("Motor ready!");
  Serial.println("Set target velocity [rad/s]");
  _delay(1000);
}

void loop() {
  motor.move(target_velocity);                    //When the motor is powered on, it will rotate at 5rad/s by default
  
  //User Newsletter
  command.run();
}
