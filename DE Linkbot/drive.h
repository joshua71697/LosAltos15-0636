#ifndef _DRIVE_H_
#define _DRIVE_H_

#include "generic.h"


//primary driving code
#define MOT_LEFT 0
#define MOT_RIGHT 3
#define PI 3.14159265358979323846264338327950288419716939937510582097494459//yea, get rekt
#define SPD 1000
#define SPDl 950.
#define SPDr 950.
#define PWRr 74//using these when in attack setup
#define PWRl 80//the upper one should always be at 80
#define HSPDl (int)(PWRl/4)//used for line squareups
#define HSPDr (int)(PWRr/4)
#define rdistmult (SPDr/SPDl)
#define DIST_CORR 0.95//make it move the right distance
#define TURN_CONST 0.83//make it turn properly
#define DtoT 85//distace to time-->should be how long (ms) it takes to move 1 in

#define ks 7.5//distance from one wheel to another in cm
#define wheeldiameter 4 //this is in cm

#define CMtoBEMF (921/wheeldiameter/PI) //921 is how many backemf ticks there are in a full wheel, take the number of units per rotation, divide by circumference
#define LBUMP digital(14)
#define RBUMP digital(15) //left/right back bump sensors (used for square_back())


#define drive_off() off(MOT_RIGHT) ;off(MOT_LEFT)
#define drive(mL,mR); {mav(MOT_LEFT,(mL)*10);mav(MOT_RIGHT,(mR)*10);}

#define TIME_TURN_POWER 80

#define LLIGHT analog(15)
#define RLIGHT analog(10)
#define BLACK 750


void right_time(int time)
{
	motor(MOT_RIGHT, -TIME_TURN_POWER);
	motor(MOT_LEFT, TIME_TURN_POWER);
	msleep(time);
	ao();
}
void left_time(int time)
{
	motor(MOT_RIGHT, TIME_TURN_POWER);
	motor(MOT_LEFT, -TIME_TURN_POWER);
	msleep(time);
}
	

void square_back()
{
	int _A = 0,_B = 0;
	float time = seconds();//used for timeout
	bk(MOT_LEFT);
	bk(MOT_RIGHT);
	while((_A == 0 || _B == 0) && (seconds()-time < 10))//while the bump sensors are false & it has been less than 10 seconds
	// move backwards, if the bot is bumped, turn the motor off, and break out of the loop
	{
		if (LBUMP){//if the left sensor is pressed
			off(MOT_LEFT);//turn towards wall
			_A = 1;
			} else {
			bk(MOT_LEFT);//otherwise, approach the wall more
			_A = 0;
		}
		
		if (RBUMP){//if the right sensor is pressed
			off(MOT_RIGHT);//turn towards wall
			_B = 1;
			} else {
			bk(MOT_RIGHT);//otherwise, approach the wall more
			_B = 0;
		}
		msleep(1);//give other processes time to do things
	}
	drive_off();//turn both motors off at end
}


