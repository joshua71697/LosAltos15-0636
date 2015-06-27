/********************************************************************************
 * Botball LACT Template							*
 * Include this in each of your programs as a default library. 			*
 * 										*
 * Sample Usage: 								*
 *										*
 *	Place the following files inside the Link folder			*
 *		: menu.h							*
 *		: drive.h							*
 *										*
 *	Place the following files inside the Create folder			*
 *		: createDrive.h							*
 *		: finalscripting.c						*
 *										*
 *	Place the version of this file (either with the create or link mode	*
 *	defined) in the same directory as your create or link folder.		*
 * 										*
 *	Then, include the following line at the beginning of your program:	*
 *		#include "./template.h"						*
 *										*
 * @author Manan								*
 * Version 421014								*
 ********************************************************************************/

//common light sensor port
#define LS 0
#define ls() light_start(LS)

#define WAIT(x); while(!(x)){msleep(10);}
#define LIMIT(thing,time); {double _tmptime = seconds()+time; while(!(thing) && (_tmptime > seconds())){msleep(1);}}

#define gmpc(port) get_motor_position_counter(port)

#define DEGTORAD 0.017453292519943295769236907684886
#define RADTODEG 57.295779513082320876798154814105
#define pi 3.1415926535897932384626433832795

//Generic Utility
float bound(float num, float max)
{
	if (num > max) return max;
	else if (num < -max) return -max;
	else return num;
}
//1 if true, 0 if false
int in_range(int input, int wanted, int fudge)
{
	if (wanted+fudge >= input && wanted-fudge <= input) return 1;
	else return 0;
}

//actual distance away from object with ET sensor
float ET_distance(int port){
    return ((sqrt(100.0/analog10(port)))-2.2);
	//	return sqrt(3674186.332167/(X-9.384503)+26294.871673)-176.997129;
}

int et_avg(int port)
{
	int totalcount = 100;
	int run = 0;
	int i;
	for(i=0;i<totalcount;i++)
	{
		run += analog_et(port);
		msleep(2);
	}
	return run/totalcount;
}

//Light Start
#define light_s() analog10(sensor)
void light_start(int sensor)
{
	int max = 0,min = 9999,curr = 0,avg = 0;
	display_clear();
	display_printf(0,0,"Max:");
	display_printf(0,1,"Min:");
	display_printf(0,2,"Curr:");
	display_printf(0,3,"avg:");
	while(!b_button() && !c_button())	//Interaction Button
	{
		curr = light_s();
		if (curr > max) max = curr;
		if (curr < min) min = curr;
		
		avg = (max+min)/2;
		display_printf(5,0,"%d   ",max);
		display_printf(5,1,"%d   ",min);
		display_printf(6,2,"%d   ",curr);
		display_printf(5,3,"%d   ",avg);
		
		if (curr > avg) display_printf(10,5,"XX");
		else display_printf(10,5,"OO");
		msleep(50);
	}
	display_clear();
	display_printf(0,4,"Prepared to begin: c to skip");
	while(light_s() > avg && !(c_button())) msleep(50);
}

//Timing Utility 
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
    msleep(((long)(t*1000))-curr_time());
}

//Servo Utility
void servo_set(int port, int end, float time)
{
	//position is from 0-2047
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

//Camera Utility
int cam_area(int channel){
	//returns largest blob in channel, or 0 if none
	if (get_object_count(channel) > 0) return get_object_area(channel,0);
	return 0;
}
void update_wait(){
	//updates the camera, and waits until success
	while(!camera_update()) msleep(1);
}

//deprecated, use update_wait()
void cam_block() { update_wait(); }

//ASUS Xtion Depth Sensor Utility

//0 if not interactive, 1 if interactive
//only need meaningful x and y values if not interactive
int depth_distance(int interactive, int x, int y) 
{
	depth_open();
	if(interactive==1)
	{
		int r,g,b,row,c,val;
		graphics_open(320,240); // open up graphics window (full screen on Link)
		while(!get_mouse_left_button()) 
		{ 
			depth_update(); 
			for(row=0; row<240; row++) 
			{ 
				for(c=0; c<320; c++) 
				{ 
					val = get_depth_value(row,c) ; // get distance for pixel in mm
					// if its not a legal value or its too far (1.5m for this program), color it black
					if(val > 1536 || val==0) graphics_pixel(c,row,0,0,0);
					else 
					{ 
					       val=val - 512; 
					       r=val > 512 ? 0 : 255-val/2;  		// red if close
					       g=val > 512 ? 255-(val-512)/2 : val/2; 	// green if mid value
					       b=val > 512 ? val/2-255 : 0;  		// blue if far (~1.5m)
					       graphics_pixel(c,row, r,g,b); 		// draw the pixel
					}
				}
			}
			graphics_update(); // paint the screen with all of the pixels
		}
		get_mouse_position(&c,&row); 		
		depth_close(); graphics_close();  
		printf("Distance to pixel %i,%i is %imm\n\n\n",c,row,get_depth_value(row,c));
	}
	else return get_depth_value(x,y);
}
  
//points a servo centered on the camera's fov towards an object that fits the color defined at camchannel
void set_servo_color(int servo, int camchannel) 
{
	int offset, x, y;
	enable_servo(servo);	// enable servo
	camera_open();	
	camera_update();    
	while (get_object_count < 0) { camera_update(); msleep(1); }
	
	//largest blob - we now have an object in sight
	point2 p = get_object_center(camchannel,0);
	x = p.x;
	y = p.y;
	
	display_printf(0,1,"Center of largest blob: (%d,%d)   ",x,y);
	offset=5*(x-80); // amount to deviate servo from center
	set_servo_position(0,1024+offset);
}
