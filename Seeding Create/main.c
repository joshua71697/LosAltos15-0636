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
	//pingArmUp();
	//timePingUp(7000);
	
	if (menu() == TESTING)
		return 0;
	
	init();
	/*
	create_forward(62,FS);
	create_right(5,100,NS); 
	create_block();
	
	forward_time(2000,NS);
	backward_time(800,SS);
	forward_time(1000,SS);
	
	create_backward(5,SS);
	create_block();
	msleep(500);
	
	blockArmUp();
	create_backward(50,NS);
	create_right(90,1,TS);
	
	create_block();
	backward_time(1000,NS);
	
	create_forward(15,NS);
	create_right(135,1,TS);
	create_backward(7,NS);
	//create_left(10,1,TS); //extra turn
	timeBlockUp(6500);
	shake(4);
	
	///// finished scoring blocks, onto phase 2 /////
	
	timeBlockDown(3000);
	create_right(30,1,TS);
	create_block();
	backward_time(2000,SS);
	create_forward(20,NS);
	create_right(90,1,TS);
	create_backward(24, NS);
	create_block();
	*/
	pingArmUp();
	
	///// square up routine in the corner
	backward_time(3500,SS); //squared up with base wall.
	create_forward(2,NS);
	create_right(90,1,TS);
	create_block();
	backward_time(2800,SS); //squared up with side wall.
	create_forward(8,NS);
	create_left(90,1,TS);
	create_block();
	backward_time(3000,SS); //squared up with base wall.
	create_forward(2,NS);
	create_right(10,1,TS);
	create_block();
	pingArmDown();

	create_left(10,1,TS);
	create_block();
	
	
	///// start scoring the ping pong balls
	scorePing(); // ### SCORE ONE ###
	scorePing(); // ### SCORE TWO ###
	upDown();    // ### SCORE THREE ### 
	create_left(5,1,TS);
	create_forward(30,NS); // dist between is 36
	create_block();
	scorePing(); // ### SCORE FOUR ###
	upDown(); // ### SCORE FIVE ###
	create_left(15,100,TS);
	create_block();
	pingArmUp();    // ### SCORE SIX ###
	
	///// get back to the caldera to score
	create_left(165,1,TS);
	create_block();
	backward_time(1000,SS); //squared up with far wall.
	motor(PING_ARM, PING_ARM_UP_SPEED); //tighten this a bit more.
	msleep(1000);
	ao();
	create_left(30,400,SS);
	create_forward(25,NS); 
	create_left(30,1,TS);
	create_block();
	forward_time(2000,NS); // square up with the side wall.
	
	///// deploy, end, and be happy :)
	deploy();
	create_backward(10,NS);
	
	create_block();
	create_disconnect();
}

