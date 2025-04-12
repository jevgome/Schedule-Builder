#include <stdio.h>
#include <stdlib.h>

#include "input/scraper.h"

int main(int argc, char* argv[]) {
    if(argc != 3) {
        printf("Usage: %s subject catalog\n", argv[0]);
        exit(-1);
    }
    char query[200];
    sprintf(query, "%s %s", argv[1], argv[2]);
    char** filters;
    entry_list entries = search_class(query, filters);
    for(int i = 0; i < entries.n; i++) {
        print_entry(&entries.e_list[i]);
    }
    free(entries.e_list);
    return 0;
}