#include "drive.h"

int main()
{
	set_a_button_text("lightstart");
	set_b_button_text("no ls");
	set_c_button_text("testing");
	boolean l_s=false;//whether or not is doing light start
	WAIT(a_button()||b_button()||c_button());
	if(a_button())
	{
		printf("nyi...\n");
		l_s=true;
		reset_buttons();
		return -1;//remove this once we actually have lighstart...
		msleep(2000);
	}
	else if(c_button())
	{
		test_driving();
		return 9001;
	}//if b (no ls), does nothing-->l_s is already false
	reset_buttons();
	set_up();//only gets here if running real code
	if(l_s)
	{
		light_start(0);//get the right port eventually...
	}
	else//no light start
	{
		printf("press black button when ready\n");
		WAIT(side_button());
		printf("starting in 2 seconds...\n");
		msleep(2000);
	}
	ready_to_jump();
	time_drive(-90, -90, 3000);//jump!
	msleep(250);
	set_servo_position(TRIBBLE_ARM, TA_UP);//move the claw up while it's squaring up
	physical_squareup(false);//square up on the back
	time_drive(50, 50, 1000);//center on the middle connecter thingy
	right(90, 0, 40);//turn back to outside of field
	time_drive(-50, -50, 3000);//move towards edge
	physical_squareup(false);//square up on the outside of the field
	set_servo_position(TRIBBLE_ARM, TA_JUMP);//move arm down some so it can open the claw
	msleep(1000);
	set_servo_position(TRIBBLE_CLAW, TC_OPEN);//open the claw
	msleep(1000);
	set_servo_position(TRIBBLE_ARM, TA_DOWN);//drop the claw-->ready to go
	msleep(1000);
	return 42;
}
