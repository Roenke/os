#include "includes/screen.h"
#include "includes/isr.h"
#include "includes/helpers.h"
#include "includes/pic.h"
#include "includes/task.h"
#include "includes/serial_port.h"

int threads = 0;            // количество потоков
int cur_thread = 0;         // номер текущего потока
state_t regs[100];     // список потоков
int isInitialized = 0;      // флаг инициализации 

void isr_handler(state_t state)
{

    static int skipped;
    if(state.int_num != 32){
        printf ("Unhandled exception %d\n", state.int_num);
        asm volatile ("cli");   
        asm volatile ("hlt");
    }

    if (!isInitialized)
    {
        // Инициализация многопоточности - записываем данные kernel-потока, выставляем глобальные переменные
        regs[0] = state;
        threads = 1;
        cur_thread = 0;
        isInitialized = 1;
        printf("Inited.\n");
    }
    else
    {
        // Смена потоков
        regs[cur_thread] = state;                   // Сохраняем текущий контекст
        state = regs[(cur_thread + 1) % threads];   // Загружаем контекст следующего потока
    
        cur_thread = (cur_thread + 1) % threads;    // Меняем номер текущего потока
    }

    send_eoi(0);
}
