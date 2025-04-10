#ifndef SCHEDULE_H
#define SCHEDULE_H

#include "course.h"

typedef struct schedule_tag {
    int n;
    course* course_pointers[10];
} schedule;

schedule create_schedule(int n, course** course_pointers);

int time_to_int(const char* time);

int check_collisions(int c_n, course** course_buff);

schedule* generate_schedules(int c_n, course_list** c_lists);

void print_schedule(schedule* s);

#endif