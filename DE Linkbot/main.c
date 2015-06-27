#include "drive.h"
/*===ISSUES=====
Does not drive completely straight
Cannot front square up
=====ISSUES===*/

void back_squareup(time){
	motor(0,-80);
	motor(3,-80);
	sleep(time);
	ao();
}
void servo_setup(){
	set_servo_position(CLAWL,CLAW_DOWN);
	set_servo_position(CLAWOC,CLAW_CLOSE);
	set_servo_position(ARM,ARM_DOWN);
	msleep(1000);
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
	back(1,100);
	msleep(500);
	set_servo_position(CLAWOC,CLAW_OPEN);
	msleep(1000);
	set_servo_position(CLAWL,CLAW_DOWN);
	msleep(2000);
	//set_servo_position(CLAWOC,CLAW_CLOSE);//fast
	servo_set(CLAWOC,CLAW_CLOSE,2);//slow
	msleep(1000);
	//set_servo_position(CLAWL,CLAW_UP);//fast
	servo_set(CLAWL,CLAW_UP,1);//slow
	msleep(500);
}

int main(){
	/*===Start===*/
	servo_setup();
	printf("Press any button to start");
	WAIT(a_button()||b_button()||c_button());
	
	/*===Gold One===*/
	servo_drive_pos();
	back_squareup(1);
	forward(35,100);
	get_gold();
	
	/*===Reposition===*/
	back(17,100);
	right(90,0,100);
	back_squareup(2);
	
	/*===Botguy===*/
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

