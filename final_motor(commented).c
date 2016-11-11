//Alexander Benedict, Joseph Nelson
#include <math.h>
#include "io_softcore.h"
#include <util/delay.h>
#include "motor.h"
#define F_CPU 16000000UL
#define switches PINC
//used float for x and y because we use the math.h library to convert angle/distance to x and y later in the code
float x;
float y;
unsigned short angle;
char dist;
//Function used to input either xy or angle distance
void input()
	{
		while(1)
		{
			//flip up the furthest right switch to input 				xy or 2nd furthest right for angle distance 
			PORTD = 0x01;
			if(switches == 1)
			{
				//once x value is input flip up the 					leftmost switch  to save x value. The 					second switch from the left is  				considered a negative sign
				while(switches < 128)
				{
					PORTD = 0x02;
				}
				if(switches<192)
				{
					x = switches - 128;
				}
				if(switches>=192)
				{
					x = (switches - 192)*(-1);
				}
				//Once y value is put on switches flip 					the furthest left down to save value 
				while(switches>=128)
				{
					PORTD = x;
				}
				if(switches<64)
				{
					y=switches;
				}
				if(switches>=64)
				{
					y= (switches-64)*(-1);
				}
			break;
			}
			// angle distance 
			else if(switches == 2)
			{
				//input angle value in two batches 					which are added together then 					multiplied by two, toggling leftmost 					switch between inputs (no negative 					switch)
				while (switches <128)
				{
					PORTD = 0x04;
				}
				angle = switches - 128;
				while (switches >= 128)
				{
					PORTD = 0x05;
				}
				angle = angle + switches;
				angle = angle *2;
				//distance input
				while (switches < 128)
				{
					PORTD = 0x06;
				}
				dist = switches - 128;
			//calls the convert function if angle distance 				was input
			convert(angle, dist);
			break;
			}
		}
	}

//used for converting angle distance to xy
void convert(unsigned short angledeg, char dist)
{
    char i = 0;
    float PI = 3.14159;
    float angle;

    //angle must be converted from degrees to radians
    while(angledeg >= 90)
        {
        	i++;
        	angledeg = angledeg - 90;
        }
    angle = angledeg * PI / 180;
    if (i == 0)
        {
        	x = cos(angle) *dist;
        	y = sin(angle) *dist;
        }
    else if (i == 1)
        {
        	x = -sin(angle)*dist;
        	y = cos(angle)*dist;
        }
    else if (i == 2)
        {
        	x = -cos(angle)*dist;
        	y = -sin(angle)*dist;
        }
    else if (i == 3)
        {
        	x = sin(angle)*dist;
        	y = -cos(angle)*dist;
        }
}
//the move function determines which quadrant the xy location is in and utilizes the drive_dist and rotate functions to reach the location
void move(float x, float y)
{
	 //we found it beneficial to add delays between each function 		 called ("_delay_ms (2000)")     
	 if (x>=0 && y>=0)
            {
            drive_dist(x);
	    _delay_ms (2000);
            rotate();
	    _delay_ms (2000);
            drive_dist(y);
            }
        else if (x<0 && y>=0)
            {
            rotate();
	    _delay_ms (2000);
            drive_dist(y);
            _delay_ms (2000);
	    rotate();
	    _delay_ms (2000);
            drive_dist(-x);
            }
        else if (x<0 && y<0)
            {
            rotate();
	    _delay_ms (2000);
            rotate();
	    _delay_ms (2000);
            drive_dist(-x);
            _delay_ms (2000);
	    rotate();
	    _delay_ms (2000);
            drive_dist(-y);
            }
        else if (x>=0 && y<0)
            {
            rotate();
	    _delay_ms (2000);
            rotate();
	    _delay_ms (2000);
            rotate();
	    _delay_ms (2000);
            drive_dist(-y);
	    _delay_ms (2000);
            rotate();
            _delay_ms (2000);
	    drive_dist(x);
            }
}
//drive_dist takes the distance needed to drive in inches and drives to the location in a staight line
void drive_dist(char c) 
{
	int e = 0;
	unsigned char w = 0;
	unsigned char vala = 0;
	while (c>=0) 
	{
		e=0;
		//since our rover didn't drive straight and our left 			encoder didn't work properly we alternate between both 			motors going foward and the left motor going foward 			while the right brakes, this sequence is repeated 			about every inch it has to move
		while(e<=39)
		{
			PORTE = 0x44;
			w = PINE & 0x03;
			if (vala != w) 
			{
				e++;
				vala = w;
			}
		}
		PORTE = 0xC4;
		_delay_ms (120); 
		c--;
		e = 0;
	}
	PORTE = 0xCC;
}
//the rotate function makes the right motor go foward and the left in reverse until the car has turned 90 degrees to the left, the encoder tick value was determined by testing
void rotate() 
{
	unsigned char q;
	int i = 0;
	unsigned char vala;
	while(1)
	{
		PORTE = 0x48;
		q = PINE & 0x03;
		if (vala != q)
		{
			i++;
			vala = q;
		}
		if(i >= 490)
		{
			PORTE = 0xCC;
			break;
		}
	}
}

int main()
{
	DDRD = 0xFF;
	DDRE = 0xCC;
	//Calls the input function which will take user inputs, if the 		inputs are angle distance the input function will call the 		convert function 
	input();
	//the move function is called with the xy coordinates as it's 		input, the move function utilizes the rotate and drive_dist 		functions to reach the location
	move(x,y);
	return 1;
}


