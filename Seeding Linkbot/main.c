#include "drive.h"

#define PIPE_JUMP 1//strategmegies
#define CENTER_DRIVE 2//

void grab_blocks();//see below

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
		reset_buttons();
		set_up_drive();
		printf("press black button to begin\n");
		WAIT(side_button());
		printf("starting in 2 seconds...\n");
		msleep(2000);
		back_line_follow_time(60, 5000);
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
		msleep(500);//give the robot time to settle down
		time_drive(-90, -90, 1300);//jump!
		servo_set(TRIBBLE_ARM, TA_UP,.3);//move the claw up so it can square up
		msleep(500);//let the robot settle down some
		time_drive(-50, -50, 1500);//get towards the pipe
		msleep(500);//let the robot settle down some
		physical_squareup(false);//square up on the back
		time_drive(50, 50, 2500);//go back towards the other wall
		physical_squareup(true);//square up on the front
		time_drive(-50, -50, 1000);//get back to the center
		right(87, 0, 50);//turn towards the edge
		time_drive(-60, -60, 1500);//move towards edge
		physical_squareup(false);//square up on the outside of the field
		tribble_claw_drop();//drop the claw-->plow position
		forward(5.25, 60);//move to block position
		grab_blocks();//grab the blocks...
		forward(20, 60);//plow the tribbles!
		servo_set(TRIBBLE_CLAW, TC_CLOSE, .7);//grab the tribbles
		servo_set(TRIBBLE_ARM, TA_DOWN-100, .3);//lift the arm slightly so it doesn't get caught on the bump
		forward(18, 60);//get across the center
		servo_set(TRIBBLE_ARM, TA_START, .1);//push into the ground so the claw doesn't jump
		servo_set(TRIBBLE_CLAW, TC_OPEN, .8);//back into plow position
		servo_set(TRIBBLE_ARM, TA_DOWN, .1);//back to drive position
		msleep(250);
		forward(14, 60);//get to dumping location (overshoot a bit to get an extra tribble or two)
		back(2, 60);//pull back a bit from the tribbles (the far end of the claw dumps better than the close end
		servo_set(TRIBBLE_CLAW, TC_CLOSE, .5);//grab the tribbles so they can't escape
		back(1.5, 60);//get back to the dumping location
		move_block_arm(BLA_MID);//get the block arm out of the way
		nowstr("first dump started at");
		dump_basket();//dump...
		tribble_claw_dump();//put the tribbles in the basket
		dump_basket();//and again
		nowstr("first dump finished at");
		move_block_arm(BLA_UP);//back to driving position
		//forward(15, 60);//plow the second set of tribbles, get to block location
		time_drive(62, 60, 3200);//arc towards the right wall to make sure it follows the right wall
		grab_blocks();//grab the blocks...
		forward(8, 60);//plow the remaining tribbles
		servo_set(TRIBBLE_CLAW, TC_CLOSE, .4);//grab the tribbles
		servo_set(TRIBBLE_ARM, TA_UP, 1);//raise the arm (to allow it to square up)
		time_drive(60, 60, 1500);//get towards the wall
		physical_squareup(true);//and square up on it
		msleep(200);
		back_line_follow(33.5, 60);//back up to the dumping location
		servo_set(TRIBBLE_ARM, TA_DOWN, .5);//put the arm down to get it out of the way
		move_block_arm(BLA_MID);//get the block arm out of the way
		nowstr("second dump started at");
		dump_basket();//dump
		tribble_claw_dump();//put the tribbles in the basket
		dump_basket();//and again
		forward(2, 60);//push the tribbles a bit
		back(2, 60);//get back into dump location, tribbles near edge of claw
		tribble_claw_dump();//one last chance to get any extra tribbles that didn't make it the first time
		dump_basket();//and one final time
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
	nowstr("finished at");
	return 42;
}

void grab_blocks()//goes through the routine to pick up the blocks
{//starts once near to the pipe, with plowed tribbles; ends back from the pipe, with the tribbles in front, tribble arm down
	servo_set(BLOCK_CLAW, BC_CLOSE, .1);//drop the claw
	move_block_arm(BLA_DOWN);//
	servo_set(BLOCK_CLAW, BC_OPEN, .1);//
	time_drive(50,50,1700);//drive up to blocks
	servo_set(BLOCK_CLAW, BC_CLOSE,.75);//and pick them up
	msleep(250);
	move_block_arm(BLA_LIFT);//get off the ground
	thread hold=thread_create(hold_ba_lift);//this will hold the block arm at the lift location
	thread_start(hold);//
	back(6, 60);//back away from the pipe
	thread_destroy(hold);//don't want to hold it up any more
	servo_set(TRIBBLE_ARM, TA_JUMP, .5);//bring the ta up to keep the blocks in the basket
	servo_set(TRIBBLE_CLAW, TC_CLOSE, .5);//
	servo_set(TRIBBLE_ARM, TA_UP, .5);//
	move_block_arm(BLA_UP);//drop in the basket
	msleep(500);
	servo_set(BLOCK_CLAW, BC_OPEN,.75);//
	msleep(300);
	servo_set(BLOCK_CLAW, BC_START, .4);//shake the claw a bit to try to push the blocks in if they didn't go in the first time
	msleep(100);
	servo_set(BLOCK_CLAW, BC_OPEN, .75);//
	msleep(300);
	servo_set(BLOCK_CLAW, BC_START, .4);//
	servo_set(TRIBBLE_CLAW, TC_PART_OPEN, .4);//put the claw back down
	servo_set(TRIBBLE_ARM, TA_START, .4);//
	msleep(200);
	servo_set(TRIBBLE_CLAW, TC_OPEN, .6);//
	servo_set(TRIBBLE_ARM, TA_DOWN, .1);//
}
