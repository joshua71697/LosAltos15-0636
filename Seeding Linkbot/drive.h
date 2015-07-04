#ifndef _DRIVE_H_
#define _DRIVE_H_

#include "generic.h"


//primary driving code
#define MOT_LEFT 0
#define MOT_RIGHT 3
#define PI 3.14159265358979323846264338327950288419716939937510582097494459//yea, get rekt
#define R_DIST_CONST 1.02//distance constants-->how far you tell it to move/how far it actually moves
#define L_DIST_CONST 1.01//
#define PID_CONST .4//how much the motor speeds change based off how far off each motor is
#define END_THRESHOLD 460.//at what point the "end" starts-->460 is 1/2 turn (~4 inches for the standard wheels)
#define END_SCALE .9//how much the motor slows down at the end (=final speed)
//if you don't want it to slow down at the end, set END_THRESHOLD to 0. or END_SCALE to 1.

#define ks 15//distance from one wheel to another in cm
#define wheeldiameter 6//this is in cm

#define CMtoBEMF (921/wheeldiameter/PI) //number of units per rotation divided by circumference
#define INtoCM 2.5
#define DEGtoRAD PI/180//convert from degrees to radians
#define TIMEOUT 30//how many ms the timeout is per tick the motor has to travel

//line following/squareup
#define LLIGHT analog(3)
#define RLIGHT analog(2)
#define THRESHOLD 600//below is black, above is white

#define drive_off() off(MOT_RIGHT) ;off(MOT_LEFT)

void back(float distance, int power);//function declarations
void forward(float distance, int power);//
void right(float degrees, float radius, int power);//
void left(float degrees, float radius, int power);//
void drive(float l_ticks, float r_ticks, float max_pwr);//


void back_line_follow(float distance, int power)//follows a black line while going backwards, end based off distance
{//distance in inches, power 1-100
	if(distance==0||power==0)//sanitize results
		return;
	distance=my_abs(distance);//
	power=my_abs(power);//
	int ticks=round(distance*INtoCM*CMtoBEMF*(R_DIST_CONST+L_DIST_CONST)/2);//total number of ticks the motors go-->look at average of left and right motors
	cmpc(MOT_LEFT);
	cmpc(MOT_RIGHT);
	motor(MOT_LEFT, -power);//start moving at max power
	motor(MOT_RIGHT, -power);
	while(my_abs(gmpc(MOT_LEFT))+my_abs(gmpc(MOT_RIGHT))<ticks*2)//go until the average ticks is the goal
	{
		if((LLIGHT>=THRESHOLD&&RLIGHT>=THRESHOLD)||(LLIGHT<=THRESHOLD&&RLIGHT<=THRESHOLD))
		{//both black or both white-->lined up (probs)
			motor(MOT_LEFT, -power);//so run at full power
			motor(MOT_RIGHT, -power);//
		}
		else if(LLIGHT<=THRESHOLD)//only left is black-->left is too far forward
		{
			motor(MOT_LEFT, -power/2);//so slow down left
			motor(MOT_RIGHT, -power);//
		}
		else//only right is black-->right is too far forward
		{
			motor(MOT_LEFT, -power);//so slow down right
			motor(MOT_RIGHT, -power/2);//
		}
		msleep(10);
	}
	drive_off();
}

void back_line_follow_time(int power, int time)//following a line for a given amount of time
{//time in ms, power 1-100
	if(time==0||power==0)//sanitize results
		return;
	time=my_abs(time);//
	power=my_abs(power);//
	motor(MOT_LEFT, -power);//start moving at max power
	motor(MOT_RIGHT, -power);
	float start_time=curr_time();
	while((curr_time()-start_time)*1000<time)//go for the amount of time specified
	{
		if((LLIGHT>=THRESHOLD&&RLIGHT>=THRESHOLD)||(LLIGHT<=THRESHOLD&&RLIGHT<=THRESHOLD))
		{//both black or both white-->lined up (probs)
			motor(MOT_LEFT, -power);//so run at full power
			motor(MOT_RIGHT, -power);//
		}
		else if(LLIGHT<=THRESHOLD)//only left is black-->left is too far forward
		{
			motor(MOT_LEFT, -power/2);//so slow down left
			motor(MOT_RIGHT, -power);//
		}
		else//only right is black-->right is too far forward
		{
			motor(MOT_LEFT, -power);//so slow down right
			motor(MOT_RIGHT, -power/2);//
		}
		msleep(10);
	}
	drive_off();
}

