#include "drive.h"

int main()
{
	thread holding=thread_create(basket_pid);//prepare the holding thread
	thread_start(holding);//-->nothing happens until basket_hold is called
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
	servo_set(TRIBBLE_ARM, TA_UP,.1);//move the claw up so it can square up
	physical_squareup(true);//square up on the front
	right(-90, 4, 50);//sweep tribbles out of the way, get to the center
	msleep(500);
	time_drive(50, 50, 1000);//get away from the tribbles
	right(180, 0, 50);//turn around
	time_drive(-50, -50, 1500);//move towards edge
	physical_squareup(false);//square up on the outside of the field
	servo_set(TRIBBLE_ARM, TA_JUMP, .1);//move arm down some so it can open the claw
	servo_set(TRIBBLE_CLAW, TC_PART_OPEN, .1);//open the claw part way (to make sure it won't hit the edge)
	servo_set(TRIBBLE_ARM, TA_DOWN, .1);//drop the claw
	servo_set(TRIBBLE_CLAW, TC_OPEN, .1);//open the claw the rest of the way-->ready to go
	forward(6, 60);//move to block position
	servo_set(BLOCK_CLAW, BC_CLOSE, .1);//drop the claw
	servo_set(BLOCK_ARM, BA_DOWN, .1);//
	servo_set(BLOCK_CLAW, BC_OPEN, .1);//
	time_drive(50,50,1200);//drive up to blocks
	servo_set(BLOCK_CLAW, BC_CLOSE,.75);//and pick them up
	msleep(250);
	servo_set(BLOCK_ARM, BA_LIFT, .1);//get off the ground
	back(5, 60);//back away from the pipe
	servo_set(TRIBBLE_ARM, TA_JUMP, .5);//bring the ta up to keep the blocks in the basket
	servo_set(TRIBBLE_CLAW, TC_CLOSE, .5);//
	servo_set(TRIBBLE_ARM, TA_UP, .5);//
	servo_set(BLOCK_ARM, BA_UP,1);//drop in the basket
	msleep(500);
	servo_set(BLOCK_CLAW, BC_OPEN,.75);//
	servo_set(TRIBBLE_CLAW, TC_PART_OPEN, .3);//put the claw back down
	servo_set(TRIBBLE_ARM, TA_DOWN, .2);//
	servo_set(TRIBBLE_CLAW, TC_OPEN, .2);//
	servo_set(BLOCK_CLAW, BC_START, .1);//needs to be in this position to drive-->very condensed
	forward(20, 60);//plow the tribbles!
	servo_set(TRIBBLE_CLAW, TC_CLOSE, .4);//grab the tribbles
	forward(18, 60);//get across the center
	servo_set(TRIBBLE_CLAW, TC_OPEN, .4);//back into plow position
	msleep(250);
	forward(12, 58);//get to dumping location
	servo_set(BLOCK_ARM, BA_MID, .1);//get the block arm out of the way
	move_basket(BASKET_UP, true);//get it to the top
	hold_basket(BASKET_UP);//hold it there
	servo_set(BLOCK_ARM, BA_UP,.1);//get the block arm back into driving position
	time_drive(-40, -40, 2000);//dump the basket
	msleep(1000);//let the stuff drop out
	forward(6,60);//get back to the tribbles
	servo_set(BLOCK_ARM, BA_MID, .1);//get the arm out of the way
	release_basket();//stop the pid loop for now
	move_basket(BASKET_DOWN, false);//bring it back
	servo_set(TRIBBLE_CLAW, TC_CLOSE, .4);//grab the tribbles
	servo_set(TRIBBLE_ARM, TA_UP, .3);//
	servo_set(TRIBBLE_CLAW, TC_OPEN, .3);//
	msleep(1000);//this lets the tribs fall out
	servo_set(TRIBBLE_CLAW, TC_PART_OPEN, .2);//put the claw back down
	servo_set(TRIBBLE_ARM, TA_DOWN, .2);//
	servo_set(TRIBBLE_CLAW, TC_OPEN, .2);//
	move_basket(BASKET_UP, true);//raise the basket
	hold_basket(BASKET_UP);//and hold it there
	servo_set(BLOCK_ARM, BA_UP, .1);//back into driving position
	time_drive(-40, -40, 300);//dump the basket
	msleep(1000);//let stuff drop
	forward(5,60);//move away from the caldera
	servo_set(BLOCK_ARM, BA_MID, .1);//get out of the way
	release_basket();
	move_basket(BASKET_DOWN, false);//bring it back
	servo_set(BLOCK_ARM, BA_UP, .1);//back to driving position
	return 42;
}
