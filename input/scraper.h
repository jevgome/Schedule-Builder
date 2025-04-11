#include <stdio.h>
#include <curl/curl.h>


#ifndef SCRAPER_H
#define SCRAPER_H

struct CURLResponse
{
    char *html;
    size_t size;
};

static size_t WriteHTMLCallback(void *contents, size_t size, size_t nmemb, void *userp);
struct CURLResponse GetRequest(CURL *curl_handle, const char *url);
void scrape();

#endif