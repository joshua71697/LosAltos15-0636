#include "generic.h"
#include "createDrive.h"

int main()
{
	///// init /////
	set_a_button_text("FAST");
	set_b_button_text("MEDIUM");
	set_c_button_text("EARLY DEPLOY");
	printf(" ... and side button is frisbee");
	int button = getbutton();
	printf("\n\nStrategy: %d", button);
	create_connect();
	+---------------create_full();
	set_create_distance(0);
	set_servo_position(LID, LID_INIT);
	set_servo_position(SWEEPER, SWEEPER_INIT);
	
	///// light start /////
	light_start(LIGHT);
	shut_down_in(119.5);
	
	///// go to caldera /////
	if (button == A)
	create_drive_direct(500, 500);
	else if ( (button == B) || (button == C) || (button == S) )
	create_drive_direct(500,500);
	
//360nosecop

	enable_servos();
	
	///// bring the lid up /////
	motor(HELPER_MOTOR, HELPER_MOTOR_UP_SPEED);
	motor(STRING, STRING_UP);
	set_servo_position(SWEEPER, SWEEPER_ALL);
	set_servo_position(LID, LID_ALL);
	msleep(300);
	set_servo_position(LID, LID_MID);
	msleep(100);
	
	///// stop the create /////
	if (button == A)
		while ( get_create_distance() < (30 * 25.4) );
	else if ( (button == B) || (button == S) )
		while ( get_create_distance() < (30 * 25.4) );
	else if (button == C)
		while ( get_create_distance() < (25 * 25.4) ); // much shorter
	
	create_stop();
	ao();
	thread rollIgus = thread_create(roll);
	thread_start(rollIgus);
	
	///// score and block /////
	if (button == A)
	{
		set_servo_position(LID, LID_DOWN);
		msleep(1000);
		sweep(5);
		sweep(5);
	}
	else if ( (button == B) )
	{
		singleSweep(.8);
		servo_set(LID, LID_DOWN, 2);
		// wideShake();
		sweep(5);
		sweep(5);
		
	}
	else if( (button == C) )
	{
		msleep(2000); // give time for the igus.
		simple_forward(500, 5);
		servo_set(LID, LID_DOWN, 2);
		// wideShake();
		sweep(5);
		sweep(5);
		servo_set(LID, LID_DOWN, 2);
		wideShake();
		sweep(5);
		
	}	
	else if (button == S)
	{
		servo_set(LID, LID_ALL_UP, 1);
		servo_set(LID, LID_DOWN, 1);
		
		
	}
	
	///// end and hope /////
	create_stop();
	set_servo_position(SWEEPER, SWEEPER_ALL_IN);
	msleep(1000);
	disable_servos();
	thread_destroy(rollIgus);
	ao();
	
}
