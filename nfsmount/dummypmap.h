/*
 * dummyportmap.h
 *
 * Functions for the portmap spoofer
 */

#include <stdio.h>

int bind_portmap(void);
int dummy_portmap(int sock, FILE *portmap_file);

