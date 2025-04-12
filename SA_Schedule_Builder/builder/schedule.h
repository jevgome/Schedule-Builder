#ifndef SCHEDULE_H
#define SCHEDULE_H

#include "course.h"

typedef struct sa_schedule_tag {
    int n;
    sa_course* course_pointers[10];
} sa_schedule;

sa_schedule create_schedule(int n, sa_course** course_pointers);

int time_to_int(const char* time);

int check_collisions(int c_n, sa_course** course_buff);

sa_schedule* generate_schedules(int c_n, sa_course_list** c_lists);

void print_schedule(sa_schedule* s);

#endif