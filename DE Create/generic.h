

#define RIGHT_TOUCH 15
#define LEFT_TOUCH 14

#define LID 0
#define LID_UP 760
#define LID_INIT 500
#define LID_MID 1000
#define LID_ALL 2040
#define LID_DOWN 1700

#define SWEEPER 2
// in = score
#define SWEEPER_IN 0
#define SWEEPER_OUT 1000
#define SWEEPER_INIT 500
#define SWEEPER_ALL 2000

#define HELPER_MOTOR 0
#define HELPER_MOTOR_UP_SPEED 100
#define HELPER_MOTOR_DOWN_SPEED -100

#define A 0
#define B 1
#define C 2

#define LIGHT 0

void touchSquareUp(int speed)
{
	float time = seconds();//creates the variable time, which is equal to the seconds the program has been running
	
	speed = speed *-1;
	create_drive_direct(speed, speed);
	while (seconds()-time < 5)
	{
		int rTouch = digital(RIGHT_TOUCH);
		int lTouch = digital(LEFT_TOUCH);
		if (seconds()-time > 4)
		{
			printf("gonna bail");
		}
		if ( (rTouch == 0) && (lTouch == 0) )
		{
			//neither pressed
			create_drive_direct(speed, speed);
		}
		else if ( (rTouch == 1) && (lTouch == 0) )
		{
			//right is pressed, left isn't
			create_drive_direct( speed, 0);
		}
		else if ( (rTouch == 0) && (lTouch == 1) )
		{
			// left is pressed, right isn't
			create_drive_direct (0, speed);
		}
		else 
		{
			printf("I'm done!");
			create_stop();
			break;
		}
	}
	printf(" \n that took %d seconds", (seconds()-time));
}

void squareUp(int millWait)
{
	int squareUpSpeed = 9;
	create_drive_direct(-squareUpSpeed,-squareUpSpeed);
	msleep (millWait);
}


//UTILITY
float bound(float num, float max)
{
	if (num > max)
	return max;
	else if (num < -max)
	return -max;
	else
	return num;
}
int in_range(int input, int wanted, int fudge)
{
	if (wanted+fudge >= input && wanted-fudge <= input)
	return 1;
	else
	return 0;
}
float ET_distance(int port){
	return ((sqrt(100.0/analog10(port)))-2.2);
}

#define WAIT(thing); while(!(thing)){msleep(1);}
#define LIMIT(thing,time); {double _tmptime = seconds()+time; while(!(thing) && (_tmptime > seconds())){msleep(1);}}
#define gmpc(port) get_motor_position_counter(port)

//LIGHT START
#define light_s() analog10(sensor)
void light_start(int sensor)
{
	WAIT(!(b_button() || c_button()));
	int max = 0,min = 9999,curr = 0,avg = 0;
	display_clear();
	display_printf(0,0,"Max:");
	display_printf(0,1,"Min:");
	display_printf(0,2,"Curr:");
	display_printf(0,3,"avg:");
	while(!b_button() && !c_button())//INTERACTION BUTTON
	{
		curr = light_s();
		if (curr > max)
		{max = curr;}
		if (curr < min)
		{min = curr;}
		avg = (max+min)/2;
		display_printf(5,0,"%d   ",max);
		display_printf(5,1,"%d   ",min);
		display_printf(6,2,"%d   ",curr);
		display_printf(5,3,"%d   ",avg);
		if (curr > avg)
		{
			display_printf(10,5,"XX");
		}
		else
		{
			display_printf(10,5,"OO");
		}
		msleep(50);
	}
	display_clear();
	display_printf(0,4,"Prepared to begin: left to skip");
	while(light_s() > avg && !(c_button()))
	{
		msleep(50);
	}
}

//TIMING
long _start_time;
void start()
{
	_start_time = systime();
}
float curr_time()
{
	return (systime()-_start_time)/1000.0;
}
void now()     
{
	printf("now %f\n",curr_time());
}
void nowstr(char *s)
{
	printf("%s %f\n",s,curr_time());
}


void wait_till(float t)
{
	now();
	msleep((long)((t-curr_time())*1000));
}

//servo set takes time in seconds

//SERVOS
//servo_off(int) is replaced with the KIPR library function disable_servo(int)
void servo_set(int port,int end,float time)//,float increment)
{//position is from 0-2047
	float increment = .01;
	//printf("servo %d",port);
	float curr,start = get_servo_position(port);
	float i = ((end-start)/time)*increment;
	curr = start;
	if (start > end)
	{
		while(curr > end)
		{
			//printf("%f\n",curr);
			set_servo_position(port,curr);
			curr+=i;
			msleep((long)(increment*1000));
		}
	}
	else if (start < end)
	{
		while(curr < end)
		{
			//printf("%f\n",curr);
			set_servo_position(port,curr);
			curr+=i;
			msleep((long)(increment*1000));
		}
	}
	set_servo_position(port,end);
}

//camera stuff

int cam_area(int channel){//returns largest blob in channel, or 0 if none
	if (get_object_count(channel) > 0){
		return get_object_area(channel,0);
	}
	return 0;
}
void update_wait(){//updates the camera, and waits until success
	while(!camera_update()) msleep(1);
}

int currstate;
#define state(State) if (currstate == State)
#ifndef MENUa
#define next(State); {currstate = State;}
#else//If menu code is in use, print the time of each state switch
	extern struct menuitem menu[];
	void next(int State) {
	int i;
	currstate = State;
	i = -1;
	while (!strcmp(menu[++i].name,"FIN")){
	if (menu[i].snum==State){
	nowstr(menu[i].name);
	return;
	}
	}
	now();
	}
#endif


int getabbutton(){//returns 0,1 on a,b
	WAIT(!(a_button() || b_button()));
	WAIT(a_button() || b_button());
	if (a_button()) return 0;
	if (b_button()) return 1;
	printf("ERROR!");beep();
	msleep(2000);beep();
	return 0;//if something broke
}
int getabcbutton(){//returns 0,1,2 on a,b,c
	WAIT(!(a_button() || b_button() || c_button()));
	WAIT(a_button() || b_button() || c_button());
	if (a_button()) return 0;
	if (b_button()) return 1;
	if (c_button()) return 2;
	printf("ERROR!");beep();
	msleep(2000);beep();
	return 0;//if something broke
}
void sweep()
{
	int count = 0;
	while (count < 4)
	{
		servo_set(SWEEPER,SWEEPER_IN, 2.5);
		servo_set(SWEEPER,SWEEPER_OUT, 2.5);
		create_drive_direct(300,300);
		msleep(10);
		create_drive_direct(-300,-300);
		msleep(10);
		create_stop();
		count++;
	}
}

void shake()
{
	int count = 0;
	while(count < 4)
	{
		create_drive_direct(300,-300);
		msleep(30);
		create_drive_direct(-300,300);
		msleep(30);
		create_stop();
		count++;
	}
}

