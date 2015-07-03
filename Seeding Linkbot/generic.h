#define boolean int//this is the most important part of the code...
#define true 1
#define false 0

//general use fake functions
#define WAIT(thing); while(!(thing)){msleep(1);}
#define LIMIT(thing,time); {long _tmptime = systime()+time; while(!(thing) && (_tmptime > systime())){msleep(1);}}//time in ms
#define gmpc get_motor_position_counter//this just makes things easier...
#define cmpc clear_motor_position_counter
#define light_s() analog10(sensor)//this is used in lightstart

//motors and servos
#define BLOCK_CLAW 3
#define BC_OPEN 150
#define BC_CLOSE 1100
#define BC_START 1400
#define BASKET_ARM 1
#define BA_START 1100
#define BA_DOWN 1600
#define BA_UP 0
#define BA_JERK 500//down enough to jiggle the basket
#define BA_MID 600//high enough to let the block arm in
#define TRIBBLE_CLAW 2
#define TC_OPEN 1500
#define TC_PART_OPEN 700//only part way open
#define TC_CLOSE 250
#define TRIBBLE_ARM 0
#define TA_DUMP 200
#define TA_UP 500//just enough to stop the blocks from falling everywhere
#define TA_DOWN 1500
#define TA_JUMP 1000//position to get over the pipe (slightly raised)
#define TA_START 1950//pushes against the ground to shift the robot forward
#define BLOCK_ARM 1//motor
#define BLA_DOWN 0
#define BLA_START -330//all the way up-->under the basket
#define BLA_UP -250//dumping position
#define BLA_LIFT -50//lifted up right after it got the blocks
#define BLA_MID -175//out of the way of the basket
#define LIGHT_SENSOR 0//sensor port
#define LTOUCH digital(14)//
#define RTOUCH digital(15)//

