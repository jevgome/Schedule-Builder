#include "../input/input.h"

#ifndef COURSE_H
#define COURSE_H

typedef enum days {MON, TUE, WED, THU, FRI, SAT, SUN} days;

typedef struct sa_block_tag {
    char* section;
    char start_time[8];
    char end_time[8];
    days day;
    char location[15];
    char* instructors;
} sa_block;

typedef struct course_tag {
    int id;
    char* subject;
    char* catalog;
    sa_block* blocks;
    int n_b;
} sa_course;

typedef struct course_list_tag {
    int n_courses;
    sa_course *courses;
} sa_course_list;

sa_block create_block(char* section, char* start_time, char* end_time, days day, char* location, char* instructors);

sa_course create_course(int id, char* subject, char* catalog, sa_block* blocks, int n_b);

void print_block(sa_block* block);
void print_course(sa_course *course);

void parse_hdl_from_file(const char *hdl, char *start_time, char *end_time, days *days, char *location);
int get_num_courses(sa_entry_list *entries);
sa_course_list *parse_entries_from_file(sa_entry_list *e_list);
sa_course_list *parse_entries_from_web(sa_entry_list *e_list);
#endif