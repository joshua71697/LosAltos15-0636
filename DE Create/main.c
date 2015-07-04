#include "generic.h"
#include "createDrive.h"



int main()
{
	///// init /////
	set_a_button_text("FAST");
	set_b_button_text("MEDIUM");
	set_c_button_text("SLOW");
	int button = getabcbutton();
	printf("\n\nStrategy: %d", button);
	create_connect();
	create_full();
	set_create_distance(0);
	set_servo_position(LID, LID_INIT);
	set_servo_position(SWEEPER, SWEEPER_INIT);
	
	///// light start /////
	light_start(LIGHT);
	shut_down_in(119.5);
	
	///// go to caldera /////
	if (button == A)
	create_drive_direct(490, 500);
	else if (button == B)
	create_drive_direct(490,500);
	else if (button == C)
	create_drive_direct(300,300);
	enable_servos();
	
	///// bring the lid up /////
	motor(HELPER_MOTOR, HELPER_MOTOR_UP_SPEED);
	set_servo_position(SWEEPER, SWEEPER_ALL);
	set_servo_position(LID, LID_ALL);
	msleep(600);
	set_servo_position(LID, LID_MID);
	msleep(00);
	
	///// stop the create /////
	if (button == A)
		while ( get_create_distance() < (30 * 25.4) );
	else if (button == B)
		while ( get_create_distance() < (31 * 25.4) );
	else if (button == C)
		while ( get_create_distance() < (33 * 25.4) ); // SLOW
	create_stop();

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
		create_left(1,1,300);
		create_block();
		singleSweep(.8);
		servo_set(LID, LID_DOWN, 2);
		wideShake();
		sweep(5);
		sweep(5);
	}
	else if( (button == C) )
	{
		sweep(5);
		sweep(5);
		servo_set(LID, LID_DOWN, 2);
		wideShake();
		sweep(5);
		
	}	
	
	///// end and hope /////
	create_stop();
	disable_servos();
	thread_destroy(rollIgus);
	ao();
	
}
