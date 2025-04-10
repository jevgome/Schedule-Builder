#include "input.h"

#ifndef COURSE_H
#define COURSE_H

typedef enum days {MON, TUE, WED, THU, FRI, SAT, SUN} days;

typedef struct block_tag {
    char* section;
    char start_time[8];
    char end_time[8];
    days day;
    char location[15];
    char* instructors;
} block;

typedef struct course_tag {
    int id;
    char* subject;
    char* catalog;
    block* blocks;
    int n_b;
} course;

typedef struct course_list_tag {
    int n_courses;
    course *courses;
} course_list;

block create_block(char* section, char* start_time, char* end_time, days day, char* location, char* instructors);

course create_course(int id, char* subject, char* catalog, block* blocks, int n_b);

void print_block(block* block);
void print_course(course *course);

void parse_hdl(const char *hdl, char *start_time, char *end_time, days *days, char *location);
int get_num_courses(entry_list *entries);
course_list *parse_entries(entry_list *entries);

#endif