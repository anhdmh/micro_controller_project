//cac chan cua LCD duoc ket noi voi vi dieu khien
#define LCD_RS RD1                                    //RS noi vao RD1
#define LCD_EN RD0                                    //EN noi vao RD0
#define LCD_D4 RD4                                    //D4 noi vao RD4
#define LCD_D5 RD5                                    //D5 noi vao RD5
#define LCD_D6 RD6                                    //D6 noi vao RD6
#define LCD_D7 RD7                                    //D7 noi vao RD7
#define LCD_TRIS TRISD                                
//------------------------------------------------------------------------------


void LCD_Send4Bit( unsigned char a){ 
    LCD_D4= a    & 0x01;                               
    LCD_D5=(a>>1)& 0x01;                               
    LCD_D6=(a>>2)& 0x01;                               
    LCD_D7=(a>>3)& 0x01;                    
    
    LCD_EN = 1;                                         
    __delay_us(5);// delay cho 
    LCD_EN = 0;
    __delay_us(40); 
}

void LCD_SendCommand (unsigned char command ){
    LCD_Send4Bit  (command >>4);//Gui 4 bit cao ( dich phai 4 bit)
    LCD_Send4Bit  (command );  //Gui 4 bit thap
}

void LCD_Init (void){
    LCD_TRIS = 0;                               
    LCD_RS = 0;
    __delay_ms(20);
    
    LCD_Send4Bit(0b0011);                   //gui 4 bit 0011
    __delay_ms(10);
    LCD_Send4Bit(0b0011);                   //gui 4 bit 0011
    __delay_ms(10);
    LCD_Send4Bit(0b0011);                   //gui 4 bit 0011
    __delay_ms(10);
    LCD_Send4Bit(0b0010);                   //gui 4 bit 0010
    __delay_ms(10);
    
    LCD_SendCommand( 0x28 );
    LCD_SendCommand( 0x0C);
    LCD_SendCommand( 0x06 );
    LCD_SendCommand( 0x01 );
}


void LCD_PutChar (unsigned char data)
{
    LCD_RS=1;
    LCD_SendCommand (data);                    
    LCD_RS=0;
}

void LCD_PutString (char *s){
   while (*s)   {
      LCD_PutChar(*s);
      s++;
   }
}

void LCD_Clear() 
{
  LCD_SendCommand(0x01);             
  __delay_ms(5);
}

void LCD_Gotoxy(unsigned char row, unsigned char col)
{
  unsigned char address;    
  if(row==0) 
      address=0x80 + col;                 //can D7=1
  else address=0x80 + col+64;             //0x80=1000 0000  
  
  LCD_SendCommand(address);
  __delay_us(50);
}
