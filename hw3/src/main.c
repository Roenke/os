#include "includes/screen.h"
#include "includes/desc_tables.h"
#include "includes/pic.h"
#include "includes/serial_port.h"
#include "includes/task.h"

char buf;
int is_ready_to_write = 0;

void first_thread(void){
    char ch;
    while(1){
        buf = read_serial();
        is_ready_to_write = 1;
    }
}

void second_thread(void){
    while(1){
        while(!is_ready_to_write);
        printf("%c", buf);
        is_ready_to_write = 0;
    }
}

void cmain(void)
{
    cls();
    
    init_tables();
    init_timer();
    init_serial();
    
    // add_task(first_thread);
    // add_task(second_thread);
    asm volatile ("sti");
    asm volatile ("int $0x20");

    addNewThread(first_thread);
    addNewThread(second_thread);

    

    int counter = 0;
    while(1){
        if(counter % 100000 == 0)
            printf("Main thread message: %d\n",counter++);
    }
}
