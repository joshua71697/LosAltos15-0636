#define PING_ARM 0
#define PING_ARM_UP_SPEED 100
#define PING_ARM_DOWN_SPEED -100
#define PING_ARM_UP_POS 1000
#define PING_ARM_DOWN_POS (100 - PING_ARM_UP_POS) 

#define BLOCK_ARM 2
#define BLOCK_ARM_UP_SPEED -100
#define BLOCK_ARM_DOWN_SPEED 100
#define BLOCK_ARM_SPEED 100
#define BLOCK_ARM_UP_POS -500
#define BLOCK_ARM_HOVER_POS -150
#define BLOCK_ARM_MID_POS -60
//-2700 is straight up
//this position is relative to down being 0

#define BLOCK_CLAW 3
#define BLOCK_CLAW_CLOSED 1600
#define BLOCK_CLAW_OPEN 300


#define PING_GATE 0
#define PING_GATE_CLOSED 2000
#define PING_GATE_OPEN 1047


#define RIGHT_TOUCH 15
#define LEFT_TOUCH 14
#define LIGHT_START 0

//menu
#define A 0
#define B 1
#define C 2

#define TESTING 0
#define MAIN 1
#define HALF 2


// speeds defined: {slowSpeed,normalSpeed, fastSpeed} out of 500
#define SS 100
#define NS 300
#define FS 500
#define TS 100

void deploy()
{
	enable_servos();
	set_servo_position(PING_GATE, PING_GATE_OPEN);
	msleep(2500);
	
}
void printMotorPos(int port)
{
	int pos = get_motor_position_counter(port);
	pos = get_motor_position_counter(PING_ARM);
	printf("\n pos: %d", pos);	
}
void moveMotor(int port, int speed, int goalPos)
{
	motor(port, speed);
	clear_motor_position_counter(port);
	int pos = get_motor_position_counter(port);
	if ( goalPos > 0 )
	{
		while( pos < goalPos )
		{
			pos = get_motor_position_counter(port);
			printMotorPos(port);
		}
	}
	else 
	{
		while( pos > goalPos )
		{
			pos = get_motor_position_counter(port);
			printMotorPos(port);
		}
	}
	ao();
	msleep(500);
}
void blockArmUp()
{
	moveMotor(BLOCK_ARM, BLOCK_ARM_UP_SPEED, BLOCK_ARM_UP_POS);
	move_relative_position(BLOCK_ARM, BLOCK_ARM_UP_SPEED*10, BLOCK_ARM_UP_POS); 
	msleep(2000);
}
void pingArmUp()
{
	motor(PING_ARM, PING_ARM_UP_SPEED);
	clear_motor_position_counter(PING_ARM);
	int pos = get_motor_position_counter(PING_ARM);
	while( pos < PING_ARM_UP_POS )
	{
		pos = get_motor_position_counter(PING_ARM);
		printf("\n pos: %d", pos);
	}
	ao();
	msleep(500);
}
void blockArmDown()
{
	move_relative_position(BLOCK_ARM, BLOCK_ARM_DOWN_SPEED*10, -BLOCK_ARM_UP_POS); 
	msleep(2000);
}
void pingArmDown()
{
	motor(PING_ARM, PING_ARM_DOWN_SPEED);
	clear_motor_position_counter(PING_ARM);
	int pos = get_motor_position_counter(PING_ARM);
	while( pos > PING_ARM_DOWN_POS )
	{
		pos = get_motor_position_counter(PING_ARM);
		printf("\n pos: %d", pos);
	}
	ao();
	msleep(1000); // longer
}
void timeBlockDown(int time)
{
	create_block();
	motor(BLOCK_ARM, BLOCK_ARM_DOWN_SPEED);
	msleep(time);
	off(BLOCK_ARM);
}
void timeBlockUp(int time)
{
	create_block();
	motor(BLOCK_ARM, BLOCK_ARM_UP_SPEED);
	msleep(time);
	off(BLOCK_ARM);
}
void shake(int count)
{
	int i = 0;
	backward_time(2000,SS);
	create_forward(1,NS);
	while (i < count)
	{
		//create_forward(1,FS);
		//create_backward(1,FS);
		create_right(1,1,TS);
		create_left(1,1,TS);	
		i++;
	}
	create_block();
}
//helper method for "scorePing()"
void upDown()
{
	msleep(500);
	pingArmUp();
	create_block();
	create_forward(3.5,SS);
	create_block();
	msleep(500);
	pingArmDown();
}
void scorePing()
{
	upDown();
	create_forward(7.5,SS);
	create_block();
	msleep(500);
}
int menu()
{
	printf("A for testing, B for main\n");
	int button = getabcbutton();
	if (button == A)
	{
		printf("What do you want to test?\n");
		printf("  A for ping arm \n  B for block arm\n  C for drive");
		while(1)
		{
			button = getabcbutton();
			msleep(500);
			if (button == A)
			{
				printf("\n=========\n");
				printf("  A for ping arm up\n  B for ping arm down");
				button = getabcbutton();
				if (button == A)
				pingArmUp();
				else if (button == B)
				pingArmDown();
			}
			else if (button == B)
			{
				printf("\n=========\n");
				printf("  A for block arm up\n  B for block arm down");
				button = getabcbutton();
				if (button == A)
				blockArmUp();
				else if (button == B)
				blockArmDown();
			}
			else if (button == C)
			{ 
				printf("\n=========\n");
				printf("  A for forward\n  B backward\n  C for right spin");
				button = getabcbutton();
				create_connect();
				if (button == A)
				create_drive_direct(100,100);
				else if (button == B)
				create_drive_direct(-100,-100);
				else if (button == B)
				create_drive_direct(-100,100);
			}
		}
		//testing code here.
		return TESTING;
	}
	else if (button == B)
	{
		printf("Running in main\n");
		return MAIN;
	}
	else if (button == C)
	{
		printf("Running ONLY SECOND HALF");
		return HALF;
	}
	
}


void init()
{
	printf("\n init");
	// wait_for_light(LIGHT_START);
	//shut_down_in(119.5);
	clear_motor_position_counter(BLOCK_ARM);
	clear_motor_position_counter(PING_ARM);
	printf("\n asdfjkl");
	enable_servos();
	set_servo_position(PING_GATE, PING_GATE_CLOSED);
	set_servo_position(BLOCK_CLAW, BLOCK_CLAW_OPEN);
	printf("\n 12312312");
	msleep(200);
	create_connect();
	printf("\n nmojimoknjimoknij");
	backward_time(1000,SS); //square up first.
}
void blockClawClose()
{
	set_servo_position(BLOCK_CLAW, BLOCK_CLAW_CLOSED);
	msleep(300);
}
void blockClawOpen()
{
	set_servo_position(BLOCK_CLAW, BLOCK_CLAW_OPEN);
	msleep(300);
}

void cb()
{
	create_block();
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

int wait_time;
/*
void wait_shutdown()
{
	msleep(wait_time);
	printf("ended based off time\n");
	create_disconnect();
	_exit(0);//this throws a warning, but hey, it still compiles, so whatevs
}
void shutdownin(float time)//cause raisins.
{
	wait_time=(int)(time*1000);
	thread tw=thread_create(wait_shutdown);
	thread_start(tw);
}*/
