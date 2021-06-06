void Init_Bluetooth()
{
   //Khai báo các chân RX,TX
    TRISC6=1;
    TRISC7=1;
    
  //chọn baud rate với tần số 20M và baudrate = 9k6 theo datasheet
    BRGH = 1;      //Chọn baudrate ở tốc độ cao
    SPBRG = 129;
    
    // config để Thiết lập chế độ bán đồng bộ. Serial Port//
    SYNC=0;
    SPEN=1;
    
    //thiết lập 8bit reception và 8bit transmission
    RX9=0;
    TX9=0;

   
    TXEN=1; // TXEN = 1 gửi data lt
    CREN=1; // CREN = 1 cho phép nhận data liên tục
    
    //Bật các flag này để sử dụng interupt cho TX RX
    GIE = 1;
    PEIE= 1;
  
    //Config interupt cho TX và RX//
    RCIE=1;
    TXIE=1;
}

//Load đến RX của bluetooth 1 ký tự
void BT_load_char(char byte)  
{
   // gán dữ liệu vào thanh ghi TXREG 
    TXREG = byte;
   // Check status bit TRMT, và TXIF để xem đã gửi xong dữ liệu chưa
    while(!TXIF);  
    while(!TRMT);
}


// load 1 string đến RX bluetooth - sử dụng lại hàm loadchar
void BT_load_string(char* string)
{
    while(*string)
    BT_load_char(*string++);
}



//Function to broadcast data from RX. buffer//
void broadcast_BT()
{
  TXREG = 13;  // 13 là ký tự xuống dòng, đánh dấu kết thúc dữ liệu
  __delay_ms(500);
}



char BT_get_char(void)   
{
    if(OERR) // check for over run error 
    {
        CREN = 0;
        CREN = 1; //Reset CREN
    }
    
    if(RCIF==1)
    {
      while(!RCIF);  //Dùng polling, liên tục check interrupt flag bit RCIF xem đã nhận data xong chưa
      return RCREG;
    }
    else //nếu không gửi => return 0
        return 0;
}
