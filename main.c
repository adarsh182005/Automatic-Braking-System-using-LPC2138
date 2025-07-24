#include <lpc213x.h>
#include "Timer.h"
#include "Ultrasonic.h"
#include "LCD.H"


void delay()
{
 unsigned int i;
	for(i=0;i<1275;i++);
}

unsigned int range, i;
unsigned int value;


void motor_forward(int speed) {
    IOSET1 = 1<<16;
    IOCLR1 = 1<<17;
   
 PWMMR1=speed;
 PWMLER=1<<1;
    
}


void motor_stop() {
    IOCLR0|=3<<16;
	  PWMMR1=0;
	  PWMLER=1<<1;
}

int main() {
    VPBDIV = 0x01; 
    IODIR0 |= 0x00FF3400; 
    IODIR1 = 3<<16;
    IOSET1 = 1<<16;
    IOCLR0|=~(1<<17);
   PINSEL0|=2<<0;
  PWMTCR = 0x02;	
	PWMPR = 235164;	
	PWMMR0 = 255;
	PWMMR1=40;
	PWMMCR = 0X02;	
	PWMLER = 0x03;	
	PWMPCR = 1<<9;	
	PWMTCR = 0x09;
    ultrasonic_init();
    lcd_init();
    show("Distance :");
    
    while (1) {
		
        cmd(0x8A);
        range = get_range();
        dat((range / 100) % 10 + 48);
        dat((range / 10) % 10 + 48);
        dat((range % 10) + 48);
        show("cm");

        
        if (range < 50) {
            motor_stop(); 
            cmd(0xC0);
            show("CRASH WARNING  ");
        } 
else if (range <= 100 && range >= 50) {
            
            int speed = (100 - range) * 2; 
            motor_forward(speed);
                cmd(0xC0);
                show("CRASH WARNING  ");
	        
}
                
else if (range > 100) {
            motor_forward(254); 
            cmd(0xC0);
            show("You are safe :) ");

        }
    }

    return 0;
}
