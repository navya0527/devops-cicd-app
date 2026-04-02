#include <iostream>
#include <curl/curl.h>
using namespace std;

int main() {
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();

    if(curl) {
        string url = "https://dxxzokrwudxbgmalkali.supabase.co/rest/v1/vehicles";

        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "apikey: eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6ImR4eHpva3J3dWR4YmdtYWxrYWxpIiwicm9sZSI6ImFub24iLCJpYXQiOjE3NzUwOTUxMjcsImV4cCI6MjA5MDY3MTEyN30.5Iie8croGju4gACuBkQAYWZ2iz2qqev00IYdZGy5e1g");
        headers = curl_slist_append(headers, "Authorization: Bearer eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6ImR4eHpva3J3dWR4YmdtYWxrYWxpIiwicm9sZSI6ImFub24iLCJpYXQiOjE3NzUwOTUxMjcsImV4cCI6MjA5MDY3MTEyN30.5Iie8croGju4gACuBkQAYWZ2iz2qqev00IYdZGy5e1g");
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, "Prefer: return=minimal");

        string data = R"({
            "id": 201,
            "model": "TestCar",
            "rent": 3000,
            "available": true
        })";

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());

        res = curl_easy_perform(curl);

        if(res != CURLE_OK)
            cout << "Error: " << curl_easy_strerror(res) << endl;
        else
            cout << "Data inserted successfully!\n";

        curl_easy_cleanup(curl);
    }

    return 0;
}