void right(float degrees, float radius){//turn right a number of degrees with a certain radius
	degrees*=TURN_CONST;
	int turnrspeed;
	long turnl=(long)round((((2*radius+ks)*CMtoBEMF)*PI)*degrees/360.);
	long turnr=(long)round((((2*radius-ks)*CMtoBEMF)*PI)*degrees/360.);
	if(turnl == 0l) return;
	turnrspeed = round((float)turnr/(float)turnl*SPD);
	mrp(MOT_LEFT, SPD,turnl);
	if(turnrspeed < 0) turnrspeed = -turnrspeed;
	if(turnrspeed < 50){
		turnrspeed = 0;
		turnr = 0l;
		off(MOT_RIGHT);
		}else{
		mrp(MOT_RIGHT,turnrspeed,turnr);
	}
	bmd(MOT_RIGHT);
	bmd(MOT_LEFT);
}
void left(float degrees, float radius){//turn left a number of degrees with a certain radius
	degrees*=TURN_CONST;
	int turnlspeed;
	long turnl=(long)round((((2*radius-ks)*CMtoBEMF)*PI)*degrees/360.);
	long turnr=(long)round((((2*radius+ks)*CMtoBEMF)*PI)*degrees/360.);
	if(turnr == 0l) return;
	turnlspeed = round((float)turnl/(float)turnr*SPD);
	if(turnlspeed < 0) turnlspeed = -turnlspeed;
	if(turnlspeed < 50){
		turnlspeed = 0;
		turnl=0l;
		off(MOT_LEFT);
		}else{
		mrp(MOT_LEFT,turnlspeed,turnl);
	}
	mrp(MOT_RIGHT, SPD,turnr);
	bmd(MOT_LEFT);
	bmd(MOT_RIGHT);
}
void right_slow(float degrees, float radius, float spdmult){//turn right a number of degrees with a certain radius
	int turnrspeed;
	long turnl=(long)round((((2*radius+ks)*CMtoBEMF)*PI)*degrees/360.);
	long turnr=(long)round((((2*radius-ks)*CMtoBEMF)*PI)*degrees/360.);
	if(turnl == 0l) return;
	turnrspeed = round((float)turnr/(float)turnl*SPD);
	mrp(MOT_LEFT, SPD*spdmult,turnl);
	if(turnrspeed < 0) turnrspeed = -turnrspeed;
	if(turnrspeed < 50){
		turnrspeed = 0;
		turnr = 0l;
		off(MOT_RIGHT);
		}else{
		mrp(MOT_RIGHT,turnrspeed*spdmult,turnr);
	}
	bmd(MOT_RIGHT);
	bmd(MOT_LEFT);
}
void left_slow(float degrees, float radius, float spdmult){//turn left a number of degrees with a certain radius
	int turnlspeed;
	long turnl=(long)round((((2*radius-ks)*CMtoBEMF)*PI)*degrees/360.);
	long turnr=(long)round((((2*radius+ks)*CMtoBEMF)*PI)*degrees/360.);
	if(turnr == 0l) return;
	turnlspeed = round((float)turnl/(float)turnr*SPD);
	mrp(MOT_RIGHT, SPD*spdmult,turnr);
	if(turnlspeed < 0) turnlspeed = -turnlspeed;
	if(turnlspeed < 50){
		turnlspeed = 0;
		turnl=0l;
		off(MOT_LEFT);
		}else{
		mrp(MOT_LEFT,turnlspeed*spdmult,turnl);
	}
	bmd(MOT_LEFT);
	bmd(MOT_RIGHT);
}
void forward(float distance){//go forward a number of IN   NOT   CM    NOT    backEMF counts
	if(distance < 0l){
		distance = -distance;
	}
	long newdist;
	newdist = distance*CMtoBEMF*DIST_CORR;//conversion ratio
	msleep(100);
	mrp(MOT_RIGHT,SPDr,newdist*rdistmult);
	mrp(MOT_LEFT,SPDl,newdist);
	bmd(MOT_RIGHT);
	bmd(MOT_LEFT);
	/*int time=distance*DtoT;//in ms
	motor(MOT_RIGHT, PWRr);
	motor(MOT_LEFT, PWRl);
	msleep((int)(time));*/
	motor(MOT_RIGHT, 0);
	motor(MOT_LEFT, 0);
}
void forward_attack(float distance)//used when in attack configuration
{
	if(distance < 0l){
		distance = -distance;
	}
	int time=distance*DtoT;//in ms
	motor(MOT_RIGHT, PWRr);
	motor(MOT_LEFT, PWRl);
	msleep((int)(time));
	motor(MOT_RIGHT, 0);
	motor(MOT_LEFT, 0);
}
void start_forward(float percent_power)//just starts moving it forward, doesn't look for distance
{//percent power should be on (0, 1]
	motor(MOT_RIGHT, PWRr*percent_power);
	motor(MOT_LEFT, PWRl*percent_power);
}
void backward(float distance){//go backward a number of IN  NOT  CM    NOT    backEMF counts
	if(distance < 0l){
		distance = -distance;
	}
	/*long newdist;
	newdist = distance*CMtoBEMF*DIST_CORR;
	mrp(MOT_RIGHT,SPDr,-newdist*rdistmult);
	mrp(MOT_LEFT,SPDl,-newdist);
	bmd(MOT_RIGHT);
	bmd(MOT_LEFT);*/
	int time=distance*DtoT;//in ms
	motor(MOT_RIGHT, -PWRr);
	motor(MOT_LEFT, -PWRl);
	msleep(time);
	motor(MOT_RIGHT, 0);
	motor(MOT_LEFT, 0);
}

// to do: add two threshold values. >:( 

#define BLACK_SEN_THRESH 810
#define LEFT_BLACK (analog10(1)>BLACK_SEN_THRESH)
#define RIGHT_BLACK (analog10(0)>BLACK_SEN_THRESH)
//#define HIGH_POWER 1000
//#define LOW_POWER 500

void s_line_follow(float distance) //dist in cm
{
	int high = 1000;
	int low = 500;
	long position = get_motor_position_counter(MOT_RIGHT) + distance*CMtoBEMF;
	while(get_motor_position_counter(MOT_RIGHT)<=position)
	{
		msleep(1);
		//if both sensors do not detect black, drive forward
		//if the left sensor detects black, turn right
		//if the right sensor detects black, turn left
		if(!LEFT_BLACK && !RIGHT_BLACK)
		{
			mav(MOT_LEFT,high);
			mav(MOT_RIGHT,high);
		}
		else if(LEFT_BLACK)
		{
			mav(MOT_LEFT,low);
			mav(MOT_RIGHT,high);
		}
		else if(RIGHT_BLACK)
		{
			mav(MOT_RIGHT,low);
			mav(MOT_LEFT,high);
		}
	}
}

