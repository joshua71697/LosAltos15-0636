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
	timeBlockUp(1800);
	create_forward(15,NS);
	create_block();
	create_drive_direct_dist(NS*.98, NS, 41*MM_TO_INCH);
	create_block();
	
	
	timeBlockDown(1500);
	blockClawOpen();
	create_forward(10,SS);
	create_block();	
	blockClawClose();
	
	timeBlockUp(1800);
	
	
	create_backward(40,NS);
	create_right(90,1,TS);
	create_block();
	backward_time(3000,SS);
	
	
	
	
	create_block();
	
}

