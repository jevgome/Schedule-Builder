#include <stdio.h>
#include <stdlib.h>

#include "input.h"
#include "course.h"

int main() {
    entry_list *e_list = parse_file("data.txt");
    course_list *c_list = parse_entries(e_list);

    for(int i = 0; i < c_list->n_courses; i++) {
        print_course(&c_list->courses[i]);
    }

    for(int i = 0; i < c_list->n_courses; i++) 
        free(c_list->courses[i].blocks);
    
    free(c_list->courses);
    free(c_list);
    free(e_list->entries);
    free(e_list);

    return 0;
}