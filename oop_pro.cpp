#include <iostream>
#include <string>
#include <curl/curl.h>

using namespace std;

// 🔥 PUT YOUR API KEY
string API_KEY = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6ImR4eHpva3J3dWR4YmdtYWxrYWxpIiwicm9sZSI6ImFub24iLCJpYXQiOjE3NzUwOTUxMjcsImV4cCI6MjA5MDY3MTEyN30.5Iie8croGju4gACuBkQAYWZ2iz2qqev00IYdZGy5e1g";

// ================= COMMON POST =================
void sendPost(string url, string json) {
    CURL *curl = curl_easy_init();

    if (curl) {
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, ("apikey: " + API_KEY).c_str());
        headers = curl_slist_append(headers, ("Authorization: Bearer " + API_KEY).c_str());
        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json.c_str());

        CURLcode res = curl_easy_perform(curl);

        cout << (res == CURLE_OK ? "✅ Data sent to cloud\n" : "❌ Error\n");

        curl_easy_cleanup(curl);
    }
}

// ================= CALLBACK =================
size_t writeCallback(void* contents, size_t size, size_t nmemb, string* output) {
    output->append((char*)contents, size * nmemb);
    return size * nmemb;
}

// ================= GET VEHICLE BY ID =================
string getVehicleById(int vid) {
    CURL *curl = curl_easy_init();
    string response;

    if (curl) {
        string url = "https://dxxzokrwudxbgmalkali.supabase.co/rest/v1/vehicles?vehicle_id=eq." + to_string(vid);

        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, ("apikey: " + API_KEY).c_str());
        headers = curl_slist_append(headers, ("Authorization: Bearer " + API_KEY).c_str());

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }

    return response;
}

// ================= UPDATE AVAILABILITY =================
void updateVehicleAvailability(int vid) {
    CURL *curl = curl_easy_init();

    if (curl) {
        string url = "https://dxxzokrwudxbgmalkali.supabase.co/rest/v1/vehicles?vehicle_id=eq." + to_string(vid);
        string json = "{\"is_available\":false}";

        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, ("apikey: " + API_KEY).c_str());
        headers = curl_slist_append(headers, ("Authorization: Bearer " + API_KEY).c_str());
        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PATCH");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json.c_str());

        curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        cout << "🚗 Vehicle marked as NOT AVAILABLE\n";
    }
}

// ================= TABULAR DISPLAY =================
void getVehicles() {
    CURL *curl = curl_easy_init();
    string response;

    if (curl) {
        string url = "https://dxxzokrwudxbgmalkali.supabase.co/rest/v1/vehicles";

        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, ("apikey: " + API_KEY).c_str());
        headers = curl_slist_append(headers, ("Authorization: Bearer " + API_KEY).c_str());

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        CURLcode res = curl_easy_perform(curl);

        if (res == CURLE_OK) {

            cout << "\n=========== VEHICLE LIST ===========\n";
            cout << "ID\tBrand\tModel\tRent\tStatus\n";
            cout << "-----------------------------------\n";

            size_t pos = 0;

            while ((pos = response.find("\"vehicle_id\":", pos)) != string::npos) {

                int id = stoi(response.substr(pos + 13));

                size_t b = response.find("\"brand\":\"", pos);
                string brand = response.substr(b + 9, response.find("\"", b + 9) - (b + 9));

                size_t m = response.find("\"model\":\"", pos);
                string model = response.substr(m + 9, response.find("\"", m + 9) - (m + 9));

                size_t r = response.find("\"rent_per_day\":", pos);
                float rent = stof(response.substr(r + 15));

                size_t a = response.find("\"is_available\":", pos);
                bool available = response.substr(a + 15, 5).find("true") != string::npos;

                cout << id << "\t" << brand << "\t" << model << "\t"
                     << rent << "\t" << (available ? "Available" : "Not Available") << endl;

                pos++;
            }

            cout << "===================================\n";

        } else {
            cout << "❌ Failed to fetch data\n";
        }

        curl_easy_cleanup(curl);
    }
}

