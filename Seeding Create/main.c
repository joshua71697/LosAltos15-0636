#include "createDrive.h"
#include "generic.h"

/* setup:
 * robot one cube away from the side
 * flat side of the create squared up with the back.
 * ping arm is down.
 * block arm is down - facing forward
 * block attachement on - with the two starting cubes on the robot's *right*
 * block attachment with the axels through the beam's end holes
 * otherside cubes start together, one cube distnace away from the edge
*/

int main()
{
	
	init();
	blockClawClose();
	// timeBlockUp(800);
	
	///// go to far blocks
	create_drive_direct_dist(NS*.95, NS, 21*MM_TO_INCH);
	create_block();	
	create_drive_direct_dist(NS, NS*.96, 35*MM_TO_INCH);
	create_left(15,1,TS);
	create_block();
	
	///// pick up far cubes
	// timeBlockDown(700);
	blockClawOpen();
	create_forward(9,SS);
	create_block();	
	blockClawClose();
	timeBlockUp(1800);
	
	///// go to caldera
	create_backward(41,NS);
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
	
	///// go to ping pongs 
	create_forward(8,NS);
	create_right(40,1,TS);
	create_block();
	backward_time(2000,SS);
	create_forward(17,NS);
	create_block();
	create_right(80,1,TS);
	create_block();
	pingArmUp();
	motor(PING_ARM, PING_ARM_UP_SPEED / 2); //slowly help it in the background
	create_right(5,1,TS);
	create_backward(24, NS);
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
	
	create_left(10,1,TS);
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
	backward_time(3000,SS); //squared up with base wall.
	pingArmUp();
	motor(PING_ARM, PING_ARM_UP_SPEED); //tighten this a bit more.
	msleep(1000);
	ao();
	create_forward(20,NS); 
	create_left(100,1,TS);
	
	create_block();
	///// deploy, end, and be happy :)
	deploy();
	create_backward(10,NS);
	
	create_block();
	create_disconnect();
}


