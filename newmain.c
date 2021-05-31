

#include <xc.h>
#include <stdint.h>
// CONFIG
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)


#define _XTAL_FREQ 20000000  // tan so thach anh 20MHz

#include"_DHT11.h"
#include"motors.h"
#include"bluetooth_module.h"
#include"_lcd_module.h"

int value_control = 0;
void print_status();
void print_option_BT();
void temp_warning();
void humi_warning();
void control_BT();

//--------START MAIN----------//
int main(void) {
    TRISD = 0x00;           
    LCD_Init();
    
    T1CON=0x20;
    TRISC2 = 0; //Trigger pin 
    TRISC3 = 1; //Echo pin      
    Pin_LED = 0x00;                             
    Data_LED = 1;
    
    water_init();
    init_fan();
    SERVO_PIN_DIR = 0x00;
    
    // khoi tao bluetooth
    Init_Bluetooth();
    BT_load_string("Bluetooth da duoc ket noi");
    broadcast_BT();
    
    DHT11_start();
    Read_DTH11();
    print_Temp_Humi_LCD();
    
    print_status();
    print_option_BT();
    
    while(1){         
        __delay_ms(1000);
        value_control = BT_get_char();
        DHT11_start();            
        Read_DTH11();
        
 
        control_BT();
        print_Temp_Humi_LCD();
                
        water_start();
        auto_food();
        
        temp_warning();
        humi_warning(); 
    }
} 
//--------END MAIN----------//


// In cac lua chon ra BT
void print_option_BT() {
    BT_load_string("Nhan phim de chon cac chuc nang sau day: ");
    broadcast_BT();
    BT_load_string("  1: Hien thi trang thai trong chuong");
    broadcast_BT();
    BT_load_string("  2: Cho ga an");
    broadcast_BT();
    BT_load_string("  3: Bat quat gio");
    broadcast_BT();
    BT_load_string("  4: Tat quat gio");
    broadcast_BT();
}

// In trang thai ra BT
void print_status() {
    BT_load_string("Nhiet do hien tai trong chuong: ");
    BT_load_string(getStringData(temp));
    BT_load_string(" do C");
    broadcast_BT();
    BT_load_string("Do am hien tai trong chuong: ");
    BT_load_string(getStringData(humidity));
    BT_load_string(" %");
    broadcast_BT();
    BT_load_string("Quat Thong Gio: ");
    if (RB2 == 1) BT_load_string("ON");
        else BT_load_string("OFF");
    broadcast_BT();
    BT_load_string("May bom nuoc: ");
    if (PIN_Water_Sensor == 0) BT_load_string("ON");
        else BT_load_string("OFF");
    broadcast_BT();
    BT_load_string("Luong thuc an: ");
    switch(get_level_food()) {
        case 1: BT_load_string("DA HET"); break;
        case 2: BT_load_string("CON IT"); break;
        case 3: BT_load_string("TRUNG BINH"); break;
        default: BT_load_string("DAY DU");
    }
    broadcast_BT();
}

// Canh bao nhiet do
void temp_warning() {
    if (temp > 35) {
        BT_load_string("WARNING! nhiet do trong chuong qua CAO!");
        broadcast_BT();
        __delay_ms(500);
    } else if(temp < 15) {
        BT_load_string("WARNING! nhiet do trong chuong qua THAP!");
        broadcast_BT();
        __delay_ms(500);
    }
}

// canh bao do am
void humi_warning() {
    if (humidity > 75) {
        BT_load_string("WARNING! do am trong chuong qua CAO!");
        broadcast_BT();
        __delay_ms(500);
    } else if(humidity < 55) {
        BT_load_string("WARNING! do am trong chuong qua THAP!");
        broadcast_BT();
        __delay_ms(500);
    }
}

// dieu khien PIC bang BT
void control_BT() {
    if (value_control == '1') {
        print_status();
        value_control = 0;
    }
    
    if (value_control == '2') {
        runFood();
        value_control = 0;
        BT_load_string("Dang cho an...!");
        broadcast_BT();
    }
    
    if (value_control == '3') {
        fan_on();
        value_control = 0;
        BT_load_string("Da Bat quat...!");
        broadcast_BT();
    }
    
    if (value_control == '4') {
        fan_off();
        value_control = 0;
        BT_load_string("Da tat quat...!");
        broadcast_BT();
    }
}