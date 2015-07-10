#include "drive.h"
#include "etdrive.h"

/******************ISSUES********************
Needs to be more reliable; aka. not suck
*******************ISSUES*******************/
int half;

void squareup(power,time){
	motor(0,power);
	motor(3,power);
	msleep(time);
	ao();
}
void squareup_arc(lpower,rpower,time){
	motor(0,lpower);
	motor(3,rpower);
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
	set_servo_position(ARM,ARM_UP-100);
	msleep(500);
	set_servo_position(CLAWL,CLAW_UP);
	msleep(500);
	set_servo_position(CLAWOC,CLAW_CLOSE-25);
	msleep(500);
}
void get_gold(){
	//back(1,100);
	set_servo_position(CLAWOC,CLAW_OPEN);
	msleep(500);
	set_servo_position(CLAWL,CLAW_DOWN-25);
	msleep(500);
	
	servo_set(CLAWOC,CLAW_CLOSE,2);//slow
	msleep(200);
	back(1,100);
	msleep(1000);
	servo_set(CLAWL,CLAW_UP-50,1);//slow
	msleep(500);
}
void get_gold_proto(){
	//back(1,100);
	set_servo_position(CLAWOC,CLAW_OPEN);
	msleep(500);
	set_servo_position(CLAWL,CLAW_DOWN);
	msleep(500);//
	
	servo_set(CLAWOC,CLAW_OPEN-200,0.5);//slow
	msleep(200);
	set_servo_position(CLAWL,CLAW_DOWN);//rearticulate
	msleep(200);
	servo_set(CLAWOC,CLAW_CLOSE,1.5);
	back(1,100);
	msleep(1000);
	servo_set(CLAWL,CLAW_UP-50,1);//slow
	msleep(500);
}
void score_gold(){
	servo_set(CLAWL,CLAW_UP,0.5);//servo_set(CLAWL,CLAW_DOWN+250,1);
	//msleep(1000);
	servo_set(CLAWOC,CLAW_OPEN,1);//slow
	msleep(200);
	set_servo_position(CLAWL,CLAW_UP);
	msleep(200);
	servo_set(CLAWOC,CLAW_CLOSE,1);//set_servo_position(CLAWOC,CLAW_CLOSE);
	//msleep(500);
}
void arms_down_slow(){
	msleep(1500);
	servo_set(CLAWL,CLAW_DOWN,1);
	servo_set(ARM,ARM_DOWN,1);
}
void arms_down_slow_two(){
	msleep(2500);
	set_servo_position(CLAWOC,CLAW_CLOSE);
	servo_set(CLAWL,CLAW_DOWN-25,0.25);
	servo_set(ARM,ARM_DOWN,0.25);
}

void arms_down_slow_three(){
	set_servo_position(CLAWOC,CLAW_CLOSE);
	servo_set(CLAWL,CLAW_DOWN-25,0.5);
	servo_set(ARM,ARM_MID+100,0.5);//used to be ARM_BOTGUY
}

void test(){
	etbackward_sens_touch(1);
}



