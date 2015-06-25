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
	start();//timing
	ready_to_jump();
	time_drive(-90, -90, 1300);//jump!
	set_servo_position(TRIBBLE_ARM, TA_UP);//move the claw up so it can square up
	msleep(500);
	physical_squareup(true);//square up on the front
	time_drive(-50, -50, 1000);//center on the middle connecter thingy
	right(85, 0, 40);//turn back to outside of field
	time_drive(-50, -50, 3000);//move towards edge
	physical_squareup(false);//square up on the outside of the field
	set_servo_position(TRIBBLE_ARM, TA_JUMP);//move arm down some so it can open the claw
	msleep(500);
	set_servo_position(TRIBBLE_CLAW, TC_PART_OPEN);//open the claw part way (to make sure it won't hit the edge)
	msleep(500);
	set_servo_position(TRIBBLE_ARM, TA_DOWN);//drop the claw
	msleep(500);
	set_servo_position(TRIBBLE_CLAW, TC_OPEN);//open the claw the rest of the way-->ready to go
	msleep(500);
	forward(8, 50);//move to block position
	set_servo_position(BLOCK_CLAW, BC_CLOSE);//drop the claw
	msleep(500);
	set_servo_position(BLOCK_ARM, BA_DOWN);//
	msleep(500);
	set_servo_position(BLOCK_CLAW, BC_OPEN);//
	msleep(500);
	time_drive(50,50,1000);//drive up to blocks
	servo_set(BLOCK_CLAW, BC_CLOSE,.75);//and pick them up
	msleep(250);
	set_servo_position(BLOCK_ARM, BA_LIFT);//get off the ground
	msleep(200);
	back(5, 50);//back away from the pipe
	servo_set(BLOCK_ARM, BA_UP,1);//drop in the basket
	msleep(500);
	servo_set(BLOCK_CLAW, BC_OPEN,.75);//
	msleep(300);
	set_servo_position(BLOCK_CLAW, BC_START);//needs to be in this position to drive-->very condensed
	forward(20, 50);//plow the tribbles!
	set_servo_position(TRIBBLE_CLAW, TC_CLOSE);//grab the tribbles
	forward(15, 50);//get across the center
	set_servo_position(TRIBBLE_CLAW, TC_OPEN);//back into plow position
	msleep(250);
	forward(20, 50);//get to dumping location
	set_servo_position(BLOCK_ARM, BA_DOWN);//get the block arm out of the way
	msleep(200);
	move_basket(BASKET_UP, true);//get it to the top
	hold_basket(BASKET_UP);//hold it there
	set_servo_position(BLOCK_ARM, BA_UP);//get the block arm back into driving position
	msleep(200);
	back(5,40);//dump the basket
	msleep(1000);//let the stuff drop out
	forward(6,50);//get back to the tribbles
	set_servo_position(BLOCK_ARM, BA_DOWN);//get the arm out of the way
	release_basket();//stop the pid loop for now
	move_basket(BASKET_DOWN, false);//bring it back
	set_servo_position(TRIBBLE_CLAW, TC_CLOSE);//grab the tribbles
	msleep(250);
	set_servo_position(TRIBBLE_ARM, TA_UP);//
	msleep(500);
	set_servo_position(TRIBBLE_CLAW, TC_OPEN);//
	msleep(1000);//this also lets the tribs fall out
	set_servo_position(TRIBBLE_CLAW, TC_PART_OPEN);//put the claw back down
	msleep(250);
	set_servo_position(TRIBBLE_ARM, TA_DOWN);//
	msleep(250);
	set_servo_position(TRIBBLE_CLAW, TC_OPEN);//
	msleep(250);
	move_basket(BASKET_UP, true);//raise the basket
	hold_basket(BASKET_UP);//and hold it there
	set_servo_position(BLOCK_ARM, BA_UP);//back into driving position
	back(8,40);//dump the basket
	msleep(1000);//let stuff drop
	forward(5,50);//move away from the caldera
	set_servo_position(BLOCK_ARM, BA_DOWN);//get out of the way
	release_basket();
	move_basket(BASKET_DOWN, false);//bring it back
	set_servo_position(BLOCK_ARM, BA_UP);//back to driving position
	return 42;
}
