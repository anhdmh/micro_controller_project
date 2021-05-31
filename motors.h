// Servo Controller
#define SERVO_PIN RB7
#define SERVO_PIN_DIR TRISB7
#define Trigger RC2
#define Echo RC3



int distance() {
    int time_taken;
    int distance;
    TMR1H =0; TMR1L =0; //clear the timer bits
        
        Trigger = 1; 
        __delay_us(10);           
        Trigger = 0;  
        
        while (Echo==0);
            TMR1ON = 1;
        while (Echo==1);
            TMR1ON = 0;
        
        time_taken = (TMR1L | (TMR1H<<8)); 
        distance= (0.0272*time_taken)/2;
            
        time_taken = time_taken * 0.8;
        return distance + 1;
}

void rotation0() //0 Degree
{
    unsigned int i;
    for(i=0;i<50;i++) {
        SERVO_PIN = 1;
        __delay_us(1000);
        SERVO_PIN = 0;
        __delay_us(19200);
    }
}

void rotation90() {
    unsigned int i;
    for(i=0;i<50;i++)
    {   
        SERVO_PIN = 1;
        __delay_us(1500);
        SERVO_PIN = 0;
        __delay_us(18500);
    }
}

// Cho an
void runFood() {
    rotation90();
    __delay_ms(2000);
    rotation0();
}

void auto_food() {
    if (distance() >= 10) {
        runFood();
    }
}

uint8_t get_level_food() {
    uint8_t d = distance();
    if (d > 10) return 1;
    if (d >=8) return 2;
    if (d > 6) return 3;
    return 0;
}

// ----- PUMB -----------
#define PIN_Water_Sensor RB4
#define PIN_Water_Sensor_Dir TRISB4


void water_init() {
    PIN_Water_Sensor_Dir = 1;
    TRISB5 = 0x00;
    TRISB6 = 0x00;
    RB5 = 0;
    RB6 = 0;
}

void water_start() {
    if (PIN_Water_Sensor == 1) {
        RB6 = 0;
    } else {
        RB6 = 1;
    }
}

// Quat Gio

void init_fan() {
    TRISB2 = 0x00;
    TRISB3 = 0x00;
    RB2 = 0;
    RB3 = 0;
}

void fan_on() {
    RB2 = 1;
}

void fan_off() {
    RB2 = 0;
}


