//******Initialize Bluetooth using USART********//
void Init_Bluetooth()
{
   //Set the pins of RX and TX//
    TRISC6=1;
    TRISC7=1;
    
  //Set the baud rate using the look up table in datasheet(pg114)//
    BRGH=1;      //Always use high speed baud rate with Bluetooth else it wont work
    SPBRG  =129;
    
    //Turn on Asyc. Serial Port//
    SYNC=0;
    SPEN=1;
    
    //Set 8-bit reception and transmission
    RX9=0;
    TX9=0;

   //Enable transmission and reception//
    TXEN=1; 
    CREN=1; 
    
    //Enable global and ph. interrupts//
    GIE = 1;
    PEIE= 1;
  
    //Enable interrupts for Tx. and Rx.//
    RCIE=1;
    TXIE=1;
}
//___________BT initialized_____________//
//Function to load the Bluetooth Rx. buffer with one char.//
void BT_load_char(char byte)  
{
    TXREG = byte;
    while(!TXIF);  
    while(!TRMT);
}
//End of function//
//Function to Load Bluetooth Rx. buffer with string//
void BT_load_string(char* string)
{
    while(*string)
    BT_load_char(*string++);
}
//End of function//

//Function to broadcast data from RX. buffer//
void broadcast_BT()
{
  TXREG = 13;  
  __delay_ms(500);
}
//End of function//

//Function to get a char from Rx.buffer of BT//
char BT_get_char(void)   
{
    if(OERR) // check for over run error 
    {
        CREN = 0;
        CREN = 1; //Reset CREN
    }
    
    if(RCIF==1) //if the user has sent a char return the char (ASCII value)
    {
    while(!RCIF);  
    return RCREG;
    }
    else //if user has sent no message return 0
        return 0;
}
//End of function/