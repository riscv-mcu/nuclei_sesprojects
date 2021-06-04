#include "nuclei_demosoc.h"

#if defined(DEBUG_SEMIHOST) && DEBUG_SEMIHOST == 0

int puts(const char * string)
{
    const char * t = string;
    const char * v = string;
    int i = 0;
    while(*t!='\0')
    {
        i++;
        t++;
    }
    int j = 0;
    for(j;j<=i;j++)
        __putchar((v[j]),0);
    __putchar('\n',0);
    return 0;
}

int __putchar(int ch,__printf_tag_ptr unused)
{
    if (ch == '\n') {
        uart_write(SOC_DEBUG_UART, '\r');
    }
    uart_write(SOC_DEBUG_UART, ch);

    return ch;
}
#endif