void physical_squareup(boolean forward)//true means square up on the front, false means back
{
	int direction=1;//forward (-1 is back)
	if(!forward)
		direction=-1;
	motor(MOT_LEFT, 40*direction);
	motor(MOT_RIGHT, 40*direction);
	msleep(1000);
	drive_off();
}

void test_driving()
{
	WAIT(!(a_button()||b_button()||c_button()||x_button()||y_button()||z_button()));
	extra_buttons_show();
	set_a_button_text("left");
	set_b_button_text("forward");
	set_c_button_text("right");
	set_x_button_text("back left");
	set_y_button_text("back");
	set_z_button_text("back right");
	int choice;
	display_clear();
	display_printf(0,0,"choose an option to test");
	WAIT(a_button()||b_button()||c_button()||x_button()||y_button()||z_button());
	if(a_button())
		choice=1;//yea, I should use #defines, but whatevs...
	else if(b_button())
		choice=2;
	else if(c_button())
		choice=3;
	else if(x_button())
		choice=4;
	else if(y_button())
		choice=5;
	else//z
		choice=6;
	WAIT(!(a_button()||b_button()||c_button()||x_button()||y_button()||z_button()));
	if(choice==2||choice==5)//forward or back
	{
		display_printf(0,1,"input distance");
		float distance=input_float(0,2);//in generic.h
		display_printf(0,3,"input power");
		int power=input_int(0,4);
		if(choice==2)//forward
			forward(distance, power);
		else//back
			back(distance, power);
	}
	else//turns
	{
		display_printf(0,1,"input angle");
		float degrees=input_float(0,2);
		display_printf(0,3,"input radius");
		float radius=input_float(0,4);
		display_printf(0,5,"input power");
		int power=input_int(0,6);
		if(choice==1)//left
			left(degrees, radius, power);
		else if(choice==3)//right
			right(degrees, radius, power);
		else if(choice==4)//back left
			left(-degrees, radius, power);
		else//back right
			right(-degrees, radius, power);
	}
	extra_buttons_hide();
	display_clear();
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
		printf("I told you not to press that button...\n");
		msleep(1000);
		brick();//huehuehuehue
	}
}

void time_drive(int lspeed, int rspeed, int time)//speed 0-100, time in ms
{//-->drive at the given speeds for the given time
	motor(MOT_LEFT, lspeed);
	motor(MOT_RIGHT, rspeed);
	msleep(time);
	drive_off();
}

void forward(float distance, int power)//distance in inches, power on [1,100]
{
	if(distance==0||power==0)//not helpful-->don't move
		return;
	if(distance<0)//negative distance means go backwards
	{
		back(-distance, power);//- to make it positive
		return;
	}
	if(power<0)//same deal with negative power
	{
		back(distance, -power);
		return;
	}
	if(power>100)//can't go above 100
		power=100;//so go at max power
	float r_dist=distance*INtoCM*CMtoBEMF*R_DIST_CONST;//total backEMF counts it needs to go
	float l_dist=distance*INtoCM*CMtoBEMF*L_DIST_CONST;//
	drive(l_dist, r_dist, (float)(power));
}

