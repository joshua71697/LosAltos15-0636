#include "drive.h"

void jerk_bot();//see below

int main()
{
	set_a_button_text("lightstart");
	set_b_button_text("no ls");
	set_c_button_text("testing");
	boolean l_s=false;//whether or not is doing light start
	WAIT(a_button()||b_button()||c_button());
	if(a_button())
	{
		l_s=true;
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
		light_start(LIGHT_SENSOR);
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
	time_drive(-50, -50, 1500);//get towards the pipe
	physical_squareup(false);//square up on the back
	time_drive(50, 50, 750);//get back to the center
	right(87, 0, 50);//turn towards the edge
	time_drive(-50, -50, 2500);//move towards edge
	physical_squareup(false);//square up on the outside of the field
	servo_set(TRIBBLE_ARM, TA_JUMP, .3);//move arm down some so it can open the claw
	servo_set(TRIBBLE_CLAW, TC_PART_OPEN, .3);//open the claw part way (to make sure it won't hit the edge)
	servo_set(TRIBBLE_ARM, TA_DOWN, .5);//drop the claw
	msleep(300);
	servo_set(TRIBBLE_CLAW, TC_OPEN, .3);//open the claw the rest of the way-->ready to go
	forward(6, 60);//move to block position
	servo_set(BLOCK_CLAW, BC_CLOSE, .1);//drop the claw
	move_block_arm(BLA_DOWN);//
	servo_set(BLOCK_CLAW, BC_OPEN, .1);//
	time_drive(50,50,1200);//drive up to blocks
	servo_set(BLOCK_CLAW, BC_CLOSE,.75);//and pick them up
	msleep(250);
	move_block_arm(BLA_LIFT);//get off the ground
	back(5, 60);//back away from the pipe
	servo_set(TRIBBLE_ARM, TA_JUMP, .5);//bring the ta up to keep the blocks in the basket
	servo_set(TRIBBLE_CLAW, TC_CLOSE, .5);//
	servo_set(TRIBBLE_ARM, TA_UP, .5);//
	move_block_arm(BLA_UP);//drop in the basket
	msleep(500);
	servo_set(BLOCK_CLAW, BC_OPEN,.75);//
	msleep(300);
	servo_set(TRIBBLE_CLAW, TC_PART_OPEN, .4);//put the claw back down
	servo_set(TRIBBLE_ARM, TA_DOWN, .3);//
	servo_set(TRIBBLE_CLAW, TC_OPEN, .3);//
	servo_set(BLOCK_CLAW, BC_START, .1);//needs to be in this position to drive-->very condensed
	forward(20, 60);//plow the tribbles!
	servo_set(TRIBBLE_CLAW, TC_CLOSE, .5);//grab the tribbles
	forward(18, 60);//get across the center
	servo_set(TRIBBLE_CLAW, TC_OPEN, .5);//back into plow position
	msleep(250);
	forward(9, 58);//get to dumping location
	servo_set(TRIBBLE_CLAW, TC_CLOSE, .5);//grab the tribbles so they can't escape
	move_block_arm(BLA_MID);//get the block arm out of the way
	servo_set(BASKET_ARM, BA_UP, 1);//get it to the top
	jerk_bot();//try to get the stuff to fall out
	servo_set(BASKET_ARM, BA_DOWN, 1);//bring it back
	servo_set(TRIBBLE_ARM, TA_UP, .5);//
	servo_set(TRIBBLE_CLAW, TC_OPEN, .5);//
	msleep(1000);//this lets the tribs fall out
	servo_set(TRIBBLE_CLAW, TC_PART_OPEN, .2);//put the claw back down
	servo_set(TRIBBLE_ARM, TA_DOWN, .2);//
	servo_set(TRIBBLE_CLAW, TC_OPEN, .2);//
	servo_set(BASKET_ARM, BA_UP, 1);//raise the basket
	jerk_bot();//try to get the stuff to fall out
	servo_set(BASKET_ARM, BA_DOWN, 1);//bring it back
	move_block_arm(BLA_UP);//back to driving position
	return 42;
}

void jerk_bot()//when the bot is dumping, jerks the bot to get stuff to fall out
{
	msleep(1000);//let the stuff that easily falls out do so
	time_drive(-60, -60, 300);//jerk the robot backwards
	msleep(250);
	time_drive(50, 50, 200);//jerk it forwards a bit (also get a bit further away for another jerk)
	msleep(250);
	time_drive(-50, -50, 300);//one more jerk
	msleep(1000);//last chance to let it fall out
}
