#ifndef LIBMOON3DP_H
#define LIBMOON3DP_H

typedef struct printer printer_t;

printer_t *find_printer(void);
void send_gcode(printer_t *p, const char *src);
int get_printer_reply(printer_t *p,char *buf,size_t buf_len);
void free_printer(printer_t *p);

#endif
