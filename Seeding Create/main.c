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
	blockClawClose();
	// timeBlockUp(800);
	
	///// go to far blocks
	create_drive_direct_dist(NS*.95, NS, 21*MM_TO_INCH);
	create_block();	
	create_drive_direct_dist(NS, NS*.96, 36*MM_TO_INCH);
	create_left(15,1,TS);
	create_block();
	
	///// pick up far cubes
	// timeBlockDown(700);
	blockClawOpen();
	create_forward(10,SS);
	create_block();	
	blockClawClose();
	timeBlockUp(1800);
	
	///// go to caldera
	create_right(15,1,TS);
	create_backward(43,NS);
	create_right(85,1,TS);
	create_block();
	backward_time(2000,SS); // square up with side wall
	create_forward(15,NS);
	create_right(130,1,TS);
	create_backward(11,NS);
	create_block();
	
	///// score blocks
	blockArmUp();
	create_backward(3,NS);
	create_block();
	blockClawOpen();
	create_forward(3,NS);
	create_block();
	blockClawClose(); // this knocks in the last one
	timeBlockDown(3500);
	blockClawOpen();
	
	printf("\n\nBLOCKS %d \n",curr_time());
	
	///// go to ping pongs 
	create_forward(28,NS);
	create_left(140,1,TS);
	create_block();
	backward_time(2000,SS);
	pingArmUp();
	motor(PING_ARM, PING_ARM_UP_SPEED / 2); //slowly help it in the background
	create_forward(8,NS); // this number determines how far away we will be.
	create_left(90,1,TS);
	create_block();

	/////////////////////////////////////////////////////////////////////
	///////////////////////// SECOND HALF ///////////////////////////////
	/////////////////////////////////////////////////////////////////////
	
	///// square up 
	backward_time(2000,SS); //squared up with base wall.
	create_forward(1,NS);
	create_right(10,1,TS);
	create_block();
	pingArmDown();
	
	create_left(8,1,TS);
	create_block();
	
	blockArmDown(); // make sure.
	
	
	///// start scoring the ping pong balls
	scorePing(); // ### SCORE ONE ###
	scorePing(); // ### SCORE TWO ###
	upDown();    // ### SCORE THREE ### 
	create_forward(31,NS); // dist between is 36
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
	backward_time(1500,SS); //squared up with base wall.
	pingArmUp();
	motor(PING_ARM, PING_ARM_UP_SPEED); //tighten this a bit more.
	create_forward(20,NS); 
	create_left(70,1,TS);
	create_forward(10,NS);
	
	create_block();
	///// deploy, end, and be happy :)
	
	deploy();
	
	create_block();
	create_disconnect();
	ao();
	printf("\n\ntook %d",curr_time());
}