void back(float distance, int power)//same params as forward
{
	if(distance==0||power==0)//not helpful-->don't move
		return;
	if(distance<0)//negative distance means go fowards
	{
		forward(-distance, power);//- to make it positive
		return;
	}
	if(power<0)//same deal with negative power
	{
		forward(distance, -power);
		return;
	}
	if(power>100)//can't go above 100
		power=100;//so go at max power
	float r_dist=distance*INtoCM*CMtoBEMF*R_DIST_CONST;//total backEMF counts it needs to go
	float l_dist=distance*INtoCM*CMtoBEMF*L_DIST_CONST;//
	drive(-l_dist, -r_dist, (float)(power));//negative cause it's going backwards
}

void right(float degrees, float radius, int power)//radius in inches, power on [1,100]
{
	boolean backwards=false;//whether or not it goes backwards
	if(degrees==0||power==0)//can't move-->exit
		return;
	if(radius<0)//negative radius means turn left instead
	{
		left(degrees, -radius, power);
		return;
	}
	if(degrees<0||power<0)//negative in either means turn while going backwards-->treated differently
	{
		backwards=true;//it's going backwards!
		degrees=my_abs(degrees);//make both positive
		power=my_abs(power);//
	}
	if(power>100)//can't do more than 100
		power=100;//so just do 100
	//these formulas work in all four cases (center of rotation at center of mass, under robot, under wheel, or outside of robot)
	float r_dist=-degrees*DEGtoRAD*((ks/2)-(radius*INtoCM))*CMtoBEMF*R_DIST_CONST;//target position for the right wheel
	float l_dist=degrees*DEGtoRAD*((ks/2)+(radius*INtoCM))*CMtoBEMF*L_DIST_CONST;//left
	if(backwards)//if it needs to go backwards...
	{
		r_dist=-r_dist;//make them negative
		l_dist=-l_dist;//so it goes backwards
	}
	drive(l_dist, r_dist, (float)(power));
}
	
void left(float degrees, float radius, int power)//same params as right
{
	boolean backwards=false;//whether or not it is going backwards...
	if(degrees==0||power==0)//can't move-->exit
		return;
	if(radius<0)//negative radius means turn right instead
	{
		right(degrees, -radius, power);
		return;
	}
	if(degrees<0||power<0)//negative in either means turn while going backwards-->treated differently
	{
		backwards=true;//it's going backwards!
		degrees=my_abs(degrees);//make both positive
		power=my_abs(power);//
	}
	if(power>100)//can't do more than 100
		power=100;//so just do 100
	//these formulas work in all four cases (center of rotation at center of mass, under robot, under wheel, or outside of robot)
	float l_dist=-degrees*DEGtoRAD*((ks/2)-(radius*INtoCM))*CMtoBEMF*L_DIST_CONST;//target position for the left wheel
	float r_dist=degrees*DEGtoRAD*((ks/2)+(radius*INtoCM))*CMtoBEMF*R_DIST_CONST;//right
	if(backwards)//if it needs to go backwards...
	{
		r_dist=-r_dist;//make it negative
		l_dist=-l_dist;//so it goes backwards
	}
	drive(l_dist, r_dist, (float)(power));
}

