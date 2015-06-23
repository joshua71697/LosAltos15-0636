#ifndef _DRIVE_H_
#define _DRIVE_H_

#include "generic.h"


//primary driving code
#define MOT_LEFT 0
#define MOT_RIGHT 3
#define PI 3.14159265358979323846264338327950288419716939937510582097494459//yea, get rekt
#define HSPDl 20//used for line squareups
#define HSPDr 18
#define PID_CONST .4//how much the motor speeds change based off how far off each motor is
#define END_THRESHOLD 460.//at what point the "end" starts-->460 is 1/2 turn (~4 inches for the standard wheels)
#define END_SCALE .4//how much the motor slows down at the end (=final speed)
//if you don't want it to slow down at the end, set END_THRESHOLD to 0. or END_SCALE to 1.

#define ks 15//distance from one wheel to another in cm
#define wheeldiameter 6//this is in cm

#define CMtoBEMF (921/wheeldiameter/PI) //number of units per rotation divided by circumference
#define INtoCM 2.5
#define DEGtoRAD PI/180//convert from degrees to radians

#define LBUMP digital(14)
#define RBUMP digital(15) //left/right back bump sensors (used for square_back())

#define drive_off() off(MOT_RIGHT) ;off(MOT_LEFT)

#define LLIGHT analog(15)
#define RLIGHT analog(10)
#define BLACK 750

void back(float distance, int power);//function declarations
void forward(float distance, int power);//
void right(float degrees, float radius, int power);//
void left(float degrees, float radius, int power);//
void drive(float l_ticks, float r_ticks, float max_pwr);//

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

// to do: add two threshold values. >:( 
#define BLACK_SEN_THRESH 810
#define LEFT_BLACK (analog10(1)>BLACK_SEN_THRESH)
#define RIGHT_BLACK (analog10(0)>BLACK_SEN_THRESH)

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

#define R_DIST_CONST 1.//distance constants-->how far you tell it to move/how far it actually moves
#define L_DIST_CONST 1.//
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
	if(my_abs(l_ticks)>my_abs(r_ticks))//left motor is moving faster
	{
		l_base_pwr=max_pwr*sign(l_ticks);//run left at max power in the right direction
		r_base_pwr=max_pwr*(r_ticks/my_abs(l_ticks));//run right at lower speed, proportional to distance to travel
	}
	else//< (can also be equal, but then the division term will end up being 1, so it works out)
	{
		r_base_pwr=max_pwr*sign(r_ticks);//see above
		l_base_pwr=max_pwr*(l_ticks/my_abs(r_ticks));//
	}
	cmpc(MOT_LEFT);
	cmpc(MOT_RIGHT);
	if(l_ticks==0)//means the right motor moves but the left doesn't
	{//I know they can't both be 0 because I sanitized my results at the beginning
		while(my_abs(gmpc(MOT_RIGHT))<my_abs(r_ticks))
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
		while(my_abs(gmpc(MOT_LEFT))<my_abs(l_ticks))
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
		while(my_abs(gmpc(MOT_LEFT))<my_abs(l_ticks)||my_abs(gmpc(MOT_RIGHT))<my_abs(r_ticks))//one isn't finished
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
