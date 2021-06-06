#define  Data_Pin RB0                 
#define  Data_Pin_TRIS TRISB0   
#define  Pin_LED TRISB1
#define  Data_LED RB1

int DATA[5]; 
uint8_t temp, humidity;

void LCD_Send4Bit( unsigned char a);
void LCD_SendCommand (unsigned char command );
void LCD_Init (void);
void LCD_PutChar (unsigned char data);
void LCD_PutString (char *s);
void LCD_Clear();
void LCD_Gotoxy(unsigned char row, unsigned char col);
void fan_on();

uint8_t DHT11_start()
{
    Data_Pin_TRIS = 0; // config chân data thành output
    Data_Pin = 0;
    __delay_ms(30); 
    // cho chân data xuống 0 trên 18ms sau đó kéo lên 1 (20 -> 40us) và thiết lập data thành input
    Data_Pin = 1; 
    Data_Pin_TRIS = 1;
    __delay_us(40);   
     // lúc này sẽ đợi dht11 phản hồi lại, nếu phản hồi data sẽ xuống 0 còn sau 40us mà không xuống => không giao tiếp được.
    if(Data_Pin == 0)                          
    {
        // data xuống 0 trong khoảng 80us và lên 1 trong khoảng 80us => đã giao tiếp thành công với dht11
        __delay_us(80);                
        if(Data_Pin == 1)
        {
            __delay_us(80);             
            return 1;    
        }
    }
}

/* Sau khi giao tiếp được với DHT11, DHT11 sẽ gửi liên tiếp 40 bit 0 hoặc 1 về MCU, tương ứng chia
* thành 5 byte kết quả của Nhiệt độ và độ ẩm.
*/
uint8_t Read_DTH11()
{
    // sau khi giao tiếp thành công => dht11 sẽ gửi về 5byte dữ liệu và nó sẽ được lưu trong mảng DATA đã khai báo
    // Vòng for đầu tương ứng với 5 byte dữ liệu
    for(uint8_t i = 0; i<5; i++){
        // vòng for thứ 2 tương ứng 8 bit dữ liệu
       for(uint8_t ii = 0; ii<8; ii++) {
        while(Data_Pin == 0);                        
        __delay_us(50);  
        // Trong khoảng thời gian lớn hơn 28ms mà datapin vẫn = 1 => bắt đầu gửi các bit
        if(Data_Pin == 1)                               
        {
            DATA[i] = DATA[i]<<1| 1;
            while(Data_Pin == 1){}
        }
        else  DATA[i] =DATA[i]<<1| 0;
       }
    }
    temp = DATA[2];
    humidity = DATA[0];
    // checksum
    if(DATA[4] == ( DATA[3]+ DATA[2]+DATA[1]+ DATA[0])) return 1;
}

// kiem tra nhiet do do am de dieu khien led va quat
void checkTemp(){
    if(DATA[2] <= 28) {
        Data_LED = 0;
    } else if(DATA[2] >= 35) {
        Data_LED = 1;
    }
    
    if (DATA[0] >= 75) {
        fan_on();
    }
}

// đổi từ số sang chuỗi
char* getStringData(uint8_t num) {
    char buff[2];
    buff[1] = num % 10 + 48;
    num = num / 10;
    buff[0] = num % 10 + 48;
    num = num / 10;
    buff[2] = '\0';
    return buff;
}

void print_Temp_Humi_LCD(){
    LCD_Clear();    
    checkTemp();
    LCD_PutString("Nhiet do = ");
    for(uint8_t i=13;i>11;i--){
        LCD_Gotoxy(0,i);                                 
        LCD_PutChar(DATA[2] %10+48);                     
        DATA[2]= DATA[2]/10;
    }
    
    //in do am
    LCD_Gotoxy(1,0); 
    LCD_PutString("Do am = ");
    for(uint8_t i=13;i>11;i--){
        LCD_Gotoxy(1,i);                                 
        LCD_PutChar(DATA[0] %10+48);     
        DATA[0]=DATA[0]/10;
    }
}

