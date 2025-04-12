#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../input/input.h"
#include "course.h"

sa_block create_block(char* section, char* start_time, char* end_time, days day, char* location, char* instructors) {
    sa_block new_block;
    new_block.section = section;
    strcpy(new_block.start_time, start_time);
    strcpy(new_block.end_time, end_time);
    strcpy(new_block.location, location);
    new_block.instructors = instructors;
    new_block.day = day;
    return new_block;
}

sa_course create_course(int id, char* subject, char* catalog, sa_block* blocks, int n_b) {
    sa_course new_class;
    new_class.id = id;
    new_class.subject = subject;
    new_class.catalog = catalog;
    new_class.blocks = blocks;
    new_class.n_b = n_b;
    return new_class;
}

void print_block(sa_block* block) {
    if(block == NULL) {
        printf("Block does not exist.\n");
        return;
    }

    printf("----------------------------------\n");
    printf("Section %s\n", block->section);
    printf("Time: %s - %s\n", block->start_time, block->end_time);
    char* names[] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
    printf("Day: %s\n", names[block->day]);
    printf("Location: %s\n", block->location);
    printf("Instructor(s): %s\n", block->instructors);
    printf("----------------------------------\n");
}

void print_course(sa_course *course) {
    if(course == NULL) {
        printf("Course does not exist.\n");
        return;
    }
    for(int i = 0; i < course->n_b; i++) printf("%20s  ", "--------------------------");
    printf("\n");
    for(int i = 0; i < course->n_b; i++) printf("|%20s%4s|  ", course->subject, "");
    printf("\n");
    for(int i = 0; i < course->n_b; i++) printf("|%20s%4s|  ", course->catalog, "");
    printf("\n");
    for(int i = 0; i < course->n_b; i++) printf("|%20s%4s|  ", course->blocks[i].section, "");
    printf("\n");
    for(int i = 0; i < course->n_b; i++) printf("|%20s%4s|  ", course->blocks[i].start_time, "");
    printf("\n");
    for(int i = 0; i < course->n_b; i++) printf("|%20s%4s|  ", course->blocks[i].end_time, "");
    printf("\n");
    char *days[] = {"Monday","Tuesday","Wednesday","Thursday","Friday","Saturday","Sunday"};
    for(int i = 0; i < course->n_b; i++) printf("|%20s%4s|  ", days[course->blocks[i].day], "");
    printf("\n");
    for(int i = 0; i < course->n_b; i++) printf("|%20s%4s|  ", course->blocks[i].location, "");
    printf("\n");
    for(int i = 0; i < course->n_b; i++) printf("|%20s%4s|  ", course->blocks[i].instructors, "");
    printf("\n");
    for(int i = 0; i < course->n_b; i++) printf("%20s  ", "--------------------------");
    printf("\n");
}

void parse_hdl_from_file(const char *hdl, char *start_time, char *end_time, days *days, char *location) {
    int i = 0;
    char d[2];
    while(hdl[i] != ' ') {
        start_time[i] = hdl[i];
        i++;
    }
    start_time[i] = '\0';
    i += 3;
    int j = 0;
    while(hdl[i] != ' ') {
        end_time[j] = hdl[i];
        i++;
        j++;
    }
    end_time[j] = '\0';
    i += 3;
    int l = 0;
    while(hdl[i] != ' ') {
        for(int k = 0; k < 2; k++) {
            d[k] = hdl[i];
            i++;
        }
        switch(d[0]) {
            case 'M':
                days[l] = MON;
                l++;
                break;
            case 'T':
                days[l] = (d[1] == 'u' ? TUE : THU);
                l++;
                break;
            case 'W':
                days[l] = WED;
                l++;
                break;
            case 'F':
                days[l] = FRI;
                l++;
                break;
            case 'S':
                days[l] = (d[1] == 'u' ? SUN : SAT);
                l++;
                break;
        }
    }
    days[l] = -1;
    
    i += 3;
    if(i < strlen(hdl)-2) {
        j = 0;
        for(; i < strlen(hdl); i++) {
            location[j] = hdl[i];
            j++;
        }
        location[j] = '\0';
    } else{
        copy_fit(location, "N/A");
    }
}

int get_num_courses(sa_entry_list *e_list) {
    int num_courses = 0;
    for(int curr_entry = 0; curr_entry < e_list->n_entries; curr_entry++) if(e_list->entries[curr_entry].id != 0) num_courses++;
    return num_courses;
}