// ================= CUSTOMER =================
void addCustomer() {
    int id;
    string name, phone, license;

    cout << "Customer ID: "; cin >> id;
    cout << "Name: "; cin >> name;
    cout << "Phone: "; cin >> phone;
    cout << "License No: "; cin >> license;

    string json = "{";
    json += "\"customer_id\":" + to_string(id) + ",";
    json += "\"name\":\"" + name + "\",";
    json += "\"phone\":\"" + phone + "\",";
    json += "\"license_no\":\"" + license + "\"}";

    sendPost("https://dxxzokrwudxbgmalkali.supabase.co/rest/v1/customer", json);
}

// ================= VEHICLE =================
void addVehicle() {
    int id;
    string brand, model;
    float rent;

    cout << "Vehicle ID: "; cin >> id;
    cout << "Brand: "; cin >> brand;
    cout << "Model: "; cin >> model;
    cout << "Rent per day: "; cin >> rent;

    string json = "{";
    json += "\"vehicle_id\":" + to_string(id) + ",";
    json += "\"brand\":\"" + brand + "\",";
    json += "\"model\":\"" + model + "\",";
    json += "\"rent_per_day\":" + to_string(rent) + ",";
    json += "\"is_available\":true}";

    sendPost("https://dxxzokrwudxbgmalkali.supabase.co/rest/v1/vehicles", json);
}

// ================= RENTAL =================
void createRental() {
    int rid, cid, vid, days;

    cout << "Rental ID: "; cin >> rid;
    cout << "Customer ID: "; cin >> cid;
    cout << "Vehicle ID: "; cin >> vid;
    cout << "Number of days: "; cin >> days;

    string data = getVehicleById(vid);

    if (data == "[]") {
        cout << "❌ Vehicle not found\n";
        return;
    }

    if (data.find("\"is_available\":false") != string::npos) {
        cout << "❌ Vehicle booked\n";
        return;
    }

    float rent = 0;

    size_t pos = data.find("rent_per_day");
    if (pos != string::npos) {
        rent = stof(data.substr(pos + 14));
    }

    float total = rent * days;

    cout << "💰 Rent/day: " << rent << endl;
    cout << "💰 Total: " << total << endl;

    string json = "{";
    json += "\"rental_id\":" + to_string(rid) + ",";
    json += "\"customer_id\":" + to_string(cid) + ",";
    json += "\"vehicle_id\":" + to_string(vid) + ",";
    json += "\"rent_date\":\"2026-04-02\",";
    json += "\"return_date\":\"2026-04-05\",";
    json += "\"total_cost\":" + to_string(total) + "}";

    sendPost("https://dxxzokrwudxbgmalkali.supabase.co/rest/v1/rental", json);

    updateVehicleAvailability(vid);
}

// ================= PAYMENT =================
void makePayment() {
    int pid, rid;
    float amount;

    cout << "Payment ID: "; cin >> pid;
    cout << "Rental ID: "; cin >> rid;
    cout << "Amount: "; cin >> amount;

    string json = "{";
    json += "\"payment_id\":" + to_string(pid) + ",";
    json += "\"rental_id\":" + to_string(rid) + ",";
    json += "\"amount\":" + to_string(amount) + ",";
    json += "\"payment_mode\":\"Cash\",";
    json += "\"date_payment\":\"2026-04-02\"}";

    sendPost("https://dxxzokrwudxbgmalkali.supabase.co/rest/v1/payment", json);
}

// ================= MAIN =================
int main() {
    int choice;

    do {
        cout << "\n===== VEHICLE RENTAL SYSTEM =====\n";
        cout << "1. Add Customer\n";
        cout << "2. Add Vehicle\n";
        cout << "3. Create Rental\n";
        cout << "4. Make Payment\n";
        cout << "5. Show Vehicles (Cloud)\n";
        cout << "6. Exit\n";
        cout << "Enter choice: ";

        cin >> choice;

        switch(choice) {
            case 1: addCustomer(); break;
            case 2: addVehicle(); break;
            case 3: createRental(); break;
            case 4: makePayment(); break;
            case 5: getVehicles(); break;
            case 6: cout << "Exiting...\n"; break;
            default: cout << "Invalid choice\n";
        }

    } while(choice != 6);

    return 0;
}