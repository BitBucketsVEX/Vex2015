#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, dgtl1,  elevBottom,     sensorTouch)
#pragma config(Sensor, dgtl2,  autoJumper,     sensorTouch)
#pragma config(Sensor, I2C_1,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Sensor, I2C_2,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Motor,  port1,           gate,          tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port2,           frontRight,    tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port3,           backRight,     tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port4,           backLeft,      tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port5,           frontLeft,     tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port6,           rightLauncher, tmotorVex393_MC29, openLoop, reversed, encoderPort, I2C_1)
#pragma config(Motor,  port7,           leftLauncher,  tmotorVex393_MC29, openLoop, encoderPort, I2C_2)
#pragma config(Motor,  port8,           intakeLower,   tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port9,           intakeUpper,   tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port10,          cuteIntake,    tmotorVex393_HBridge, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#pragma platform(VEX)

/*+++++++++++++++++++++++++++++++++++++++++++++| Notes |++++++++++++++++++++++++++++++++++++++++++++++
CompAutonomous.c
- Basic driver control program for 2014 VEX omni drive base.
- The left joystick Y-axis controls the robot's forward and backward movement.
- The left joystick X-axis controls the robot's left and right movement.
- The right joystick X-axis controls the robot's rotation.
- Button 5 controls the grabber.
- Button 6 controls the elevator.
[I/O Port]          [Name]              [Type]                [Description]
Motor Port 1
Motor Port 2        frontRight          VEX Motor 393         Front Right motor
Motor Port 3        backRight           VEX Motor 393         Back Right motor
Motor Port 4        frontLeft           VEX Motor 393         Front Left motor
Motor Port 5        backLeft            VEX Motor 393         Back Left motor
Motor Port 6        rightLauncher       VEX Motor 393         Right Launcher primary motor
Motor Port 7        leftLauncher        VEX Motor 393         Left Launcher primary motor
Motor Port 8        intakeLower         VEX Motor 393         Lower intake motor
Motor Port 9				intakeUpper					VEX Motor 393					Upper intake motor
Motor Port 10
----------------------------------------------------------------------------------------------------*/

// TODO - Add closed-loop control code for drive motor control.

//Competition Control and Duration Settings
#pragma competitionControl(Competition)
#pragma autonomousDuration(20)    // This is longer than 15 seconds to include external timer padding.
#pragma userControlDuration(120)  // This is longer than 105 seconds to include external timer padding.

#include "Vex_Competition_Includes.c"   //Main competition background code...do not modify!
//#include "AutonomousFunctions.c"

#define ONE_POINT_AUTO true

//Global variables

int frontLeftMotorSpeed = 0;
int frontRightMotorSpeed = 0;
int backRightMotorSpeed = 0;
int backLeftMotorSpeed = 0;
int launcherSpeed = 0;
int lowerSpeed = 0;
int upperSpeed = 0;
int gateSpeed = 0;
int maxLauncherSpeed = 127;
int per = 200;
int prevPos = 0;
int currentPos = 0;
int kp = 2;
int kpden = 5;
int ki = 2;
int kiden = 10;
int kd = 1;
int kdden = 5;
int actualSpeed = 0;
int motorSpeed = 0;
int speedError = 0;
int prevError = 0;
int intError = 0;
int difError = 0;
int desiredSpeed;


task Pid1() {
	resetMotorEncoder(rightLauncher);
	while (true) {
		sleep(per);

		desiredSpeed = launcherSpeed;

		currentPos = nMotorEncoder[rightLauncher];
		actualSpeed = (currentPos - prevPos);
		speedError = (desiredSpeed - actualSpeed);
		intError += speedError;
		difError = prevError - speedError;
		// motor speed is 75 when motorSpeed = 127
		motorSpeed = (speedError * kp / kpden) + (intError * ki / kiden) + (difError * kd / kdden);
		//motorSpeed = 127;
		prevPos = currentPos;
		prevError = speedError;
		if (motorSpeed > 127) {
			motorSpeed = 127;
		}
		if (motorSpeed < 0) {
			motorSpeed = 0;
		}
		motor[rightLauncher] = motorSpeed;
	}
}

