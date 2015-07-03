#include "generic.h"
#include "createDrive.h"



int main()
{
	///// init /////
	printf("A for fast, B for medium, C for slow");
	int button = getabcbutton();
	printf("HELLO");
	create_connect();
	create_full();
	set_create_distance(0);
	enable_servos();
	set_servo_position(LID, LID_INIT);
	set_servo_position(SWEEPER, SWEEPER_INIT);
	
	///// light start /////
	light_start(LIGHT);
	
	///// go to caldera /////
	if (button == A)
	create_drive_direct(500, 500);
	else if (button == B)
	create_drive_direct(500,500);
	else if (button == C)
	create_drive_direct(300,300);
	
	///// bring the lid up with the two servos /////
	motor(HELPER_MOTOR, HELPER_MOTOR_UP_SPEED);
	set_servo_position(SWEEPER, SWEEPER_ALL);
	set_servo_position(LID, LID_ALL);
	msleep(400);
	set_servo_position(LID, LID_MID);
	msleep(400);
	if (button == C)
	{
		while ( get_create_distance() < (31 * 25.4) ); // SLOW
		create_stop();			
	}
	else
	{
		while ( get_create_distance() < (30 * 25.4) );
		create_stop();	
	}
	
	
	///// score and block /////
	if (button == A)
	{
		set_servo_position(LID, LID_DOWN);
		msleep(1500);
		sweep();
	}
	else if ( (button == B) )
	{
		servo_set(LID, LID_DOWN, 2);
		sweep();
	}
	else if( (button == C) )
	{
		sweep();
		servo_set(LID, LID_DOWN, 2);
	}
		
	
	create_stop();
	disable_servos();
	motor(IGUS, IGUS_OUT_SPEED);
	msleep(10000);
	ao();
	
}
