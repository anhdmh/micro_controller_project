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
    
    // phát 1 xung high-to-low transition của tín hiệu chân E để đưa các bit vào thanh ghi.
    LCD_EN = 1;                                         
    __delay_us(5);
    LCD_EN = 0;
    __delay_us(40); 
}

void LCD_SendCommand (unsigned char command ){
    LCD_Send4Bit  (command >>4);//Gui 4 bit cao ( 4 bit đầu)
    LCD_Send4Bit  (command );  //Gui 4 bit thap  (4 bit cuối)
}

void LCD_Init (void){
    LCD_TRIS = 0;                               
    LCD_RS = 0;
    __delay_ms(20); // đợi trên 15ms trước khi VCC lên 4.5V
   
    // Tiếp tục khởi tạo theo đúng lưu đồ của nxs 
    //  khi kết nối mạch theo giao thức 4bit, 4 bit thấp từ DB0-DB3 không được kết nối đến LCD
    //  nên lệnh khởi tạo ban đầu (lệnh chọn giao thức giao tiếp – function set 0010****) phải giao tiếp theo chế độ 8 bit(bỏ qua 4 bit thấp)
    LCD_Send4Bit(0b0011);                   //gui 4 bit 0011
    __delay_ms(10);
    LCD_Send4Bit(0b0011);                   //gui 4 bit 0011
    __delay_ms(10);
    LCD_Send4Bit(0b0011);                   //gui 4 bit 0011
    __delay_ms(10);
    LCD_Send4Bit(0b0010);                   //gui 4 bit 0010
    __delay_ms(10);
    
    // Các lệnh sau phải gửi cả 2 bit cao và thấp thông qua SendCommand
    
    LCD_SendCommand( 0x28 ); // 0010 1000 function set: 4 bit(D7->D4); hiển thị 2 hàng và kí tự 5x8 điểm ảnh
    LCD_SendCommand( 0x0C); // 0000 1100 display off:  Hiển thị màn hình, không hiển thị con trỏ và nhấp nháy con trỏ
    LCD_SendCommand( 0x01 ); // 0000 0001 display clear
    LCD_SendCommand( 0x06 ); // 0000 0110 entry mode set: Tăng địa chỉ bộ đếm hiển thị AC mỗi khi ghi, vị trí con trỏ cũng di chuyển theo  
}


void LCD_PutChar (unsigned char data)
{
    LCD_RS=1; // set RS = 1 để ghi dữ liệu vào thanh ghi DR
    LCD_SendCommand (data);                    
    LCD_RS=0; // set RS = 0 để thanh ghi IR ra lệnh cho LCD(ở đây đang là display do đã ghi dữ liệu tại hàm init)
}

// sử dụng put char để làm lại
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
      address=0x80 + col;                 //D7 bằng 1 để cho phép ghi các giá trị tọa độ của DDRAM vào AC
  else address=0x80 + col+64;             //0x80=1000 0000  
  
  LCD_SendCommand(address); // gửi lệnh SetDDRAM address với địa chỉ đã cho ở trên
  __delay_us(50);
   //khi ở chế độ hiển thị 2 hàng, địa chỉ từ 00h đến 27h cho hàng thứ nhất, và từ 40h đến 67h cho hàng thứ 2.
}
