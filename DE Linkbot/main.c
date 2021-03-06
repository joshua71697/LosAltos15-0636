#include "drive.h"
/*===ISSUES=====
Does not drive straight
=====ISSUES===*/

void squareup(power,time){
	motor(0,power);
	motor(3,power);
	sleep(time);
	ao();
}
void servo_setup(){
	set_servo_position(CLAWL,CLAW_DOWN);
	set_servo_position(CLAWOC,CLAW_CLOSE);
	set_servo_position(ARM,ARM_DOWN);
	enable_servos();
}
void servo_drive_pos(){
	set_servo_position(ARM,ARM_UP);
	msleep(500);
	set_servo_position(CLAWL,CLAW_UP);
	msleep(500);
	set_servo_position(CLAWOC,CLAW_CLOSE-25);
	msleep(500);
}
void get_gold(){
	squareup(-100,0.6);
	set_servo_position(CLAWOC,CLAW_OPEN);
	msleep(1000);
	set_servo_position(CLAWL,CLAW_DOWN);
	msleep(2000);
	//set_servo_position(CLAWOC,CLAW_CLOSE);//fast
	servo_set(CLAWOC,CLAW_CLOSE,2);//slow
	msleep(1000);
	//set_servo_position(CLAWL,CLAW_UP-50);//fast
	servo_set(CLAWL,CLAW_UP-50,1);//slow
	msleep(500);
}
void score_gold(){
	//set_servo_position(CLAWL,CLAW_DOWN);//fast
	servo_set(CLAWL,CLAW_DOWN,1);
	msleep(1000);
	//set_servo_position(CLAWOC,CLAW_OPEN);//fast
	servo_set(CLAWOC,CLAW_OPEN,1);//slow
	msleep(1000);
	set_servo_position(CLAWL,CLAW_UP);
	msleep(500);
	set_servo_position(CLAWOC,CLAW_CLOSE);
	msleep(500);
}
void arms_down_slow(){
	sleep(2);
	servo_set(CLAWL,CLAW_DOWN,1);
	servo_set(ARM,ARM_DOWN,1);
}

int main(){
	/*===Start===*/
	servo_setup();
	printf("Press any button to start");
	WAIT(a_button()||b_button()||c_button());
	//printf("waiting 2 seconds"); sleep(2);
	
	/*===Gold One===*/
	servo_drive_pos();
	squareup(-80,1);
	forward(35,100);
	squareup(80,1);
	get_gold();
	
	/*===Score Gold===*/
	back(17,80);
	score_gold();
	
	/*===Reposition===*/
	left(90,0,100);
	squareup(80,3);
	
	/*===Botguy===*/
	thread going=thread_create(arms_down_slow);
	thread_start(going);
	back(20,100);
	
	back(30,100);
	/*set_servo_position(ARM,ARM_UP);
	msleep(500);
	forward(34+5,100);
	back(5);
	set_servo_position(ARM,ARM_DOWN);
	msleep(500);*/
	
	/*===Tail===*/
	disable_servos();
	ao();
	
}

int tempmain(){
	back(40,100);
}