// Discontinued
sa_course_list *parse_entries_from_file(sa_entry_list *e_list) {
    int num_courses = get_num_courses(e_list);
    int curr_course = 0;

    sa_course *courses = (sa_course*) malloc(sizeof(sa_course) * num_courses);
    
    sa_block block_buf[5];
    int curr_block;
    int cb = 0;
    for(int curr_entry = 0; curr_entry < e_list->n_entries; curr_entry++) {
        days d[10];
        char start_time[10];
        char end_time[10];
        char location[20];
        
        curr_block = 0;
        if(e_list->entries[curr_entry].id == 0) {
            while(e_list->entries[curr_entry].id == 0) {
                parse_hdl_from_file(e_list->entries[curr_entry].hdl, start_time, end_time, d, location);
                int i = 0;
                while(d[i] != -1) {
                    block_buf[curr_block] = create_block(e_list->entries[curr_entry].section, start_time, end_time, d[i], location, e_list->entries[curr_entry].instructor);
                    curr_block++;
                    i++;
                }
                curr_entry++;
            }
            cb = curr_block;
            parse_hdl_from_file(e_list->entries[curr_entry].hdl, start_time, end_time, d, location);
            int i = 0;
            while (d[i] != -1) {
                block_buf[curr_block] = create_block(e_list->entries[curr_entry].section, start_time, end_time, d[i], location, e_list->entries[curr_entry].instructor);
                curr_block++;
                i++;
            }
        } else {
            if(strcmp(e_list->entries[curr_entry].req_add, " ")){
                curr_block = cb;
            }
            parse_hdl_from_file(e_list->entries[curr_entry].hdl, start_time, end_time, d, location);
            int i = 0;
            while (d[i] != -1) {
                block_buf[curr_block] = create_block(e_list->entries[curr_entry].section, start_time, end_time, d[i], location, e_list->entries[curr_entry].instructor);
                curr_block++;
                i++;
            }
        }

        sa_block *blocks = (sa_block *)malloc(sizeof(sa_block) * curr_block);
        for(int i = 0; i < curr_block; i++) {
            blocks[i] = block_buf[i];
        }
        
        courses[curr_course] = create_course(e_list->entries[curr_entry].id, e_list->entries[curr_entry].subject, e_list->entries[curr_entry].catalog, blocks, curr_block);
        curr_course++;
    }
    sa_course_list *c_list = (sa_course_list *)malloc(sizeof(sa_course_list));
    c_list->n_courses = num_courses;
    c_list->courses = courses;
    return c_list;
    
}

sa_course_list *parse_entries_from_web(sa_entry_list *e_list) {
    int num_courses = get_num_courses(e_list);
    int curr_course = 0;

    sa_course *courses = (sa_course*) malloc(sizeof(sa_course) * num_courses);
    
    sa_block block_buf[5];
    int curr_block;
    int cb = 0;
    for(int curr_entry = 0; curr_entry < e_list->n_entries; curr_entry++) {
        days d[10];
        char start_time[10];
        char end_time[10];
        char location[20];
        
        curr_block = 0;
        if(e_list->entries[curr_entry].id == 0) {
            while(e_list->entries[curr_entry].id == 0) {
                parse_hdl_from_file(e_list->entries[curr_entry].hdl, start_time, end_time, d, location);
                int i = 0;
                while(d[i] != -1) {
                    block_buf[curr_block] = create_block(e_list->entries[curr_entry].section, start_time, end_time, d[i], location, e_list->entries[curr_entry].instructor);
                    curr_block++;
                    i++;
                }
                curr_entry++;
            }
            cb = curr_block;
            parse_hdl_from_file(e_list->entries[curr_entry].hdl, start_time, end_time, d, location);
            int i = 0;
            while (d[i] != -1) {
                block_buf[curr_block] = create_block(e_list->entries[curr_entry].section, start_time, end_time, d[i], location, e_list->entries[curr_entry].instructor);
                curr_block++;
                i++;
            }
        } else {
            if(strcmp(e_list->entries[curr_entry].req_add, " ")){
                curr_block = cb;
            }
            parse_hdl_from_file(e_list->entries[curr_entry].hdl, start_time, end_time, d, location);
            int i = 0;
            while (d[i] != -1) {
                block_buf[curr_block] = create_block(e_list->entries[curr_entry].section, start_time, end_time, d[i], location, e_list->entries[curr_entry].instructor);
                curr_block++;
                i++;
            }
        }

        sa_block *blocks = (sa_block *)malloc(sizeof(sa_block) * curr_block);
        for(int i = 0; i < curr_block; i++) {
            blocks[i] = block_buf[i];
        }
        
        courses[curr_course] = create_course(e_list->entries[curr_entry].id, e_list->entries[curr_entry].subject, e_list->entries[curr_entry].catalog, blocks, curr_block);
        curr_course++;
    }
    sa_course_list *c_list = (sa_course_list *)malloc(sizeof(sa_course_list));
    c_list->n_courses = num_courses;
    c_list->courses = courses;
    return c_list;
}