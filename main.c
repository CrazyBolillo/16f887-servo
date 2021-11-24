#pragma config DEBUG = 1
#pragma config LVP = 0
#pragma config FCMEN = 0
#pragma config IESO = 0
#pragma config BOREN = 00
#pragma config CPD = 0
#pragma config CP = 0
#pragma config MCLRE = 0
#pragma config PWRTE = 1
#pragma config WDTE = 0
#pragma config FOSC = 101

#include <stdint.h>
#include "config.h"
#include "lcd.h"
#include "keypad.h"

#define SERVO PORTCbits.RC2

void vdelay_us(uint16_t us) {
    for (uint16_t count = 0; count != us; count++) {
        __delay_us(1);
    }
}

void servo_zero() {
    SERVO = 1;
    __delay_us(1500);
    SERVO = 0;
    __delay_us(18500);
}

uint16_t serv_control = 100;

void main() {
    OSCCON = 0x71;
    while (OSCCONbits.HTS == 0);
    
    PORTC = 0x00;
    TRISC = 0x00;
    
    lcd_init(true, false, false);
    keypad_init();
    lcd_clear_display();
    lcd_write_string("Bailaservo");
    
    while (1) {
        servo_zero();
        __delay_ms(1000);
        for (uint8_t x = 1; uint8_t <= 9; x++) {
            SERVO = 1;
        }
    }
}
