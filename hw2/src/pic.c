#include "includes/pic.h"
#include "includes/helpers.h"

void send_eoi(uint8_t irq)
{
    if (irq >= 8)
    {
        outb(0x20, 0xA0);
    }

    outb(0x20, 0x20);
}

static void set_mask(uint8_t irq_line, uint8_t mask) 
{
    uint16_t port;
    uint8_t value;
 
    if (irq_line < 8)
    {
        port = 0x21;
    }
    else 
    {
        port = 0xA1;
        irq_line -= 8;
    }

    outb(port, mask);        
}

static void remap_pic(uint32_t offset1, uint32_t offset2)
{
    // Инициализация
    outb(0x10 + 0x01, 0x20);    
    outb(0x10 + 0x01, 0xA0);

    // Смещения перенаправления адресов 
    outb(offset1, 0x21);        // Для master 
    outb(offset2, 0xA1);        // Для slave

    // Подцепляем slave к master
    outb(0x04, 0x21);           
    outb(0x02, 0xA1);

    // Для 8086
    outb(0x01, 0x21);           
    outb(0x01, 0xA1);
}

static void set_timer()
{
    outb (0x34, 0x43);          // Выбор режима работы
    outb (0xFF, 0x40);          // Верхний байт сброса
    outb (0xFF, 0x40);          // Нижний байт сброса
} 

void init_timer(){
    remap_pic(0x20, 0x28);
    set_mask(0, 0xFE);
    set_timer();
}