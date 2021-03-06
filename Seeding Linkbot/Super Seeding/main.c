//edited seeding code to run for super seeding!

#include "drive.h"

#define PIPE_JUMP 1//strategmegies
#define CENTER_DRIVE 2//

void grab_blocks();//see below
void drive_dump();//

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
		WAIT(side_button());
		msleep(1000);
		set_servo_position(TRIBBLE_ARM, TA_DOWN);
		set_servo_position(TRIBBLE_CLAW, TC_OPEN);
		set_servo_position(BLOCK_CLAW, BC_START);
		set_servo_position(BASKET_ARM, BA_DOWN);
		enable_servos();
		msleep(2000);
		forward(20, 60);
		return 0;
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
	shutdownin(239.5);
	start();//timing
	if(strategy==PIPE_JUMP)
	{
		ready_to_jump();
		time_drive(-90, -90, 1300);//jump!
		servo_set(TRIBBLE_ARM, TA_UP,.3);//move the claw up so it can square up
		time_drive(-50, -50, 1500);//get towards the pipe
		physical_squareup(false);//square up on the back
		time_drive(50, 50, 2500);//go back towards the other wall
		physical_squareup(true);//square up on the front
		time_drive(-50, -50, 1000);//get back to the center
		right(87, 0, 50);//turn towards the edge
		time_drive(-60, -60, 1500);//move towards edge
		physical_squareup(false);//square up on the outside of the field
		tribble_claw_drop();//drop the claw-->plow position
		time_drive(68, 60, 1100);//arc into the wall to wall follow to the blocks
		//forward(5.25, 60);//move to block position
		grab_blocks();//grab the blocks...
		forward(20, 60);//plow the tribbles!
		servo_set(TRIBBLE_CLAW, TC_CLOSE, .7);//close claw to catch tribbles
		servo_set(TRIBBLE_ARM, TA_UP, 1);//put the arm up to get across the center
		forward(20, 60);//get across the center
		servo_set(TRIBBLE_ARM, TA_START, .5);//push into the ground so the claw doesn't jump
		servo_set(TRIBBLE_CLAW, TC_OPEN, .8);//back into plow position
		servo_set(TRIBBLE_ARM, TA_DOWN, .1);//back to drive position
		msleep(250);
		forward(7, 60);//get to dumping location
		nowstr("first dump started at");
		drive_dump();//dump...
		forward(7, 60);//replow the tribbles (maybe get a few extra)
		back(2, 60);//get to optimal grab location
		tribble_claw_dump();//put the tribbles in the basket
		back(5, 60);//back to dump location
		tribble_claw_drop();//put the claw back down
		drive_dump();//dump again
		nowstr("first dump finished at");
		//forward(15, 60);//plow the second set of tribbles, get to block location
		time_drive(66, 60, 3000);//arc towards the right wall to make sure it follows the right wall
		grab_blocks();//grab the blocks...
		forward(8, 60);//plow the remaining tribbles
		servo_set(TRIBBLE_CLAW, TC_CLOSE, .4);//grab the tribbles
		servo_set(TRIBBLE_ARM, TA_DUMP, 1);//raise the arm (to allow it to square up)
		time_drive(60, 60, 1500);//get towards the wall
		physical_squareup(true);//and square up on it
		msleep(200);
		back_line_follow(35.8, 60);//back up to the dumping location
		servo_set(TRIBBLE_ARM, TA_DOWN, .5);//put the arm down to get it out of the way
		nowstr("second dump started at");
		drive_dump();//dump
		tribble_claw_dump();//put the tribbles in the basket
		tribble_claw_drop();//
		drive_dump();//dump again
		nowstr("second dump finished at");
		servo_set(TRIBBLE_CLAW, TC_PART_OPEN, .3);//get the claw up and out of the way
		servo_set(TRIBBLE_ARM, TA_UP, .3);
		back(6, 60);//back up to get the caught tribbles
		servo_set(TRIBBLE_ARM, TA_START, .3);//put the claw back down
		servo_set(TRIBBLE_CLAW, TC_OPEN, .5);//
		servo_set(TRIBBLE_ARM, TA_DOWN, .1);//
		forward(38, 60);//push the tribbles (get any that went way down the field)
		back(2, 60);//tribbles near edge of claw
		tribble_claw_dump();
		servo_set(TRIBBLE_CLAW, TC_CLOSE, .3);//close the claw, just for good measure
		back_line_follow(30, 60);//get back to the dumping location
		servo_set(TRIBBLE_ARM, TA_DOWN, .5);//get the arm out of the way
		nowstr("third dump started at");
		drive_dump();
		nowstr("third dump finished at");
		back_line_follow(17, 60);//get back across the center
		right(180, 0, 50);//turn around
		tribble_claw_drop();//put the claw down...
		forward(25, 60);//and plow!
		back(2, 60);//optimal grab location
		tribble_claw_dump();//well duh...
		servo_set(TRIBBLE_CLAW, TC_CLOSE, .3);//close the claw for good measure
		time_drive(60, 60, 1500);//move towards the wall
		physical_squareup(true);//and square up on it
		back_line_follow(35.8, 60);//back up to the dumping location
		servo_set(TRIBBLE_ARM, TA_DOWN, .5);//get the claw out of the way
		nowstr("fourth dump started at");
		drive_dump();//dump stuff
		move_block_arm(BLA_MID);//get the block arm back out of the way
		servo_set(BASKET_ARM, BA_UP, 1);//put the basket back up
		time_drive(-50, -50, 1500);//one last chance to dump (stay there just cause...you never know)
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
	motor(BLOCK_ARM, -50);//stall it into the basket
	servo_set(BLOCK_CLAW, BC_OPEN,.75);//
	msleep(300);
	servo_set(BLOCK_CLAW, BC_START, .4);//shake the claw a bit to try to push the blocks in if they didn't go in the first time
	msleep(100);
	servo_set(BLOCK_CLAW, BC_OPEN, .75);//
	msleep(300);
	servo_set(BLOCK_CLAW, BC_START, .4);//
	off(BLOCK_ARM);//stop stalling the motor
	servo_set(TRIBBLE_CLAW, TC_PART_OPEN, .4);//put the claw back down
	servo_set(TRIBBLE_ARM, TA_START, .4);//
	msleep(200);
	servo_set(TRIBBLE_CLAW, TC_OPEN, .6);//
	servo_set(TRIBBLE_ARM, TA_DOWN, .1);//
}

void drive_dump()//dumps the basket into the caldera by driving
{
	move_block_arm(BLA_MID);//get the arm out of the way
	servo_set(BASKET_ARM, BA_UP, 1);//raise the arm
	move_block_arm(BLA_UP);//driving position
	time_drive(-50, -50, 1000);//drive back into the caldera
	msleep(1000);//let stuff fall out
	time_drive(50, 50, 500);//one more attempted dump
	time_drive(-50, -50, 1000);//
	time_drive(50, 50, 500);//get off the caldera a bit (hopefully reducing catching)
	msleep(1000);//more time to fall out
	move_block_arm(BLA_MID);//get the arm out of the way
	servo_set(BASKET_ARM, BA_DOWN, 1);//bring the basket down
	move_block_arm(BLA_UP);//driving position
}
