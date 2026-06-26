#include <stdio.h>
#include <string.h>
#include <glob.h>
#include <libserialport.h>
#include <stdlib.h>

#include "libprintlink.h"

struct printer {
    struct sp_port *port;
};



static int try_open(struct printer *p, const char *path) {
    if (sp_get_port_by_name(path, &p->port) != SP_OK)
        return 0;

    if (sp_open(p->port, SP_MODE_READ_WRITE) != SP_OK)
        return 0;

    sp_set_baudrate(p->port, 115200);
    sp_set_bits(p->port, 8);
    sp_set_parity(p->port, SP_PARITY_NONE);
    sp_set_stopbits(p->port, 1);
    sp_set_flowcontrol(p->port, SP_FLOWCONTROL_NONE);

    return 1;
}

printer_t *find_printer(void) {
    static const char *patterns[] = {
        "/dev/ttyACM*",
        "/dev/ttyUSB*",
        NULL
    };

    struct printer *p = calloc(1, sizeof(*p));
    glob_t g;

    for (int i = 0; patterns[i]; i++) {
        if (glob(patterns[i], 0, NULL, &g) == 0) {
            for (size_t j = 0; j < g.gl_pathc; j++) {
                if (try_open(p, g.gl_pathv[j])) {
                    globfree(&g);
                    return p;
                }
            }
        }
        globfree(&g);
    }

    free(p);
    return NULL;
}

void send_gcode(printer_t *p, const char *src) {
    if (!p || !p->port)
        return;

    size_t len = strlen(src);

    // Write the Gcode line
    sp_blocking_write(p->port, src, len, 1000);

    // Ensure newline is sent
    sp_blocking_write(p->port, "\n", 1, 1000);
}

int get_printer_reply(printer_t *p,char *buf,size_t buf_len) {
    if (!p || !p->port)
        return -1;

    int n = sp_blocking_read(p->port, buf, buf_len - 1, 2000);

    if (n < 0)
        return -1;

    buf[n] = '\0';
    return n;
}


void free_printer(printer_t *p) {
    if (!p) return;
    if (p->port) {
        sp_close(p->port);
        sp_free_port(p->port);
    }
    free(p);
}
