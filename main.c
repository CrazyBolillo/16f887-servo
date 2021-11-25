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
#define SERVO_ZERO 500
#define SERVO_PULSE 20000
#define ORDER_LEN 1

unsigned char key;
uint8_t key_count = 0;

uint8_t order_count = 0;
uint8_t orders[ORDER_LEN][2]; // First element = degrees. Second = time on pos.

uint8_t servo_count = 0;

void read_instruction() {
    lcd_clear_display();
    lcd_write_string("Posicion  :");
    lcd_move_cursor(0x09);
    lcd_write_char(49 + order_count);
    lcd_move_cursor(0x40);
    lcd_write_string("Segundos: ");
    lcd_move_cursor(0x0C);
    lcd_display(true, true, true);
    
    key_count = 0;
    orders[order_count][0] = 0;
    while (key_count != 3) {
        key = key_int(keypad_read());
        if (key <= 9) {
            lcd_write_char(48 + key);
            switch (key_count) {
                case 0:
                     orders[order_count][0] += (100 * key);
                    break;
                case 1:
                    orders[order_count][0] += (10 * key);
                    break;
                case 2:
                    orders[order_count][0] += key;
            }
            key_count++;
        }
    }
    lcd_move_cursor(0x4A);
    key = 100;
    while (key > 9) {
        key = key_int(keypad_read());
        if (key < 9) {
            lcd_write_char(48 + key);
            orders[order_count][1] = key;
        }
    }
    __delay_ms(500);
}

void servo_zero() {
    SERVO = 1;
    __delay_us(SERVO_ZERO);
    SERVO = 0;
    __delay_us(SERVO_PULSE - SERVO_ZERO);
}

void main() {
    OSCCON = 0x71;
    while (OSCCONbits.HTS == 0);
    
    PORTC = 0x00;
    TRISC = 0x00;
    
    lcd_init(true, false, false);
    keypad_init();
    lcd_clear_display();
    servo_zero();
    
    while (1) {
        order_count = 0;
        while (order_count != ORDER_LEN) {
            read_instruction();
            order_count++;
        }
        lcd_display(true, false, false);
        lcd_clear_display();
        lcd_write_string("   Trabajando   ");
        
        order_count = 0;
        while (order_count != ORDER_LEN) {
            servo_count = 0;
            SERVO = 1;
            __delay_us(500);
            while (servo_count != orders[order_count][0]) {
                __delay_us(1);
                servo_count++;
            }
            SERVO = 0;
            servo_count = 0;
            while (servo_count != orders[order_count][1]) {
                __delay_ms(1000);
                servo_count++;
            }
            order_count++;
        }
        
        servo_zero();
    }
}
