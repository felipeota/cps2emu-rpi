/*
 * Some Linux Maemo OS related 
 */

#include <stdio.h>
#include "maemo.h"

int file_exists(const char *filename){
    //Try open for read...
    FILE *fp;
    if (fp = fopen(filename, "r")) {
        fclose(fp);
        return 1;
    }
    return 0;
}
