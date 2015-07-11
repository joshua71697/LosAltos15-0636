#define boolean int
#define true 1
#define false 0

#define WAIT(thing); while(!(thing)){msleep(1);}
#define LIMIT(thing,time); {double _tmptime = seconds()+time; while(!(thing) && (_tmptime > seconds())){msleep(1);}}
#define gmpc get_motor_position_counter
#define cmpc clear_motor_position_counter
#define light_s() analog10(sensor)//this is used in lightstart

//motors and servos
#define CLAWOC 0//claw open/close port
#define CLAWL 1//claw lift port
#define ARM 3// arm port

#define CLAW_CLOSE 635//670
#define CLAW_OPEN 1550
#define CLAW_UP 1100
#define CLAW_DOWN 525
#define CLAW_BOTGUY 750
#define ARM_UP 600
#define ARM_MID 1040
#define ARM_DOWN 1230
//#define ARM_BOTGUY 

//UTILITY
float sign(float input)//returns 1 for positive, 0 for 0, -1 for negative
{
	if(input>0)
		return 1.;
	if(input<0)
		return -1.;
	return 0.;//implied else
}
float my_abs(float input)//because apparently kipr broke abs...
{
	if(input<0)
		return -input;
	return input;
}

//LIGHT START
void light_start(int sensor)
{
	WAIT(!(b_button() || c_button()));
	extra_buttons_hide();
	set_a_button_text("-");
	set_a_button_text("arm");
	set_a_button_text("skip");
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
	display_printf(0,4,"Prepared to begin: c to skip");
	while(light_s() > avg && !(c_button()))
	{
		msleep(50);
	}
}

//TIMING
long _start_time=0;//just in case someone forgets start-->will still work ish
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

//SERVOS
void servo_set(int port,int end,float time)
{//position is from 0-2047
	float increment = .01;
	float curr,start = get_servo_position(port);
	float i = ((end-start)/time)*increment;
	curr = start;
	if (start > end)
	{
		while(curr > end)
		{
			set_servo_position(port,curr);
			curr+=i;
			msleep((long)(increment*1000));
		}
	}
	else if (start < end)
	{
		while(curr < end)
		{
			set_servo_position(port,curr);
			curr+=i;
			msleep((long)(increment*1000));
		}
	}
	set_servo_position(port,end);
}

//CAMERA
int cam_area(int channel){//returns largest blob in channel, or 0 if none
	if (get_object_count(channel) > 0){
		return get_object_area(channel,0);
	}
	return 0;
}
void update_wait(){//updates the camera, and waits until success
	while(!camera_update()) msleep(1);
}

int getabbutton(){//returns 0,1 on a,b
	WAIT(!(a_button() || b_button()));
	WAIT(a_button() || b_button());
	if (a_button()) return 0;
	if (b_button()) return 1;
	printf("ERROR!");beep();
	msleep(2000);beep();
	return -1;//if something broke
}
int getabcbutton(){//returns 0,1,2 on a,b,c
	WAIT(!(a_button() || b_button() || c_button()));
	WAIT(a_button() || b_button() || c_button());
	if (a_button()) return 0;
	if (b_button()) return 1;
	if (c_button()) return 2;
	printf("ERROR!");beep();
	msleep(2000);beep();
	return -1;//if something broke
}

void reset_buttons()
{
	extra_buttons_hide();
	set_a_button_text("A");
	set_b_button_text("B");
	set_c_button_text("C");
	msleep(100);
}

int wait_time;
void wait_shutdown()
{
	msleep(wait_time);
	printf("ended based off time\n");
	_exit(0);//this throws a warning, but hey, it still compiles, so whatevs
}
void shutdownin(float time)//cause raisins.
{
	wait_time=(int)(time*1000);
	thread tw=thread_create(wait_shutdown);
	thread_start(tw);
}

void panic(){
	int i;
	for(i;i<4;i++){
		beep();
	}
}
