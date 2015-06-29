#include "drive.h"
#include "etdrive.h"

/******************ISSUES********************
Needs to  do far side
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

void test(){
	printf("%d\n",avg_etdistance());
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
	squareup(-80,1);
	msleep(1000);
	etforward(32);
	squareup(50,1);
	get_gold();
	
	/*===Score Gold===*/
	etbackward(17);
	score_gold();
	
	/*===Reposition===*/
	forward(4,100);
	right(90,0,100);
	squareup(50,2);
	
	/*===Botguy===*/
	thread going=thread_create(arms_down_slow);
	thread_start(going);
	
	etbackward(65);
	set_servo_position(ARM,ARM_UP);
	etforward(8);
	set_servo_position(ARM,ARM_UP-150);
	set_servo_position(CLAWOC,CLAW_OPEN-450);
	msleep(500);
	
	int i = 0;
	for(i;i<3;i++){
		etforward(10);
		etbackward(10);
	}
	etbackward(8);
	set_servo_position(ARM,ARM_DOWN);
	msleep(500);
	etforward(45);
	
	/*===Cross Field===*/
	etbackward(100);
	squareup(-50,1);
	
	/*===Gold Two===*/
	etfoward(10);
	left(90,0,100);
	
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
