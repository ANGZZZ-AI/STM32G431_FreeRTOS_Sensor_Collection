#include "cmd.h"
#include "led.h"
#include "adc.h"
#include "usart.h"
#include <string.h>
#include <stdio.h>

void CMD_Parse(const char *cmd)
{
    if (strncmp(cmd, "led on", 6) == 0)
    {
        LED_Set(7);
        printf("[OK] LED8 ON\r\n");
    }
    else if (strncmp(cmd, "led off", 7) == 0)
    {
        LED_Clear(7);
        printf("[OK] LED8 OFF\r\n");
    }
    else if (strncmp(cmd, "data", 4) == 0)
    {
        float v = Get_ADC_POT();
        int vw = (int)v;
        int vf = (int)((v - vw) * 100 + 0.5f);
        if (vf < 0) vf = -vf;
        printf("[DATA] POT: %d.%02d V\r\n", vw, vf);
    }
    else
    {
        printf("[WARN] Unknown: %s\r\n", cmd);
        printf("  CMD: led on / led off / data\r\n");
    }
}
