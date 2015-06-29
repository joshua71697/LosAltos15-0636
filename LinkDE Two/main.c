#define CLAWOC 0//claw open/close port
#define CLAWL 1//claw lift port
#define ARM 3// arm port

#define CLAW_CLOSE 600
#define CLAW_OPEN 1550
#define CLAW_UP 1100
#define CLAW_DOWN 525
#define ARM_UP 1500
#define ARM_DOWN 800

#include "./template.h"
#include "drive.h"

#define RIGHT(distance,radius); left(distance,radius);
#define LEFT(distance,radius); right(distance,radius);

/******************ISSUES********************
Needs to get botguy and far side
*******************ISSUES*******************/

void squareup(power,time){
	motor(0,power);
	motor(3,power);
	msleep(time*1000);
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
	backward(2.5);
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
	msleep(2000);
	servo_set(CLAWL,CLAW_DOWN,1);
	servo_set(ARM,ARM_DOWN,1);
}

void routine(){
	/*===Start===*/
	servo_setup();
	printf("Press any button to start\n");
	//WAIT(a_button()||b_button()||c_button());
	light_start(0);
	printf("waiting 2 seconds"); sleep(2);
	
	/*===Gold One===*/
	servo_drive_pos();
	squareup(-80,1);
	forward(90);
	squareup(80,1);
	get_gold();
	
	/*===Score Gold===*/
	backward(42);
	score_gold();
	
	/*===Reposition===*/
	forward(5);
	RIGHT(80,0);
	squareup(80,1);
	
	/*===Botguy===*/
	thread going=thread_create(arms_down_slow);
	thread_start(going);
	
	backward(180);
	set_servo_position(ARM,ARM_UP);
	msleep(500);
	forward(55);
	backward(55);
	set_servo_position(ARM,ARM_DOWN);
	msleep(500);
	set_servo_position(CLAWOC,CLAW_OPEN-300);
	forward(250);
	
	/*===Tail===*/
	disable_servos();
	ao();
	
}

void test(){
	backward(200);
	printf("Hello,World!");
	msleep(1000);
}

int main(){
	set_a_button_text("MAIN");
	set_b_button_text("TEST");
	set_c_button_text("EXIT");
	while(!c_button()){
		if(a_button()){
			reset_buttons();
			routine();
		}
		else if(b_button()){
			reset_buttons();
			test();
		}
	}
}
