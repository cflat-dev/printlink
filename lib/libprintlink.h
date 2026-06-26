#ifndef LIBMOON3DP_H
#define LIBMOON3DP_H

typedef struct printer printer_t;

printer_t *find_printer(void);
const char *send_gcode(printer_t *p, const char *src);
void free_printer(printer_t *p);

#endif
