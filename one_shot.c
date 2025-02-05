#include <stdio.h> 
#include "pico/stdlib.h" 
#include "pico/time.h"   

const uint LED_PIN_GREEN = 11;    
const uint LED_PIN_BLUE = 12;
const uint LED_PIN_RED = 13;
const uint BUTTON_PIN = 5; 

bool led_active = false;    
int led_state = 0;         

#define LED_DELAY 3000      

void rotina(){
    if (led_state == 0) {
        gpio_put(LED_PIN_RED, true);   
        gpio_put(LED_PIN_GREEN, true); 
        gpio_put(LED_PIN_BLUE, true);  
    } else if (led_state == 1) {
        gpio_put(LED_PIN_RED, true);   
        gpio_put(LED_PIN_GREEN, true); 
        gpio_put(LED_PIN_BLUE, false); 
    } else if (led_state == 2) {
        gpio_put(LED_PIN_RED, true);  
        gpio_put(LED_PIN_GREEN, false); 
        gpio_put(LED_PIN_BLUE, false);  
    } else if (led_state == 3) {
        gpio_put(LED_PIN_RED, false);  
        gpio_put(LED_PIN_GREEN, false); 
        gpio_put(LED_PIN_BLUE, false);  
    }

    led_state = (led_state + 1) % 4;
}

int64_t led_cycle_callback(alarm_id_t id, void *user_data) {
    rotina();
    if (led_state != 3) {
        add_alarm_in_ms(LED_DELAY, led_cycle_callback, NULL, false);
    } else {
        led_active = false;
    }

    return 0; 
}

int main() {
    stdio_init_all();

    gpio_init(LED_PIN_RED);
    gpio_set_dir(LED_PIN_RED, GPIO_OUT);
    gpio_init(LED_PIN_BLUE);
    gpio_set_dir(LED_PIN_BLUE, GPIO_OUT);
    gpio_init(LED_PIN_GREEN);
    gpio_set_dir(LED_PIN_GREEN, GPIO_OUT);

    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);

    gpio_pull_up(BUTTON_PIN);

    while (true) {
        if (gpio_get(BUTTON_PIN) == 0 && !led_active) {
            sleep_ms(50);

            if (gpio_get(BUTTON_PIN) == 0) {
                led_active = true;  
                led_state = 0;   
                rotina();        
                
                add_alarm_in_ms(LED_DELAY, led_cycle_callback, NULL, false);
            }
        }

        sleep_ms(10);
    }

    return 0;  
}
