#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "schedule.h"

sa_schedule create_schedule(int n, sa_course** course_pointers) {
    sa_schedule new_schedule;
    new_schedule.n = n;
    for(int i = 0; i < n; i++) {
        new_schedule.course_pointers[i] = course_pointers[i];
    }
    return new_schedule;
}

int time_to_int(const char* time) {
    char hour[3];
    char minute[3];
    int i = 0;
    while(time[i] != ':') {
        hour[i] = time[i];
        i++;
    }
    hour[i] = '\0';
    i++;
    int j = 0;
    while(i < strlen(time) - 2) {
        minute[j] = time[i];
        i++;
        j++;
    }
    minute[j] = '\0';

    return (atoi(hour) + (time[i] == 'P' && atoi(hour) != 12 ? 12 : 0)) * 60 + atoi(minute);

}

int check_collisions(int c_n, sa_course** course_buff) {
    if(c_n < 2) return 0;

    for(int i = 1; i < c_n; i++) {
        for(int j = i-1; j >= 0; i++) {
            for(int k = 0; j < course_buff[i]->n_b; k++) {
                for(int l = 0; l < course_buff[j]->n_b; l++) {
                    sa_block block1 = course_buff[i]->blocks[k], block2 = course_buff[j]->blocks[l];
                    if(block1.day == block2.day 
                        && (time_to_int(block1.start_time) -  time_to_int(block2.end_time) < 15 
                            || time_to_int(block2.start_time) -  time_to_int(block1.end_time) < 15)) 
                            return 1;
                }
            }
        }
    }

    return 0;
}

sa_schedule* generate_schedules(int c_n, sa_course_list** c_lists) {
    if(c_n == 0) {
        printf("No courses found.\n");
        return NULL;
    }
    sa_schedule temp_schedules[10000];
    int indexes[c_n];
    for(int i = 0; i < c_n; i++) indexes[i] = 0;
    int num = 0, curr = 0, flag = 1;
    
    sa_course* course_buff[c_n];
    for(int i = 0; i < c_n; i++) course_buff[i] = &(c_lists[i]->courses[0]);
    if(check_collisions(c_n, course_buff) == 0) {
        temp_schedules[0] = create_schedule(c_n, course_buff);
        num = 1;
    }
    

    while(flag) {
        curr = 0;
        if(indexes[0] + 1 < c_lists[0]->n_courses) indexes[0]++;
        else {
            while(curr < c_n && indexes[curr] == c_lists[curr]->n_courses - 1)
                curr++;
            
            if(curr < c_n) {
                int i = curr - 1;
                while(i >= 0) {
                    indexes[i] = 0;
                    i--;
                }
                indexes[curr]++;
            }

            else flag = 0;
        }

        if(flag) {
            for(int i = 0; i < c_n; i++) course_buff[i] = &(c_lists[i]->courses[0]);
            if(check_collisions(c_n, course_buff) == 0) {
                temp_schedules[0] = create_schedule(c_n, course_buff);
                num++;
            }
        }
    }

    sa_schedule* sa_schedules = (sa_schedule*) malloc(sizeof(sa_schedule) * num);
    for(int i = 0; i < num; i++) sa_schedules[i] = temp_schedules[i];
    return sa_schedules;
}

void print_schedule(sa_schedule* s) {
    if(s == NULL) {
        printf("Schedule does not exist.");
        return;
    }

    char *days[] = {"Monday","Tuesday","Wednesday","Thursday","Friday","Saturday","Sunday"};

    int ijk[7][10][3];

    for(int i = 0; i < 7; i++) {
        for(int j = 0; j < s->n; j++) {
            for(int k = 0; k < s->course_pointers[j]->n_b; k++) {
                sa_block b = s->course_pointers[j]->blocks[k];
                
            }
        }
    }
}