void f_line_follow(float distance)
{
	int spd = 800;
	long position = get_motor_position_counter(MOT_RIGHT) + distance*CMtoBEMF;
	while(get_motor_position_counter(MOT_RIGHT)<=position)
	{
		msleep(1);
		//if both sensors do not detect black, drive forward
		//if the left sensor detects black, turn right
		//if the right sensor detects black, turn left
		if(!LEFT_BLACK && !RIGHT_BLACK)
		{
			fd(MOT_LEFT);
			fd(MOT_RIGHT);
		}
		else if(LEFT_BLACK)
		{
			mav(MOT_LEFT,spd);
			fd(MOT_RIGHT);
		}
		else if(RIGHT_BLACK)
		{
			mav(MOT_RIGHT,spd);
			fd(MOT_LEFT);
		}
	}
}


/*
1,1-> 1
0,1-> 0
1,0-> 0
0,0-> 0
*/

#define TOUCH_FRONT digital(15)

void line_follow_touch()
{
	int high = 1000;
	int low = 500;
	long position = get_motor_position_counter(MOT_RIGHT) + 35*CMtoBEMF;
	while(!(TOUCH_FRONT || (get_motor_position_counter(MOT_RIGHT)>position)))
	{
		msleep(1);
		//if both sensors do not detect black, drive forward
		//if the left sensor detects black, turn right
		//if the right sensor detects black, turn left
		if(!LEFT_BLACK && !RIGHT_BLACK)
		{
			mav(MOT_LEFT,high);
			mav(MOT_RIGHT,high);
		}
		else if(LEFT_BLACK)
		{
			mav(MOT_LEFT,low);
			mav(MOT_RIGHT,high);
		}
		else if(RIGHT_BLACK)
		{
			mav(MOT_RIGHT,low);
			mav(MOT_LEFT,high);
		}
	}
}

#endif

void line_square(int col)//1 = BLACK, 0 = white
{
	int _A = 0,_B = 0;
	float time = seconds();//used for timeout
	while((_A == 0 || _B == 0) && (seconds()-time < 3))//while the bump sensors are false & it has been less than 10 seconds
	// move backwards, if the bot is bumped, turn the motor off, and break out of the loop
	{
		if ((LLIGHT>BLACK)==col){//if the left sensor is pressed
			motor(MOT_LEFT,-10);//turn towards wall
			_A = 1;
			} else {
			motor(MOT_LEFT,HSPDl);//otherwise, approach the wall more
			_A = 0;
		}
		
		if ((RLIGHT>BLACK)==col){//if the right sensor is pressed
			motor(MOT_RIGHT,-10);//turn towards wall
			_B = 1;
			} else {
			motor(MOT_RIGHT,HSPDr);//otherwise, approach the wall more
			_B = 0;
		}
		msleep(1);//give other processes time to do things
		//printf("L: %d, R: %d\n",LLIGHT, RLIGHT);
	}
	drive_off();//turn both motors off at end
	//printf("FINISHED\n");
}

void test_driving()
{
	WAIT(!(a_button()||b_button()||c_button()||x_button()||y_button()||z_button()));
	extra_buttons_show();
	set_a_button_text("right 90");
	set_b_button_text("left 90");
	set_c_button_text("forward 10 in");
	set_x_button_text("backward 10 in");
	set_y_button_text("forward 30 in");
	set_z_button_text("backward 30 in");
	println("choose an option to test");
	WAIT(a_button()||b_button()||c_button()||x_button()||y_button()||z_button());
	if(a_button())
	{
		right(90,0);
	}
	else if(b_button())
	{
		left(90, 0);
	}
	else if(c_button())
	{
		forward(10);
	}
	else if(x_button())
	{
		backward(10);
	}
	else if(y_button())
	{
		forward(30);
	}
	else//z
	{
		backward(30);
	}
	WAIT(!(a_button()||b_button()||c_button()||x_button()||y_button()||z_button()));
	extra_buttons_hide();
	set_a_button_text("Go again");
	set_b_button_text("Quit testing");
	set_c_button_text("Don't press me");
	WAIT(a_button()||b_button()||c_button());
	if(a_button())
	{
		test_driving();
	}
	else if(b_button())
	{
		reset_buttons();
	}//after this, will exit testing by reaching end of function
	else//c
	{
		println("I told you not to press that button...");
		msleep(1000);
		brick();//huehuehuehue
	}
}

#define SERVO_LOCK 2
#define UNLOCKED 1150
#define LOCKED 500
void lock_igus()
{
	set_servo_position(SERVO_LOCK, LOCKED);
}
void unlock_igus()
{
	set_servo_position(SERVO_LOCK, UNLOCKED);
}
