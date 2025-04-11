#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <json-c/json.h>

#define BUFFER_SIZE 100000

// Struct to hold the response
struct response_buffer {
    char data[BUFFER_SIZE];
};

// Callback to write curl response into our buffer
size_t write_callback(void *ptr, size_t size, size_t nmemb, void *userdata) {
    size_t total_size = size * nmemb;
    struct response_buffer *buf = (struct response_buffer *)userdata;
    strncat(buf->data, (char *)ptr, total_size);
    return total_size;
}

void scrape() {
    CURL *curl;
    CURLcode res;
    struct curl_slist *headers = NULL;
    FILE* fd = fopen("output.txt", "w");

    // JSON payload: searchData
    const char *search_data =
        "{\"other\": {\"srcdb\": \"1258\"},\"criteria\": [{\"field\": \"alias\",\"value\": \"cse 2500\"}]}";
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if (curl) {
        headers = curl_slist_append(headers, "Accept: application/json, text/javascript, */*; q=0.01");
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, "Origin: https://classes.uconn.edu");
        headers = curl_slist_append(headers, "Referer: https://classes.uconn.edu/");
        headers = curl_slist_append(headers, "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64)");

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_URL, "https://classes.uconn.edu/api/?page=fose&route=search&alias=cse%202500");
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, search_data);
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, fwrite);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fd);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "Request failed: %s\n", curl_easy_strerror(res));
        }

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
    fclose(fd);
}