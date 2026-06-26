#include <stdio.h>

#include "libmoon3dp.h"

int main(void)
{
    printer_t *p = find_printer();
    if (!p) {
        printf("No printer found\n");
        return 1;
    }

    printf("Printer says: %s\n", send_gcode(p, "M115"));
    free_printer(p);

}
