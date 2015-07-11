// Created on Fri July 10 2015

#define ROLLER 3
#define ROLLER_OUT_SPEED 100
#define ROLLER_IN_SPEED -100

#define SHOOTER 1
#define SHOOTER_FIRE_SPEED -100
#define SHOOTER_LOAD_SPEED 100



int main()
{
	motor(ROLLER, ROLLER_OUT_SPEED);
	msleep(500);
	off(ROLLER);
	msleep(1500);
	//shoot();
	
}