void drive(float l_ticks, float r_ticks, float max_pwr)//ticks each motor has to travel (+ or -), power of the fast motor (+)
{//NOTE: You should probably never call this directly--it should only be called through forward/back/left/right
	drive_off();//make sure both motors are off from the start--just to be safe
	if(max_pwr==0||(r_ticks==0&&l_ticks==0))//don't move or move at 0 speed
		return;//can't do anything-->just exit
	if(max_pwr<0)//negative power-->needs to be positive
	{
		max_pwr=-max_pwr;//make it positive
		l_ticks=-l_ticks;//switch the signs of the motor movements
		r_ticks=-r_ticks;//
	}
	float l_base_pwr;//power each motor moves at if everything is going as intended
	float r_base_pwr;//
	int timeout;//in ms, based off how far the fast motor has to travel-->if it reaches the timeout, it will stop
	if(my_abs(l_ticks)>my_abs(r_ticks))//left motor is moving faster
	{
		l_base_pwr=max_pwr*sign(l_ticks);//run left at max power in the right direction
		r_base_pwr=max_pwr*(r_ticks/my_abs(l_ticks));//run right at lower speed, proportional to distance to travel
		timeout=my_abs(l_ticks)*TIMEOUT;
	}
	else//< (can also be equal, but then the division term will end up being 1, so it works out)
	{
		r_base_pwr=max_pwr*sign(r_ticks);//see above
		l_base_pwr=max_pwr*(l_ticks/my_abs(r_ticks));//
		timeout=my_abs(r_ticks)*TIMEOUT;
	}
	cmpc(MOT_LEFT);
	cmpc(MOT_RIGHT);
	int start_time=curr_time();
	if(l_ticks==0)//means the right motor moves but the left doesn't
	{//I know they can't both be 0 because I sanitized my results at the beginning
		while(my_abs(gmpc(MOT_RIGHT))<my_abs(r_ticks)&&curr_time()-start_time<timeout)
		{
			float r_pwr=r_base_pwr;//actual power it will turn at
			float r_left=my_abs(r_ticks)-my_abs(gmpc(MOT_RIGHT));//ticks remaining
			if(r_left<END_THRESHOLD)//needs to start slowing down
				r_pwr*=END_SCALE+(r_left*(1-END_SCALE)/END_THRESHOLD);//scales from full power to END_SCALE
			motor(MOT_RIGHT, round(r_pwr));
			msleep(10);
		}
	}
	else if(r_ticks==0)//ditto for if the right doesn't move
	{
		while(my_abs(gmpc(MOT_LEFT))<my_abs(l_ticks)&&curr_time()-start_time<timeout)
		{
			float l_pwr=l_base_pwr;//see comments above
			float l_left=my_abs(l_ticks)-my_abs(gmpc(MOT_LEFT));
			if(l_left<END_THRESHOLD)
				l_pwr*=END_SCALE+(l_left*(1-END_SCALE)/END_THRESHOLD);
			motor(MOT_LEFT, round(l_pwr));
			msleep(10);
		}
	}
	else//both motors have to move
	{
		while((my_abs(gmpc(MOT_LEFT))<my_abs(l_ticks)||my_abs(gmpc(MOT_RIGHT))<my_abs(r_ticks))&&curr_time()-start_time<timeout)//one isn't finished
		{
			float l_pwr=l_base_pwr;//actual power
			float r_pwr=r_base_pwr;//
			float l_left=my_abs(l_ticks)-my_abs(gmpc(MOT_LEFT));//ticks remaining
			float r_left=my_abs(r_ticks)-my_abs(gmpc(MOT_RIGHT));//
			float l_perc_left=l_left/my_abs(l_ticks);//percent of distance remaining-->on [0,1]
			float r_perc_left=r_left/my_abs(r_ticks);//
			r_pwr+=(r_perc_left-l_perc_left)*r_ticks*PID_CONST;//increases in magnitude if motor is behind,
			l_pwr+=(l_perc_left-r_perc_left)*l_ticks*PID_CONST;//or slows down if it's ahead
			if(l_left<END_THRESHOLD)//needs to slow down
				l_pwr*=END_SCALE+(l_left*(1-END_SCALE)/END_THRESHOLD);
			if(r_left<END_THRESHOLD)//same for the right
				r_pwr*=END_SCALE+(r_left*(1-END_SCALE)/END_THRESHOLD);
			if(r_left<=0||sign(r_pwr)!=sign(r_base_pwr))//has reached end or so far off that it has to stop entirely
				r_pwr=0;
			if(l_left<=0||sign(l_pwr)!=sign(l_base_pwr))//ditto for the left
				l_pwr=0;
			motor(MOT_LEFT, round(l_pwr));
			motor(MOT_RIGHT, round(r_pwr));
			msleep(10);
		}
	}
	drive_off();//stop it at the end
}
#endif
