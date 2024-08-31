/************** UART Receiving ****************/
#include<reg51.h>
#define lcd_data P2
sbit lcd_D7= P2^7;
sbit lcd_rs =P0^7;
sbit lcd_rw =P0^6;
sbit lcd_en =P0^5;


void delay_ms(unsigned char ms);

void lcd_busy();
void hex2bcd(unsigned int hexvalue);
void lcd_command(unsigned char var);
void lcd_senddata(unsigned char var);
void lcd_init();
void delay();
unsigned char bcd2ascii(unsigned char hexvalue);

unsigned char counter = 0;

unsigned char bcd_digit1;
unsigned char bcd_digit2;
unsigned char bcd_digit3;

unsigned char ascii_val1;
unsigned char ascii_val2;
unsigned char ascii_val3;

unsigned int i;
static unsigned int rflag = 0;
unsigned char rx_msg = 0;

void main()
{
//	unsigned char x;
	
	TMOD = 0X20;
	TH1 = 0XFD; 
	SCON = 0x50;     // Enable serial receiption of variable baud rate  9600
	IE = 0X92;    	// Enable serial port 0,Timer 0 Interrupt
	TR1 = 1;
	lcd_init();
	while(1)
	{


		if(rflag == 1)
		{
		  lcd_senddata(rx_msg);
		  delay();
			rx_msg = 0;
			rflag = 0;
		}

	}
}
	
void delay()
{
	unsigned int j=0;
	for(j=0;j<3500;j++);
} 
void hex2bcd(unsigned int hex_value)
{
	unsigned char remainder = 0;
	bcd_digit3 = hex_value/100;
	remainder = hex_value%100;
	bcd_digit2 = remainder/10;
	bcd_digit1 = remainder%10;
}
unsigned char bcd2ascii(unsigned char bcdvalue)
{
	volatile unsigned char x = 0;
	volatile unsigned char ascii_result = 0;
	x = (bcdvalue&0x0f);
	ascii_result =(x+0x30);
	return(ascii_result);
} 
void lcd_init()
{
	lcd_command(0x38);delay();
	lcd_command(0x0f);delay();
	lcd_command(0x01);delay();
	lcd_command(0x06);delay();
}
void lcd_busy()
{
	lcd_D7=1;
		delay();
	lcd_en=1;					
		delay();
	lcd_rs=0;	
		delay();
	lcd_rw=1;					
		delay();

	while(lcd_D7)
	{
		lcd_en=0;					delay();
		lcd_en=1;					delay();
	}
}	
void lcd_command(unsigned char var)
{
	lcd_data=var;
	lcd_rs=0;
	lcd_rw=0; 
	lcd_en=1;
	lcd_en=0;
	lcd_busy();
}
void lcd_senddata(unsigned char var)
{
	lcd_data=var;
	lcd_rs=1;
	lcd_rw=0;
	lcd_en=1;
	lcd_en=0 ;
	lcd_busy();
}

/*-----------------------------DELAY Routine--------------------------------------*/
void delay_ms(unsigned char ms)
{
  unsigned int t1, t2;

   for(t1=0; t1<ms; t1++){
      for(t2=0; t2<334; t2++);
   }
}

	void serial0()interrupt 4
	{
		 if(RI==1)
		{
			rx_msg=SBUF;
			rflag=1;
			RI=0;
		}
	}

