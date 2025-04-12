#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "input.h"

void copy_fit(char *destination, const char *src) {
    int i = 0;
    while (src[i] != '\n') {
        destination[i] = src[i];
        i++;
    }
    destination[i] = '\0';
}

sa_entry_list *parse_file(char *filename) {
    FILE *file = fopen(filename, "r");
    if(file == NULL) {
        printf("File not found.\n");
        return NULL;
    }

    char line[100];
    int num_lines = 0;
    while(fgets(line, 100, file) != NULL) {
        num_lines++;
    }
    rewind(file);
    sa_entry *entries = (sa_entry *)malloc(sizeof(sa_entry) * num_lines/18);
    for(int curr_entry = 0; curr_entry < num_lines/18; curr_entry++) {
        sa_entry new_entry;
        for(int j = 0; j < 18; j++) {
            fgets(line, 100, file);
            line[strcspn(line, "\n\r")] = '\0';
            switch(j) {
                case 0: new_entry.id = (!strcmp(line, " ") ? 0 : atoi(line)); break;
                case 1: copy_fit(new_entry.subject, line); break;
                case 2: copy_fit(new_entry.catalog, line); break;
                case 3: copy_fit(new_entry.section, line); break;
                case 4: copy_fit(new_entry.career, line); break;
                case 5: new_entry.credits = (!strcmp(line, " ") ? 0 : atoi(line)); break;
                case 6: copy_fit(new_entry.campus, line); break;
                case 7: copy_fit(new_entry.session, line); break;
                case 8: copy_fit(new_entry.description, line); break;
                case 9: copy_fit(new_entry.instruction, line); break;
                case 10: copy_fit(new_entry.hdl, line); break;
                case 11: copy_fit(new_entry.req_add, line); break;
                case 12: new_entry.e_cap = atoi(line); break;
                case 13: new_entry.e_total = atoi(line); break;
                case 14: new_entry.s_avail = atoi(line); break;
                case 15: copy_fit(new_entry.c_avail, line); break;
                case 16: new_entry.wait_list = atoi(line); break;
                case 17: copy_fit(new_entry.instructor, line); break;
            }
        }
        entries[curr_entry] = new_entry;
    }
    fclose(file);
    sa_entry_list *e_list = (sa_entry_list*)malloc(sizeof(sa_entry_list));
    e_list->n_entries = num_lines / 18;
    e_list->entries = entries;
    return e_list;
}

void print_entry(sa_entry *sa_entry) {
    if(sa_entry == NULL) {
        printf("Entry does not exist.\n");
        return;
    }

    printf("----------------------------------\n");
    printf("ID: %d\n", sa_entry->id);
    printf("Subject: %s\n", sa_entry->subject);
    printf("Catalog #: %s\n", sa_entry->catalog);
    printf("Section: %s\n", sa_entry->section);
    printf("Career: %s\n", sa_entry->career);
    printf("Credits: %d\n", sa_entry->credits);
    printf("Campus: %s\n", sa_entry->campus);
    printf("Session: %s\n", sa_entry->session);
    printf("Description: %s\n", sa_entry->description);
    printf("Instructions: %s\n", sa_entry->instruction);
    printf("Hour/day/location: %s\n", sa_entry->hdl);
    printf("Required additional sections: %s\n", sa_entry->req_add);
    printf("Enrollment Capacity: %d\n", sa_entry->e_cap);
    printf("Enrollment Total: %d\n", sa_entry->e_total);
    printf("Seats available: %d\n", sa_entry->s_avail);
    printf("Capacity available: %s\n", sa_entry->c_avail);
    printf("Wait List Capacity: %d\n", sa_entry->wait_list);
    printf("Instructor(s): %s\n", sa_entry->instructor);
    printf("----------------------------------\n");
}