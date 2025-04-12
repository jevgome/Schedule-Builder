#include <stdio.h>
#include <curl/curl.h>
#include <json-c/json.h>

#ifndef SCRAPER_H
#define SCRAPER_H

#define BUFFER_SIZE 100000

typedef enum days {MON, TUE, WED, THU, FRI, SAT, SUN} days;
typedef enum attribute_tag {CA1, CA2, CA3, CA4, CA4INT, TOI1, TOI2, TOI3, TOI4, TOI5, TOI6, Q, E, W} attribute;

typedef enum class_type_tag {
    LECTURE, LAB, DISCUSSION, LECTURE_STANDALONE, OTHER
} class_type;

typedef enum request_tag {SEARCH_CLASS, COURSE_DETAIL} request_type;

typedef struct entry_tag {
    class_type type;
    int id, link_id, e_cap, s_avail, wait_list, is_open, credits, attributes[14], start_time[7], end_time[7], days[7], is_enroll;
    char section[7], code[15], career[15], campus[50], session[50], title[100], description[1000], restrictions[1000], instructor[50], instruction[50];
} entry;

typedef struct entry_list_tag {
    int n;
    entry* e_list;
} entry_list;

typedef struct block_tag {
    class_type type;
    days day;
    int id;
    char section[7];
    int start_time;
    int end_time;
    char location[15];
    char instructors[50];
    char instruction[50];
    int e_cap; 
    int s_avail;
    int wait_list;
    int is_open;
    char c_avail[100];
} block;

typedef struct course_tag {
    int id, credits, attributes[14];
    char code[15], career[15], campus[6], session[50], title[100], description[1000], restrictions[1000];
} course;

typedef struct course_list_tag {
    int n;
    course* c_list;
} course_list;

// Struct to hold the response
struct response_buffer {
    char data[BUFFER_SIZE];
};

typedef struct curl_request_tag {
    CURL *curl;
    CURLcode res;
    struct curl_slist* headers;
} curl_request;

size_t write_callback(void *ptr, size_t size, size_t nmemb, void *userdata);
curl_request setup_curl(int n, char** h, char* request, char* payload, struct response_buffer* response);
struct CURLResponse GetRequest(CURL *curl_handle, const char *url);
entry_list search_class(char* query, char** filters);
void url_encode(char* dest, char* src);
int convert_to_minutes(const char* time);
void print_entry(entry* entry);
#endif