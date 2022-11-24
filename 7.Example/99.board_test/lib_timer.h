#include "app.h"

// 每 1ms 触发1次 handle

void timer0_init(void);
// void timer1_init(void);

extern char timer0_handle(uint32_t cnt);
// extern char timer1_handle(uint32_t cnt);
