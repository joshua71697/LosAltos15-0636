#include "createDrive.h"
#include "generic.h"

/* setup:
 * robot one cube away from the side
 * flat side of the create squared up with the back.
 * ping arm is down.
 * block arm is down - facing forward
 * block attachement on - with the two starting cubes on the robot's *right*
 * block attachment with the axels through the beam's end holes
 * otherside cubes start together, one half cube distnace away from the edge
*/

int main()
{
	init();
	float time = curr_time();
	blockClawClose();
	
	// timeBlockUp(800);
	
	///// go to far blocks
	create_drive_direct_dist(NS*.95, NS, 20*MM_TO_INCH);
	create_block();	
	pingArmDown();
	
	create_drive_direct_dist(NS, NS*.96, 36*MM_TO_INCH);
	create_left(15,1,TS);
	create_block();
	
	///// pick up far cubes
	// timeBlockDown(700);
	blockClawOpen();
	create_forward(11,SS);
	create_block();	
	blockClawClose();
	timeBlockUp(1800);
	
	///// go to caldera
	create_right(15,1,TS);
	create_backward(43,NS);
	create_right(85,1,TS);
	create_block();
	backward_time(1000,SS); // square up with side wall
	create_forward(15,SS);
	create_right(130,1,TS);
	create_backward(12,SS);
	create_block();
	
	///// score blocks
	blockArmUp();
	create_backward(5,SS);
	create_block();
	blockClawOpen();
	create_forward(4,NS);
	create_block();
	blockClawClose(); // this knocks in the last one
	timeBlockDown(3500);
	blockClawOpen();
	
	printf("\n\nBLOCKS %d \n",curr_time());
	
	///// go to ping pongs 
	
	//motor(PING_ARM, PING_ARM_UP_SPEED); //slowly help it in the background
	//msleep(200);
	//off(PING_ARM);
	create_left(20,1500,NS);
	//create_forward(20,NS);
	
	//motor(PING_ARM, PING_ARM_UP_SPEED/2); //slowly help it in the background
	//create_forward(8,NS);
	create_left(118,1,TS);
	create_backward(9,NS);
	create_block();
	backward_time(3000,SS/3);
	pingArmUp();
	motor(PING_ARM, PING_ARM_UP_SPEED); //slowly help it in the background
	
	create_forward(8,NS); // this number determines how far away we will be.
	create_left(90,1,TS);
	create_backward(12,NS);
	create_block();

	/////////////////////////////////////////////////////////////////////
	///////////////////////// SECOND HALF ///////////////////////////////
	/////////////////////////////////////////////////////////////////////
	
	backward_time(2000,SS/3); //squared up with base wall.
	create_forward(1.5,NS);
	create_right(10,1,TS);
	create_block();
	pingArmDown();
	backward_time(3000,SS/3); //squared up with base wall.
	
	create_left(9,1,TS);
	create_block();
	
	blockArmDown(); // make sure.
	
	
	///// start scoring the ping pong balls
	scorePing(); // ### SCORE ONE ###
	upDown(); // ### SCORE TWO ###
	create_drive_direct_dist(.96*SS, SS, 7.5*MM_TO_INCH);
	create_block();
	pingArmUp();    // ### SCORE THREE ### 
	pingArmDown();    
	create_block();
	create_forward(36,NS); // dist between is 36
	create_block();
	scorePing(); // ### SCORE FOUR ###
	upDown(); // ### SCORE FIVE ###
	// create_left(15,500,TS);
	create_block();
	// pingArmUp();    // ### SCORE SIX ###
	
	
	///// get back to the caldera to score
	create_backward(50,NS); 
	create_block();
	create_right(90,1,SS);
	create_block();
	backward_time(2500,SS);
	pingArmUp();
	motor(PING_ARM, PING_ARM_UP_SPEED); //tighten this a bit more.
	create_forward(20,NS); 
	create_left(70,1,TS);
	create_forward(15,NS);
	
	create_block();
	///// deploy, end, and be happy :)
	
	deploy();
	create_drive_direct(NS, NS);
	msleep(1000);
	create_drive_direct(-TS, TS);
	msleep(234234234);
	
	create_block();
	create_disconnect();
	ao();
	now();
}