task Pid2() {
	resetMotorEncoder(leftLauncher);
	while (true) {
		sleep(per);

		desiredSpeed = launcherSpeed;

		currentPos = nMotorEncoder[leftLauncher];
		actualSpeed = (currentPos - prevPos);
		speedError = (desiredSpeed - actualSpeed);
		intError += speedError;
		difError = prevError - speedError;
		// motor speed is 75 when motorSpeed = 127
		motorSpeed = (speedError * kp / kpden) + (intError * ki / kiden) + (difError * kd / kdden);
		//motorSpeed = 127;
		prevPos = currentPos;
		prevError = speedError;
		if (motorSpeed > 127) {
			motorSpeed = 127;
		}
		if (motorSpeed < 0) {
			motorSpeed = 0;
		}
		motor[leftLauncher] = motorSpeed;
	}
}

// All activities that occur before the competition starts
// Example: clearing encoders, setting servo positions, ...
void pre_auton()
{
	// Set bStopTasksBetweenModes to false if you want to keep user created tasks running between
	// Autonomous and Tele-Op modes. You will need to manage all user created tasks if set to false.
	bStopTasksBetweenModes = true;

}

// Task for the autonomous portion of the competition.
task autonomous()
{
	// because this comment is useful :P
}

// Task for the driver controlled portion of the competition.
task usercontrol()
{
	startTask(Pid1);
	startTask(Pid2);

	while (true)
	{

		sleep(20);

		// Drive commands.
		frontRightMotorSpeed = - vexRT[Ch3] + vexRT[Ch4] + vexRT[Ch1];
		backRightMotorSpeed = - vexRT[Ch3]  - vexRT[Ch4] + vexRT[Ch1];
		frontLeftMotorSpeed =  vexRT[Ch3] + vexRT[Ch4] + vexRT[Ch1];
		backLeftMotorSpeed = vexRT[Ch3] - vexRT[Ch4] + vexRT[Ch1];
		motor[frontRight] = frontRightMotorSpeed;
		motor[backRight] = backRightMotorSpeed;
		motor[frontLeft] = frontLeftMotorSpeed;
		motor[backLeft] = backLeftMotorSpeed;

		// Intake control
		upperSpeed = 0;
		lowerSpeed = 0;
		gateSpeed = 0;
		if (vexRT[Btn6U] == 1) {  // run both intake motors up when button 6 up pressed
			lowerSpeed = 127;
			if (launcherSpeed == maxLauncherSpeed) {
				upperSpeed = 127;
			}
		} else if (vexRT[Btn6D] == 1){ // both intake motors up when button 6D pressed
			lowerSpeed = -127;
			upperSpeed = -127;
		}
		if (vexRT[Btn8UXmtr2] == 1){
			gateSpeed = 127;
		}
		if (vexRT[Btn8DXmtr2] == 1){	
			gateSpeed = -127;
		}
			
		motor[gate] = gateSpeed;

		// Individual intake control
		if (vexRT[Btn5U] == 1) {
			upperSpeed = 127;  // upper intake runs up
		} else if (vexRT[Btn5D] == 1) {
			lowerSpeed = 127;  // lower intake runs up
		}
		motor[intakeLower] = lowerSpeed;
		motor[intakeUpper] = upperSpeed;
		motor[cuteIntake] = lowerSpeed;

		// Launcher
		if (vexRT[Btn6UXmtr2] == 1 && launcherSpeed < maxLauncherSpeed) {
			launcherSpeed++; // add 1 to the launcher speed
		} else if (launcherSpeed > 0 && vexRT[Btn6UXmtr2] == 0) {
			launcherSpeed--; // subtract 1 to launcher speed
		}
		//motor[leftLauncher] = launcherSpeed;
		//motor[rightLauncher] = launcherSpeed;
		// now done by pid loops
	}
}
