#ifndef INPUT_H
#define INPUT_H

typedef struct sa_entry_tag {
    int id, credits, e_cap, e_total, s_avail, wait_list;
    char hdl[40], subject[5], catalog[7], section[7], career[15], campus[6], session[50], description[50], instruction[50], location[50], req_add[100], instructor[50], c_avail[100];
} sa_entry;

typedef struct sa_entry_list_tag {
    int n_entries;
    sa_entry *entries;
} sa_entry_list;

void print_entry(sa_entry* sa_entry);
sa_entry_list *parse_file(char *filename);
void copy_fit(char *destination, const char *src);

#endif