void move_block_arm(int target);
void servo_set(int port,int end,float time);

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
void change_b_button(int digit)//makes the b button the right digit-->only really useful for input_float and input_int
{//asdkjf'kfskfhfaksjdghsdkfljafhaelikfdsncxz7asdlkjkhgre
	if(digit==0)
		set_b_button_text("0");
	else if(digit==1)
		set_b_button_text("1");
	else if(digit==2)
		set_b_button_text("2");
	else if(digit==3)
		set_b_button_text("3");
	else if(digit==4)
		set_b_button_text("4");
	else if(digit==5)
		set_b_button_text("5");
	else if(digit==6)
		set_b_button_text("6");
	else if(digit==7)
		set_b_button_text("7");
	else if(digit==8)
		set_b_button_text("8");
	else//9
		set_b_button_text("9");
}
float input_float(int x, int y)//x and y are where the display_printf starts
{
	int digits=0;//how many digits have been inputted
	int curr_digit=0;//what number is selected right now
	float curr_number=0;//current output
	int digits_after_dec=-1;//number of digits after the decimal point-->-1 means no decimal point yet
	extra_buttons_show();
	set_a_button_text("-");//move curr_digit down by one
	set_c_button_text("+");//up
	set_x_button_text("backspace");//delete the last digit
	set_y_button_text(".");//decimal point
	set_z_button_text("enter");//done with input
	while(1)
	{
		change_b_button(curr_digit);//I have to do this the hard way because there's no good way to convert between ints and strings...
		WAIT(a_button()||b_button()||c_button()||x_button()||y_button()||z_button());
		if(a_button())
			curr_digit--;//I'll sanitize the result at the end of the loop
		else if(b_button())
		{
			if(digits_after_dec<0)//no dec point yet
			{
				curr_number*=10;
				curr_number+=(float)(curr_digit);
			}
			else//has a dec point
			{
				curr_number+=((float)(curr_digit))/exp10(digits_after_dec+1);//trust me, it works
				digits_after_dec++;//has an extra digit after the decimal point
			}
			display_printf(x+digits, y, "%d", curr_digit);
			digits++;
		}
		else if(c_button())
			curr_digit++;
		else if(x_button())
		{
			if(digits>0)//has a digit to delete-->otherwise, will ignore the button tap
			{
				if(digits_after_dec>0)//deleting a digit after the dec point
				{
					digits_after_dec--;
					curr_number=((float)((int)(curr_number*exp10(digits_after_dec))))/exp10(digits_after_dec);//don't question it...
				}
				else if(digits_after_dec==0)//deleting the decimal point
				{
					digits_after_dec=-1;//no more dec point
				}
				else//deleting a digit with no dec point
				{
					curr_number=(float)((int)(curr_number/10));//again, it works, just go with it
				}
				digits--;
				display_printf(x+digits, y, " ");//clear the input it deleted
			}
		}
		else if(y_button())
		{
			if(digits_after_dec<0)//doesn't have a decimal point yet-->can add one
			{//if not, will ignore button tap
				if(digits==0)//no digits yet-->add a leading 0
				{
					display_printf(x,y,"0");
					digits=1;
				}
				display_printf(x+digits, y, ".");
				digits_after_dec=0;//has a dec, but no numbers after it yet
				digits++;//counts as a digit for the purposes of printing in the right locations
			}
		}
		else//z
		{
			if(digits>0)//has put in a number-->if not, can't exit, so will ignore button tap
				break;//exit the loop-->get to the return
		}
		curr_digit+=10;//will make it positive, if was already positive, will deal with that in the next step
		curr_digit=curr_digit%10;//this will get in in the range of 0-9
		WAIT(!(a_button()||b_button()||c_button()||x_button()||y_button()||z_button()));
		msleep(50);
	}
	return curr_number;
}
int input_int(int x, int y)//pretty much the same as input_float...
{
	int digits=0;//how many digits have been inputted
	int curr_digit=0;//what number is selected right now
	int curr_number=0;//current output
	extra_buttons_show();
	set_a_button_text("-");//move curr_digit down by one
	set_c_button_text("+");//up
	set_x_button_text("backspace");//delete the last digit
	set_y_button_text("");//nothing-->can't have a decimal point
	set_z_button_text("enter");//done with input
	while(1)
	{
		change_b_button(curr_digit);//I have to do this the hard way because there's no good way to convert between ints and strings...
		WAIT(a_button()||b_button()||c_button()||x_button()||z_button());
		if(a_button())
			curr_digit--;//I'll sanitize the result at the end of the loop
		else if(b_button())
		{
			curr_number*=10;
			curr_number+=(float)(curr_digit);
			display_printf(x+digits, y, "%d", curr_digit);
			digits++;
		}
		else if(c_button())
			curr_digit++;
		else if(x_button())
		{
			if(digits>0)//has a digit to delete-->otherwise, will ignore the button tap
			{
				curr_number=curr_number/10;//casts to an int automatically, cause both are ints
				digits--;
				display_printf(x+digits, y, " ");//clear the input it deleted
			}
		}
		else//z
		{
			if(digits>0)//has put in a number-->if not, can't exit, so will ignore button tap
				break;//exit the loop-->get to the return
		}
		curr_digit+=10;//makes it positive; if was already positive, will be undone in the next step
		curr_digit=curr_digit%10;//this will get in in the range of 0-9
		WAIT(!(a_button()||b_button()||c_button()||x_button()||z_button()));
		msleep(50);
	}
	return curr_number;
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

void brick()//the title says it all...
{
	while(1)
	{
		printf("%d", rand());
		//uncomment the next line only if you're a wimp...or don't actually want to brick the link XD
		//msleep(1);
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
void set_up_drive()//puts all the servos in the right starting places for if it's driving
{
	printf("basket and both arms need to be down\n");
	printf("press black button when ready\n");
	WAIT(side_button());
	set_servo_position(BASKET_ARM, BA_DOWN);
	set_servo_position(BLOCK_CLAW, BC_START);
	set_servo_position(TRIBBLE_CLAW, TC_CLOSE);
	cmpc(BLOCK_ARM);//down is position 0
	enable_servos();
	msleep(1500);//give the servos time to move (if necessary)
	set_servo_position(TRIBBLE_ARM, TA_UP);
	move_block_arm(BLA_UP);
}
void set_up_jump()//puts all the servos in the right places to fit in the box if jumping
{
	printf("basket up, both arms down.\n");
	printf("press black button when ready.\n");
	WAIT(side_button());
	set_servo_position(BLOCK_CLAW, BC_START);//initial positions
	set_servo_position(BASKET_ARM, BA_MID);//
	set_servo_position(TRIBBLE_ARM, TA_START);//
	set_servo_position(TRIBBLE_CLAW, TC_CLOSE);//
	cmpc(BLOCK_ARM);//starts at 0-->down
	enable_servos();
	msleep(1000);//let them get into position
	move_block_arm(BLA_START);//put the block arm up
	msleep(500);
	servo_set(BASKET_ARM, BA_START, .5);//put the basket into the right place
}
void ready_to_jump()//after start of round, moves out of box to get ready to jump
{
	servo_set(BASKET_ARM, BA_UP, 1);
	move_block_arm(BLA_DOWN);
	msleep(1500);//let teh other robot get out of teh way
	servo_set(BASKET_ARM, BA_DOWN, 1);
	move_block_arm(BLA_UP);
	servo_set(TRIBBLE_ARM, TA_JUMP, .3);
}
void tribble_claw_dump()//from claw down and closed, dumps in the basket
{
	if(get_servo_position(TRIBBLE_CLAW)!=TC_CLOSE)//claw isn't closed
		servo_set(TRIBBLE_CLAW, TC_CLOSE, .4);//so close it
	servo_set(TRIBBLE_ARM, TA_DUMP, 1.2);//lift the arm (slow enough that loose tribbles don't go flying
	msleep(500);
	servo_set(TRIBBLE_CLAW, TC_OPEN, .4);//open to dump the tribbles
	msleep(500);
	servo_set(TRIBBLE_CLAW, TC_CLOSE, .4);//close and reopen to try to get them all in
	servo_set(TRIBBLE_CLAW, TC_OPEN, .4);//(this is optional and may need to be taken out later)
	msleep(500);
	servo_set(TRIBBLE_CLAW, TC_PART_OPEN, .2);//get the arm out of the way
	servo_set(TRIBBLE_ARM, TA_DOWN, .4);//
	servo_set(TRIBBLE_CLAW, TC_OPEN, .2);//
}
void jiggle_basket()//jiggles the basket while dumping-->help stuff fall out
{
	msleep(1000);//let stuff fall out
	servo_set(BASKET_ARM, BA_JERK, .1);//jerk to loosen stuff
	servo_set(BASKET_ARM, BA_UP, .1);//
	msleep(1000);//more time to let stuff fall out
}
void move_block_arm(int target)
{
	int dir=sign(target-gmpc(BLOCK_ARM));//direction it has to move in (+1 or -1)
	if(dir==0)//is already at target
		return;//-->exit
	if(dir==sign(BLA_UP))//is moving up-->needs more power
		motor(BLOCK_ARM, 60*dir);
	else//moving down-->doesn't need that much power
		motor(BLOCK_ARM, 35*dir);
	LIMIT((target-gmpc(BLOCK_ARM))*dir<=0, 1500);//wait until it reaches the position (timeout after 1.5 seconds)
	off(BLOCK_ARM);
}
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
