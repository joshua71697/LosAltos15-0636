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
 * hook thingy between the two blue pins
*/


int main()
{
	//timeBlockUp(7000);
	
	if (menu() == TESTING)
		return 0;
		
	create_connect();
	
	backward_time(1000,SS); //square up
	create_forward(74,FS);
	create_block();
	
	forward_time(2000,SS);
	backward_time(1000,SS);
	
	
	create_block();
	
	forward_time(1500,SS);
	
	create_backward(5,SS);
	create_block();
	msleep(500);
	
	blockArmUp();
	
	create_backward(50,NS);
	create_right(90,1,TS);
	
	create_block();
	backward_time(4000,SS);
	
	create_forward(15,NS);
	create_right(140,1,TS);
	create_backward(10,NS);
	
	timeBlockUp(7000);
	shake(4);
	
	///// finished scoring blocks, onto phase 2 /////
	
	timeBlockDown(3000);
	create_backward(15, NS);
	create_left(45,1,TS);
	create_backward(8, NS);
	create_block();
	backward_time(3000,SS);
	
	
	
	
	

	create_block();
	create_disconnect();
}

