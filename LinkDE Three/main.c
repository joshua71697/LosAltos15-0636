#include "drive.h"
#include "etdrive.h"

/******************ISSUES********************
Needs to extract itself from botguy scoring.
Needs to  do far side
*******************ISSUES*******************/

void squareup(power,time){
	motor(0,power);
	motor(3,power);
	msleep(time);
	ao();
}
void servo_setup(){
	set_servo_position(CLAWL,CLAW_DOWN);
	set_servo_position(CLAWOC,CLAW_CLOSE);
	set_servo_position(ARM,ARM_DOWN);
	enable_servos();
}
void servo_drive_pos(){
	set_servo_position(ARM,ARM_UP+100);
	msleep(500);
	set_servo_position(CLAWL,CLAW_UP);
	msleep(500);
	set_servo_position(CLAWOC,CLAW_CLOSE-25);
	msleep(500);
}
void get_gold(){
	back(1,100);
	set_servo_position(CLAWOC,CLAW_OPEN);
	msleep(1000);
	set_servo_position(CLAWL,CLAW_DOWN);
	msleep(2000);
	servo_set(CLAWOC,CLAW_CLOSE,2);//slow
	msleep(1000);
	servo_set(CLAWL,CLAW_UP-50,1);//slow
	msleep(500);
}
void score_gold(){
	servo_set(CLAWL,CLAW_DOWN,1);
	msleep(1000);
	servo_set(CLAWOC,CLAW_OPEN,1);//slow
	msleep(1000);
	set_servo_position(CLAWL,CLAW_UP);
	msleep(500);
	set_servo_position(CLAWOC,CLAW_CLOSE);
	msleep(500);
}
void arms_down_slow(){
	msleep(1500);
	servo_set(CLAWL,CLAW_DOWN,1);
	servo_set(ARM,ARM_DOWN,1);
}
void arms_down_slow_two(){
	msleep(2000);
	set_servo_position(CLAWOC,CLAW_CLOSE);
	servo_set(CLAWL,CLAW_DOWN-25,1);
	servo_set(ARM,ARM_DOWN,1);
}

void arms_down_slow_three(){
	set_servo_position(CLAWOC,CLAW_CLOSE);
	servo_set(CLAWL,CLAW_DOWN-25,1);
	servo_set(ARM,ARM_DOWN+225,1);
}

void temptest(){
	while(!a_button()||!b_button()||!c_button()){
		printf("FRONT: %d",avg_etdistance());
		printf(" - BACK:  %d\n",avg_etdistanceb());
	}
}

void test(){
	set_servo_position(CLAWL,CLAW_DOWN);
	set_servo_position(CLAWOC,CLAW_OPEN);
	enable_servos();
	WAIT(a_button()||b_button()||c_button());
	set_servo_position(CLAWOC,CLAW_CLOSE);
}



void routine(){
	/*===Start===*/
	servo_setup();
	printf("Press any button to start\n\n");
	light_start(0);
	printf("waiting 2 seconds\n\n");
	sleep(2);
	
	/*===Gold One===*/
	servo_drive_pos();
	squareup(-80,1000);
	msleep(1000);
	etforward_sens(32);
	squareup(50,1000);
	get_gold();
	
	/*===Score Gold===*/
	etbackward(17);
	score_gold();
	
	/*===Reposition===*/
	forward(4,100);
	right(94,0,100);
	squareup(50,2500);
	
	/*===Botguy===*/
	thread going = thread_create(arms_down_slow);
	thread_start(going);
	
	etbackward(65);
	set_servo_position(ARM,ARM_UP);
	etforward(8);
	set_servo_position(ARM,ARM_UP-150);
	set_servo_position(CLAWOC,CLAW_OPEN-475);
	msleep(500);
	
	int i = 0;
	for(i;i<3;i++){
		etforward(7+i*2);
		etbackward(7+i*2);
	}
	etbackward(8);
	
	/*===Score Botguy===*/
	int store_et_distance = avg_etdouble();//store for later
	set_servo_position(ARM,ARM_DOWN+225);
	set_servo_position(CLAWL,CLAW_BOTGUY);
	msleep(1000);
	etforward_sens(10);//drive over divider
	
	set_servo_position(ARM,ARM_DOWN);
	set_servo_position(CLAWL,CLAW_DOWN);
	set_servo_position(CLAWOC,CLAW_OPEN);
	msleep(1000);
	etforward_sens_pass(40,store_et_distance);
	
	/*===Cross Field===*/
	thread going_two = thread_create(arms_down_slow_two);
	thread_start(going_two);
	etbackward_sens_touch(30);
	
	squareup(-50,1000);
	
	/*===Reposition===*/
	etforward(10);
	servo_drive_pos();
	msleep(500);
	right(120,0,100);
	etbackward_sens_touch(10);
	squareup(-50,1000);
	
	forward(15,100);
	right(200,0,100);
	etforward(15);
	
	/*===Gold Two===*/
	get_gold();
	back(20,100);
	right(100,0,100);
	
	thread going_home = thread_create(arms_down_slow_three);
	thread_start(going_home);
	etforward(74);
	etbackward(2);
	set_servo_position(ARM,ARM_UP);
	score_gold();
	
	/*===Tail===*/
	disable_servos();
	ao();
}

int main(){
	set_a_button_text("MAIN");
	set_b_button_text("TEST");
	set_c_button_text("EXIT");
	while(!c_button()){
		if(a_button()){
			reset_buttons();
			routine();
			break;
		}
		else if(b_button()){
			reset_buttons();
			test();
			break;
		}
	}
}