int routine(){
	/*===Start===*/
	servo_setup();
	printf("Press any button to start\n\n");
	light_start(0);
	
	shutdownin(119.5);
	
	printf("waiting 2 seconds\n\n");
	sleep(2);
	
	/*===Gold One===*/
	servo_drive_pos();
	squareup_arc(-100,-50,1000);
	etforward(32);//was sens
	squareup(50,1000);
	get_gold_proto();
	
	/*===Score Gold===*/
	etbackward(17);
	right(10,0,100);
	score_gold();
	left(10,0,100);
	
	/*===Reposition===*/
	//forward(1,100);//sets distance from the create
	right(90,0,80);
	squareup(50,2000);
	msleep(500);
	
	/*===Botguy===*/
	thread going = thread_create(arms_down_slow);
	thread_start(going);
	
	int store_et_distance = avg_etdouble();//store for later
	etbackward(63);
	set_servo_position(ARM,ARM_UP-150);

	set_servo_position(CLAWOC,CLAW_OPEN-475);
	msleep(500);
	etforward_sens_pass(6,store_et_distance);
	set_servo_position(ARM,ARM_UP+100);
	
	int i = 0;
	for(i;i<3;i++){
		etforward(7+i);
		etbackward(7+i);
	}
	etbackward(7);//back away from botguy
	
	/*===Score Botguy===*/ //A Move in two parts
	
	//Part one
	set_servo_position(ARM,ARM_MID-100);
	set_servo_position(CLAWL,CLAW_BOTGUY);
	msleep(200);
	etforward_sens(10);//drive over divider

	//Part Two
	set_servo_position(ARM,ARM_MID+100);
	set_servo_position(CLAWL,CLAW_DOWN);
	set_servo_position(CLAWOC,CLAW_OPEN-350);
	msleep(200);
	etforward(40);//etforward_sens_pass(40,store_et_distance);
	set_servo_position(ARM,ARM_UP);
	
	if(half ==1){
		return 1;
	}
	else{}
	
	/*===Cross Field===*/
	thread going_two = thread_create(arms_down_slow_two);
	thread_start(going_two);
	etbackward_sens_touch(15);
	
	squareup(-50,1000);
	
	/*===Reposition===*/
	etforward(6);
	servo_drive_pos();
	msleep(500);
	right(100,0,100);
	msleep(200);
	etbackward_sens_touch(4);//back up into position
	squareup(-50,1000);
	
	forward(18,100);
	right(190,0,100); // turn around
	msleep(500);
	//etforward(15);
	forward(15,100);
	squareup(60,1000);//drive up to the pipe
	msleep(500);
	back(0.5,100);//back up from pipe
	
	/*===Gold Two===*/
	get_gold();
	back(25,100);
	servo_set(CLAWL,CLAW_DOWN,0.5);
	right(90,0,100);
	right(45,0,100);
	msleep(500);
	left(40,0,100);

	//forward(5,100);
	//forward(5,100);
	//left(150,0,100);
	//msleep(500);
	//right(100,0,100);//sweep tribbles out of the way
	
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

void botguy_first(){
	/*===Start===*/
	servo_setup();
	printf("Press any button to start\n\n");
	light_start(0);
	shutdownin(119.5);
	printf("waiting 2 seconds\n\n");
	sleep(2);
	
	/*===Gold One===*/
	servo_drive_pos();
	squareup_arc(-100,-50,1000);
	
	etforward(14);//halfway across
	
	right(90,0,80);
	squareup(50,2000);
	msleep(500);
	
	/*===Botguy===*/
	thread going = thread_create(arms_down_slow);
	thread_start(going);
	
	int store_et_distance = avg_etdouble();//store for later
	etbackward(64);
	set_servo_position(ARM,ARM_UP-150);

	set_servo_position(CLAWOC,CLAW_OPEN-550);//-475
	msleep(500);
	etforward_sens_pass(6,store_et_distance);
	set_servo_position(ARM,ARM_UP+100);
	
	int i = 0;
	for(i;i<2;i++){
		etforward(8+i);
		etbackward(8+i);
	}
	etbackward(7);//back away from botguy
	
	/*===Score Botguy===*/ //A Move in two parts
	
	//Part one
	set_servo_position(ARM,ARM_MID-100);
	set_servo_position(CLAWL,CLAW_BOTGUY);
	msleep(200);
	etforward_sens(10);//drive over divider

	//Part Two
	set_servo_position(ARM,ARM_MID+100);
	set_servo_position(CLAWL,CLAW_DOWN);
	set_servo_position(CLAWOC,CLAW_OPEN-300);
	msleep(200);
	etforward(50);//etforward_sens_pass(40,store_et_distance);
	
	
	//Gold One
	

	//etforward(12);
	//etbackward(2);
	servo_drive_pos();
	
	left(94,0,100);
	forward(5,100);
	etforward(18);
	
	get_gold_proto();
	
	//score gold
	etbackward(17);
	right(10,0,100);
	score_gold();
	left(10,0,100);
	
}

int main(){
	set_a_button_text("MAIN");
	set_b_button_text("HALF");
	set_c_button_text("BGUY");
	while(!c_button()){
		if(a_button()){
			reset_buttons();
			half=0;
			printf("Running Main");
			routine();
			break;
		}
		else if(b_button()){
			reset_buttons();
			half = 1;
			printf("Running Half");
			routine();
			break;
		}
		else if(c_button()){
			reset_buttons();
			half = 0;
			printf("Running Botguy First");
			botguy_first();
			break;
		}
	}
}
