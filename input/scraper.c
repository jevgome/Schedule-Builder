#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <json-c/json.h>
#include "scraper.h"

// Callback to write curl response into our buffer
size_t write_callback(void *ptr, size_t size, size_t nmemb, void *userdata) {
    size_t total_size = size * nmemb;
    struct response_buffer *buf = (struct response_buffer *)userdata;
    strncat(buf->data, (char *)ptr, total_size);
    return total_size;
}

curl_request setup_curl(int n, char** h, char* request, char* payload, struct response_buffer* response) {
    curl_request new_curl;
    curl_global_init(CURL_GLOBAL_DEFAULT);
    new_curl.curl = curl_easy_init();
    new_curl.headers = NULL;
    if(new_curl.curl) {
        for(int i = 0; i < n; i++) new_curl.headers = curl_slist_append(new_curl.headers, h[i]);
        curl_easy_setopt(new_curl.curl, CURLOPT_URL, request);
        curl_easy_setopt(new_curl.curl, CURLOPT_POSTFIELDS, payload);
        curl_easy_setopt(new_curl.curl, CURLOPT_POST, 1L);
        curl_easy_setopt(new_curl.curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(new_curl.curl, CURLOPT_WRITEDATA, response);
    } else {
        printf("Something went wrong with cUrl.\n");
    }

    return new_curl;
}


void url_encode(char* dest, char* src) {
    char buf[BUFFER_SIZE];
    int i, j = 0;
    for(i = 0; i < strlen(src); i++) {
        switch (src[i]) {
            case ' ': buf[j] = '%'; buf[j+1] = '2'; buf[j+2] = '0'; j+=2; break;
            case '@': buf[j] = '%'; buf[j+1] = '4'; buf[j+2] = '0'; j+=2; break;
            case '#': buf[j] = '%'; buf[j+1] = '2'; buf[j+2] = '3'; j+=2; break;
            case '$': buf[j] = '%'; buf[j+1] = '2'; buf[j+2] = '4'; j+=2; break;
            case '%': buf[j] = '%'; buf[j+1] = '2'; buf[j+2] = '5'; j+=2; break;
            case '^': buf[j] = '%'; buf[j+1] = '5'; buf[j+2] = 'E'; j+=2; break;
            case '&': buf[j] = '%'; buf[j+1] = '2'; buf[j+2] = '6'; j+=2; break;
            case '+': buf[j] = '%'; buf[j+1] = '2'; buf[j+2] = 'B'; j+=2; break;
            case '=': buf[j] = '%'; buf[j+1] = '3'; buf[j+2] = 'D'; j+=2; break;
            case '[': buf[j] = '%'; buf[j+1] = '5'; buf[j+2] = 'B'; j+=2; break;
            case '{': buf[j] = '%'; buf[j+1] = '7'; buf[j+2] = 'B'; j+=2; break;
            case ']': buf[j] = '%'; buf[j+1] = '5'; buf[j+2] = 'D'; j+=2; break;
            case '}': buf[j] = '%'; buf[j+1] = '7'; buf[j+2] = 'D'; j+=2; break;
            case '\\': buf[j] = '%'; buf[j+1] = '5'; buf[j+2] = 'C'; j+=2; break;
            case '|': buf[j] = '%'; buf[j+1] = '7'; buf[j+2] = 'C'; j+=2; break;
            case ';': buf[j] = '%'; buf[j+1] = '3'; buf[j+2] = 'B'; j+=2; break;
            case ':': buf[j] = '%'; buf[j+1] = '3'; buf[j+2] = 'A'; j+=2; break;
            case '\'': buf[j] = '%'; buf[j+1] = '2'; buf[j+2] = '7'; j+=2;break;
            case '\"': buf[j] = '%'; buf[j+1] = '2'; buf[j+2] = '2'; j+=2; break;
            case ',': buf[j] = '%'; buf[j+1] = '2'; buf[j+2] = 'C'; j+=2; break;
            case '<': buf[j] = '%'; buf[j+1] = '3'; buf[j+2] = 'C'; j+=2; break;
            case '>': buf[j] = '%'; buf[j+1] = '3'; buf[j+2] = 'E'; j+=2; break;
            case '/': buf[j] = '%'; buf[j+1] = '2'; buf[j+2] = 'F'; j+=2; break;
            case '?': buf[j] = '%'; buf[j+1] = '3'; buf[j+2] = 'F'; j+=2; break;
            default: buf[j] = src[i];
        }
        j++;
    }
    buf[j] = '\0';
    strcpy(dest, buf);
}

// Scrapes data of query into an entry_list
entry_list search_class(char* query, char** filters) {
    char request[BUFFER_SIZE], payload[BUFFER_SIZE], encoded[(strlen(query)) * 3 + 1];
    url_encode(encoded, query);
    sprintf(request,"https://classes.uconn.edu/api/?page=fose&route=search&%s=%s", "alias", encoded); // change later
    sprintf(payload,"{\"other\":{\"srcdb\":\"%s\"},\"criteria\":[{\"field\":\"alias\",\"value\":\"%s\"}]}", "1258", query); // change later
    char* headers[] = {"Accept: application/json, text/javascript, */*; q=0.01","Content-Type: application/json","Origin: https://classes.uconn.edu","Referer: https://classes.uconn.edu/","User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64)"};
    
    struct response_buffer temp_response = { .data = "" };

    curl_request curl = setup_curl(5, headers, request, payload, &temp_response);
    
    curl.res = curl_easy_perform(curl.curl);
    if (curl.res != CURLE_OK) {
        fprintf(stderr, "Request failed: %s\n", curl_easy_strerror(curl.res));
    }
    
    struct json_object* json = json_tokener_parse(temp_response.data);
    struct json_object* results;
    int count;
    if(json_object_object_get_ex(json, "count", &results)) {
        count = json_object_get_int(results);
        if(count < 1) {
            printf("Aborting search because count variable not found\n");
        }
    } else {
        printf("Count not found\n");
    }
    
    if(!json_object_object_get_ex(json, "results", &results))
        printf("Results not found\n");
        
    struct json_object *info;
    char code[50];
    
    char id[count][10];
    entry *entries = (entry *)malloc(sizeof(entry) * count);
    for(int i = 0; i < count; i++) {
        entry new_entry;
        struct json_object *course = json_object_array_get_idx(results, i);
        if(i == 0) {
            json_object_object_get_ex(course, "code", &info);
            strcpy(new_entry.code, json_object_get_string(info));
            
            strcpy(code, json_object_get_string(info));
        }
        json_object_object_get_ex(course, "crn", &info);
        strcpy(id[i], json_object_get_string(info));
        new_entry.id = atoi(json_object_get_string(info));

        json_object_object_get_ex(course, "title", &info);
        strcpy(new_entry.title, json_object_get_string(info));

        json_object_object_get_ex(course, "no", &info);
        strcpy(new_entry.section, json_object_get_string(info));

        json_object_object_get_ex(course, "schd", &info);
        char type[4];
        strcpy(type, json_object_get_string(info));
        new_entry.type = (strcmp(type, "LEC") == 0 ? LECTURE : strcmp(type, "LSA") == 0 ? LECTURE_STANDALONE : strcmp(type, "LAB") == 0 ? LAB : strcmp(type, "DIS") == 0 ? DISCUSSION : OTHER);
        
        json_object_object_get_ex(course, "stat", &info);
        char stat[2];
        strcpy(stat, json_object_get_string(info));
        new_entry.is_open = (strcmp(stat, "A") == 0 ? 1 : 0);

        struct json_object *meeting_times;
        json_object_object_get_ex(course, "meetingTimes", &meeting_times);
        char t[200];
        sprintf(t, "{\"meetingTimes\": %s}", json_object_get_string(meeting_times));
        struct json_object* json3 = json_tokener_parse(t);
        json_object_object_get_ex(json3, "meetingTimes", &meeting_times);
        int num_days = json_object_array_length(meeting_times);
        for(int j = 0; j < 7; j++) {
            new_entry.days[j] = 0;
            new_entry.start_time[j] = 0;
            new_entry.end_time[j] = 0;
        }
        for(int j = 0; j < num_days; j++) {
            struct json_object *time, *element;
            time = json_object_array_get_idx(meeting_times, j);
            json_object_object_get_ex(time, "meet_day", &element);
            int day = atoi(json_object_get_string(element));
            new_entry.days[atoi(json_object_get_string(element))] = 1;
            
            json_object_object_get_ex(time, "start_time", &element);
            new_entry.start_time[day] = convert_to_minutes(json_object_get_string(element));

            json_object_object_get_ex(time, "end_time", &element);
            new_entry.end_time[day] = convert_to_minutes(json_object_get_string(element));
        }
        json_object_put(json3);
        
        json_object_object_get_ex(course, "instr", &info);
        strcpy(new_entry.instructor, json_object_get_string(info));

        json_object_object_get_ex(course, "acad_career", &info);
        strcpy(new_entry.career, json_object_get_string(info));

        json_object_object_get_ex(course, "is_enroll_section", &info);
        new_entry.is_enroll = atoi(json_object_get_string(info));
        if(new_entry.is_enroll) {
            json_object_object_get_ex(course, "linked_crns", &info);
            new_entry.link_id = atoi(json_object_get_string(info));
        } else {
            new_entry.link_id = 0;
        }

        entries[i] = new_entry;
    }

    
    char matched[1000] = "";
    for(int i = 0; i < count; i++) {
        strcat(matched, id[i]);
        if(i< count-1) strcat(matched, ",");
    }
    
    for(int i = 0; i < count; i++) {
        request[0] = '\0';
        payload[0] = '\0';
        sprintf(request,"https://classes.uconn.edu/api/?page=fose&route=details");
        sprintf(payload,"{\"group\":\"code:%s\",\"key\":\"crn:%s\",\"srcdb\":\"%s\",\"matched\":\"crn:%s\"}", code, id[i], "1258", matched); // change later
        curl_easy_setopt(curl.curl, CURLOPT_URL, request);
        curl_easy_setopt(curl.curl, CURLOPT_POSTFIELDS, payload);
        
        temp_response.data[0] = '\0';
        curl.res = curl_easy_perform(curl.curl);
        if (curl.res != CURLE_OK) {
            fprintf(stderr, "Request failed: %s\n", curl_easy_strerror(curl.res));
        }
        struct json_object* json2 = json_tokener_parse(temp_response.data);
        struct json_object* info2;
        
        json_object_object_get_ex(json2, "hours_html", &info2);
        entries[i].credits = atoi(json_object_get_string(info2));
        
        json_object_object_get_ex(json2, "seats", &info2);
        char seats[200];
        strcpy(seats, json_object_get_string(info2));
        char temp[200];
        int j;
        for(j = 0; j < 3; j++) temp[j] = seats[30 + j];
        temp[j] = '\0';

        entries[i].e_cap = atoi(temp);

        while(seats[30 + j] != ':') j++;

        int k;
        for(k = 0; k < 5; k++) temp[k] = seats[31 + j + k];
        temp[k] = '\0';
        // printf("%s\n", temp);
        entries[i].s_avail = atoi(temp);

        json_object_object_get_ex(json2, "camp_html", &info2);
        strcpy(entries[i].campus, json_object_get_string(info2));

        json_object_object_get_ex(json2, "instmode", &info2);
        strcpy(entries[i].instruction, json_object_get_string(info2));
        
        json_object_object_get_ex(json2, "session_html", &info2);
        strcpy(entries[i].session, json_object_get_string(info2));

        json_object_object_get_ex(json2, "description", &info2);
        strcpy(entries[i].description, json_object_get_string(info2));

        json_object_object_get_ex(json2, "registration_restrictions", &info2);
        strcpy(entries[i].restrictions, json_object_get_string(info2));

        json_object_object_get_ex(json2, "section_attributes", &info2);
        char attributes[200];
        strcpy(attributes, json_object_get_string(info2));
        for(int j = 0; j < 14; j++) entries[i].attributes[j] = 0;
        if(strlen(attributes)) {
            int j;
            for(j = 0; j < strlen(attributes); j++) {
                if(attributes[j] == ')') {
                    switch(attributes[j-1]) {
                        case '1': if(attributes[j-2] == 'A') entries[i].attributes[CA1] = 1; else entries[i].attributes[TOI1] = 1; break;
                        case '2': if(attributes[j-2] == 'A') entries[i].attributes[CA2] = 1; else entries[i].attributes[TOI2] = 1; break;
                        case '3': if(attributes[j-2] == 'A') entries[i].attributes[CA3] = 1; else entries[i].attributes[TOI3] = 1; break;
                        case '4': if(attributes[j-2] == 'A') entries[i].attributes[CA4] = 1; else entries[i].attributes[TOI4] = 1; break;
                        case '5': entries[i].attributes[TOI5] = 1; break;
                        case '6': entries[i].attributes[TOI6] = 1; break;
                        case 'T': entries[i].attributes[CA4INT] = 1; break;
                        case 'W': entries[i].attributes[W] = 1; break;
                        case 'E': entries[i].attributes[E] = 1; break;
                        case 'Q': entries[i].attributes[Q] = 1; break;
                    }
                }
            }
        }
        
        json_object_put(json2);
    }

    json_object_put(json);

    curl_slist_free_all(curl.headers);
    curl_easy_cleanup(curl.curl);
    
    curl_global_cleanup();

    entry_list new_list;
    new_list.e_list = entries;
    new_list.n = count;
    
    return new_list;
}

int convert_to_minutes(const char* time) {
    char hours[3], minutes[3];
    int i;
    if(strlen(time) == 3) { 
        hours[0] = time[0];
        hours[1] = '\0';
        for(i = 0; i < 2; i++) minutes[i] = time[i + 1];
        minutes[i] = '\0';
    } else {
        for(i = 0; i < 2; i++) hours[i] = time[i];
        hours[i] = '\0';
        for(i = 0; i < 2; i++) minutes[i] = time[i + 2];
        minutes[i] = '\0';
    }

    return atoi(minutes) + atoi(hours) * 60;
}

void print_entry(entry* entry) {
    if(!entry) {
        printf("No entry to print\n");
        return;
    }
    printf("ID: %d\n", entry->id);
    printf("Link ID: %d\n", entry->link_id);
    printf("Enrollment Capacity: %d\n", entry->e_cap);
    printf("Seats Available: %d\n", entry->s_avail);
    printf("Campus: %s\n", entry->campus);
    printf("Days: ");
    for(int i = 0; i < 7; i++) printf("%d ", entry->days[i]);
    printf("\n");
    printf("Credits: %d\n", entry->credits);
    printf("Start Times: ");
    for(int i = 0; i < 7; i++) printf("%d ", entry->start_time[i]);
    printf("\n");
    printf("End Times: ");
    for(int i = 0; i < 7; i++) printf("%d ", entry->end_time[i]);
    printf("\n");
    printf("\n");
}