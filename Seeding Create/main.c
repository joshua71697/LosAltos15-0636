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
	create_drive_direct_dist(NS*.95, NS, 21*MM_TO_INCH);
	create_block();
	
	
	
	//dropOffBlocks();
	//create_forward(5,NS);
	//create_right(30,1,TS);
	//create_block();
	//backward_time(2000,SS);
	
	
	
	
	//create_right(85,TS);
	//backward_time(2000,SS);
	
	
	
	create_drive_direct_dist(NS, NS*.98, 38*MM_TO_INCH);
	create_block();
	
	
	timeBlockDown(1500);
	blockClawOpen();
	create_forward(5,SS);
	create_block();	
	blockClawClose();
	
	timeBlockUp(1800);
	
	create_backward(41,NS);
dropOffBlocks();
	create_forward(10,NS);
	
	create_block();
	
	
}

