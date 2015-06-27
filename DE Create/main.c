#include "generic.h"
#include "createDrive.h"

#define LID 0
#define LID_UP 760
#define LID_INIT 500
#define LID_MID 1100
#define LID_ALL 2040
#define LID_DOWN 1700

#define SWEEPER 2
// in = score
#define SWEEPER_IN 0
#define SWEEPER_OUT 1000
#define SWEEPER_INIT 500

#define HELPER_MOTOR 0
#define HELPER_MOTOR_UP_SPEED 100
#define HELPER_MOTOR_DOWN_SPEED -100



int main()
{
	create_connect();
	enable_servos();
	set_servo_position(LID, LID_INIT);
	set_servo_position(SWEEPER, SWEEPER_INIT);
	msleep(100);
	
	create_drive_direct(490,500);
	
	// bring the lid up with the two servos
	
	set_servo_position(SWEEPER, SWEEPER_OUT);
	motor(HELPER_MOTOR, HELPER_MOTOR_UP_SPEED);
	msleep(800);
	//set_servo_position(LID, LID_MID);
	msleep(1300);
	
	
	create_stop();
	ao();
	set_servo_position(LID, LID_DOWN);
	msleep(1500);
	
	servo_set(SWEEPER,SWEEPER_IN, 1);
	servo_set(SWEEPER,SWEEPER_OUT, 1);
	servo_set(SWEEPER,SWEEPER_IN, 1);
	
	disable_servos();
	
}
