#include "main.h"
#include "cmsis_os.h"
#include "FreeRTOS.h"
#include "task.h"
#include "SimpleFOC.h"
//#include "usart.h"
//#include "MagneticSensorSPI.h"

BLDCMotor motor = BLDCMotor(7);
BLDCDriver3PWM driver = BLDCDriver3PWM(Gpio_pin2Index(GPIOA, GPIO_PIN_8), Gpio_pin2Index(GPIOA, GPIO_PIN_9),Gpio_pin2Index(GPIOA, GPIO_PIN_10), Gpio_pin2Index(GPIOC, GPIO_PIN_7));
//MagneticSensorSPI sensor = MagneticSensorSPI(AS5147_SPI, Gpio_pin2Index(GPIOG, GPIO_PIN_11));

void setupSimpleFOC() {
	// driver config
	// power supply voltage [V]
	driver.voltage_power_supply = 12;
	driver.init();
	// link the motor and the driver
	motor.linkDriver(&driver);

	// set motion control loop to be used
	motor.controller = MotionControlType::velocity_openloop;

	// contoller configuration
	// default parameters in defaults.h

	// velocity PI controller parameters
	motor.PID_velocity.P = 1;
	motor.PID_velocity.I = 20;
	motor.PID_velocity.D = 0;
	// default voltage_power_supply
	motor.voltage_limit = 6;
	// jerk control using voltage voltage ramp
	// default value is 300 volts per sec  ~ 0.3V per millisecond
	motor.PID_velocity.output_ramp = 1000;

	// velocity low pass filtering
	// default 5ms - try different values to see what is the best.
	// the lower the less filtered
	motor.LPF_velocity.Tf = 0.01;

	// initialize motor
	motor.init();
	// align sensor and start FOC
	motor.initFOC();
}

float target_velocity = 1; // [rad/s]

void loopSimpleFOC() {
	motor.move(target_velocity);
}
void disableSimpleFOC(int s) {
	if(s == 1)
		motor.enable();
	else motor.disable();
}
void setTarget(float t){
	target_velocity = t;
}


