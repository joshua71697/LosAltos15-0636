#include "drive.h"

#define PIPE_JUMP 1//strategmegies
#define CENTER_DRIVE 2//

void jerk_bot();//see below

int main()
{
	set_a_button_text("pipe jumping");
	set_b_button_text("center drive");
	set_c_button_text("testing");
	int strategy;
	WAIT(a_button()||b_button()||c_button());
	if(a_button())
	{
		strategy=PIPE_JUMP;
	}
	else if(b_button())
	{
		strategy=CENTER_DRIVE;
	}
	else//c
	{
		//set_up_drive();//gets the servos and stuff in driving position
		//test_driving();
		msleep(2000);
		set_servo_position(BASKET_ARM, BA_DOWN);
		set_servo_position(TRIBBLE_ARM, TA_DOWN);
		set_servo_position(TRIBBLE_CLAW, TC_OPEN);
		enable_servos();
		set_a_button_text("drop");
		set_b_button_text("exit");
		set_c_button_text("-");
		while(1)
		{
			WAIT(a_button()||b_button());
			if(b_button())
				break;
			servo_set(TRIBBLE_CLAW, TC_CLOSE, .4);//only gets here if hit a button
			servo_set(TRIBBLE_ARM, TA_JUMP, .7);
			servo_set(TRIBBLE_ARM, TA_UP, .7);
			msleep(500);
			servo_set(TRIBBLE_CLAW, TC_OPEN, .4);
			msleep(500);
			servo_set(TRIBBLE_CLAW, TC_CLOSE, .4);
			servo_set(TRIBBLE_CLAW, TC_OPEN, .4);
			msleep(500);
			servo_set(TRIBBLE_ARM, TA_DOWN, .4);
		}
		return 9001;
	}
	WAIT(!a_button()&&!b_button());
	set_a_button_text("light start");//only gets here if running one of the real code options
	set_b_button_text("no ls");
	set_c_button_text("-");
	boolean l_s=false;//whether or not is doing light start
	WAIT(a_button()||b_button());
	if(a_button())//if b, does nothing-->l_s already false
		l_s=true;
	reset_buttons();
	if(strategy==PIPE_JUMP)
		set_up_jump();
	else//drive
		set_up_drive();
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
	if(strategy==PIPE_JUMP)
	{
		ready_to_jump();
		time_drive(-90, -90, 1300);//jump!
		servo_set(TRIBBLE_ARM, TA_UP,.1);//move the claw up so it can square up
		time_drive(-50, -50, 1500);//get towards the pipe
		physical_squareup(false);//square up on the back
		time_drive(50, 50, 2500);//go back towards the other wall
		physical_squareup(true);//square up on the front
		time_drive(-50, -50, 850);//get back to the center
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
		servo_set(TRIBBLE_ARM, TA_DUMP, .5);//
		servo_set(TRIBBLE_CLAW, TC_OPEN, .5);//
		msleep(1000);//this lets the tribs fall out
		servo_set(TRIBBLE_CLAW, TC_PART_OPEN, .2);//put the claw back down
		servo_set(TRIBBLE_ARM, TA_DOWN, .2);//
		servo_set(TRIBBLE_CLAW, TC_OPEN, .2);//
		servo_set(BASKET_ARM, BA_UP, 1);//raise the basket
		jerk_bot();//try to get the stuff to fall out
		servo_set(BASKET_ARM, BA_DOWN, 1);//bring it back
		move_block_arm(BLA_UP);//back to driving position
	}
	else//drive strategy
	{
		forward(38, 90);//get towards the center
		right(90, 0, 70);//turn towards the middle of the gap
		forward(17, 90);//get through the gap and to the black tape
		left(85, 0, 70);//turn parallel to the black tape
		servo_set(TRIBBLE_ARM, TA_JUMP, .4);//get the claw into plow position
		servo_set(TRIBBLE_CLAW, TC_PART_OPEN, .4);//
		servo_set(TRIBBLE_ARM, TA_DOWN, .4);//
		servo_set(TRIBBLE_CLAW, TC_OPEN, .4);//
	}
	return 42;
}

void jerk_bot()//when the bot is dumping, jerks the bot to get stuff to fall out
{
	msleep(1000);//let the stuff that easily falls out do so
	servo_set(BASKET_ARM, BA_JERK, .1);//jerk the basket
	servo_set(BASKET_ARM, BA_UP, .1);//
	msleep(500);
	servo_set(BASKET_ARM, BA_JERK, .1);//
	servo_set(BASKET_ARM, BA_UP, .1);//
	msleep(1000);//last chance to let it fall out
}
