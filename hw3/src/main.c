#include "includes/screen.h"
#include "includes/desc_tables.h"
#include "includes/pic.h"
#include "includes/serial_port.h"
#include "includes/task.h"

void first_thread(void){
    int counter = 0;
    while(1){
        counter++;
        printf("First thread message: %d\n",counter);
    }
}

void second_thread(void){
    int counter = 0;
    while(1){
        counter++;
        printf("Second thread message: %d\n",counter);
    }
}

void cmain(void)
{
    cls();
    
    init_tables();
    init_timer();
    //init_serial();
    
    add_task(first_thread);
    add_task(second_thread);

    asm volatile ("sti");
    
    int counter = 0;
    while(1){
        printf("Main thread message: %d\n",counter);
    }
}
