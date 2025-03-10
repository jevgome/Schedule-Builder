#ifndef INPUT_H
#define INPUT_H

typedef struct entry_tag {
    int id, credits, e_cap, e_total, s_avail, wait_list;
    char hdl[40], subject[5], catalog[7], section[7], career[15], campus[6], session[50], description[50], instruction[50], location[50], req_add[100], instructor[50], c_avail[100];
} entry;

typedef struct entry_list_tag {
    int n_entries;
    entry *entries;
} entry_list;

void print_entry(entry* entry);
entry_list *parse_file(char *filename);
void copy_fit(char *destination, const char *src);

#endif