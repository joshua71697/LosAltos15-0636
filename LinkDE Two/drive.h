#define gmpc(port) get_motor_position_counter(port)


//primary driving code
#define MOT_LEFT 0
#define MOT_RIGHT 3
#define PI 3.14159265358979

/**THESE ARE IMPORTANT**/
#define SPD 60//turning
#define SPDl 89.//right forward (arc values vary based on link power)
#define SPDr 100.//left forward

#define rdistmult 1.0
#define SPDlb 80.//right backward
#define SPDrb 90.//left backward
#define rdistmultb (SPDrb/SPDlb)
#define wheeldiameter 6
#define ks 15
#define CMtoBEMF (850/(PI*wheeldiameter))

void drive_off();
void clear_all_drive();
void drive(int mL,int mR);


void right(float degrees, float radius);
void left(float degrees, float radius);
void forward(float distance);
void multforward(float distance, float speedmult);
void backward(float distance);
