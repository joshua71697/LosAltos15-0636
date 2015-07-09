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
	create_drive_direct_dist(NS*.95, NS, 19*MM_TO_INCH);
	create_block();	
	pingArmDown();
	
	create_drive_direct_dist(NS, NS*.96, 32*MM_TO_INCH);
	create_left(16,1,TS);
	create_block();
	
	///// pick up far cubes
	// timeBlockDown(700);
	blockClawOpen();
	create_forward(9.5,SS);
	create_block();	
	blockClawClose();
	timeBlockUp(1600); //1800 for a long time
	
	///// go to caldera
	create_right(14,1,TS); // was 15 for a long time...
	create_backward(38,NS);
	create_right(85,1,TS);
	create_block();
	backward_time(5000,SS); // square up with side wall
	create_forward(15,SS);
	create_right(131,1,TS);
	create_backward(11,SS);
	create_block();
	
	///// score blocks
	blockArmUp();
	create_backward(4,SS);
	create_block();
	blockClawOpen();
	create_forward(3,NS);//#yoloswag
	create_block();
	// blockClawClose(); // this knocks in the last one
	servo_set(BLOCK_CLAW, BLOCK_CLAW_CLOSED, 2);
	create_backward(2,NS);//#yoloswag
	create_block();
	servo_set(BLOCK_CLAW, BLOCK_CLAW_MID, 1);
	msleep(1000);
	create_forward(2,NS);//#yoloswag
	create_backward(2,NS);//#yoloswag
	create_block();
	motor(BLOCK_ARM, BLOCK_ARM_DOWN_SPEED/3);
	msleep(500);
	
	printf("\n\nBLOCKS %d \n",curr_time());
	
	///// go to ping pongs 
	create_left(19,1500,NS);
	create_left(118,1,TS);
	create_backward(10,NS);
	create_block();
	backward_time(4000,SS);
	blockClawOpen();
	pingArmUp();
	off(BLOCK_ARM);
	motor(PING_ARM, PING_ARM_UP_SPEED); //slowly help it in the background
	
	create_forward(8,NS); // this number determines how far away we will be.
	create_left(90,1,TS);
	create_backward(13,NS);
	create_block();
	
	/////////////////////////////////////////////////////////////////////
	///////////////////////// SECOND HALF ///////////////////////////////
	/////////////////////////////////////////////////////////////////////
	
	backward_time(4000,SS); //squared up with base wall.
	create_forward(1.5,NS);
	create_right(10,1,TS);
	create_block();
	pingArmDown();
	motor(PING_ARM, PING_ARM_UP_SPEED); //go up part way
	msleep(200);
	ao();
	create_left(8,1,TS); 
	create_block();	

	///// start scoring the ping pong balls
	msleep(1000);
	motor(PING_ARM, PING_ARM_UP_SPEED); // ### SCORE ONE ###
	msleep(1800); // go up the rest of the way
	create_forward(4,SS);
	create_block();
	msleep(500);
	pingArmDown();
	create_forward(7.6,SS); //used to be 7.5
	create_block();
	msleep(500);
	
	upDown(); // ### SCORE TWO ###
	create_right(3,1,TS);
	create_forward(8,SS); //used to be 7.5
	create_left(4,1,TS); 
	create_block();
	msleep(1000);
	pingArmUp(); // ### SCORE THREE ###
	pingArmDown();
	create_left(5,1,TS);
	create_forward(17,NS); // dist between is 36
	create_right(6,1,TS);
	create_forward(19,NS);
	create_block();
	msleep(500);
	scorePing(); // ### SCORE FOUR ###
	upDown(); // ### SCORE FIVE ###
	// create_left(15,500,TS);
	create_block();
	// pingArmUp();    // ### SCORE SIX ###
	
	
	///// get back to the caldera to score
	create_backward(55,NS); 
	create_block();
	create_right(90,1,SS);
	create_backward(6,NS);
	backward_time(5000,SS); //squared up with base wall.
	create_block();
	
	motor(PING_ARM, PING_ARM_UP_SPEED); //tighten this
	msleep(1000);
	create_forward(20,FS); 
	create_left(75,1,TS);
	create_forward(17,NS);//NOOOOB
	
	create_block();
	///// deploy, end, and be happy :)
	msleep(2000);
	
	deploy();
	printf("\n\n\n===\n\n");
	now();
		
	
	///// extras!!!! /////
	/*thread armDown = thread_create(pingArmDown);
	thread_start(armDown);
	simple_backward(12, FS);
	simple_right(10, TS);
	thread_destroy(armDown);
	pingArmUp();
	
	thread armDown2 = thread_create(pingArmDown);
	thread_start(armDown2);
	
	simple_left(20, TS);
	thread_destroy(armDown2);
	
	pingArmUp();
	
	msleep(300);
	create_stop();
	
	
	pingArmUp();
	*/
	create_block();
	create_disconnect();
//	thread_destroy(armDown);
	
	ao();
	
}


