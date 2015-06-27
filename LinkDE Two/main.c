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

/*===ISSUES=====
Needs to get botguy and far side
=====ISSUES===*/

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
	set_servo_position(ARM,ARM_UP);
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

int main(){
	/*===Start===*/
	servo_setup();
	printf("Press any button to start");
	WAIT(a_button()||b_button()||c_button());
	//printf("waiting 2 seconds"); sleep(2);
	
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
	LEFT(87,0);
	squareup(80,3);
	
	/*===Botguy===*/
	thread going=thread_create(arms_down_slow);
	thread_start(going);
	backward(180);
	set_servo_position(ARM,ARM_UP);
	msleep(500);
	forward(30);
	backward(30);
	set_servo_position(ARM,ARM_DOWN);
	msleep(500);
	
	/*===Tail===*/
	disable_servos();
	ao();
	
}

int tempmain(){
	RIGHT(90,0);
	printf("Hello,World!");
	msleep(1000);
}

