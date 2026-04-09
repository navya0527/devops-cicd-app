#include <iostream>
#include <string>
#include <curl/curl.h>

using namespace std;

string API_KEY = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6ImR4eHpva3J3dWR4YmdtYWxrYWxpIiwicm9sZSI6ImFub24iLCJpYXQiOjE3NzUwOTUxMjcsImV4cCI6MjA5MDY3MTEyN30.5Iie8croGju4gACuB kQAYWZ2iz2qqev00IYdZGy5e1g";
string BASE_URL = "https://dxxzokrwudxbgmalkali.supabase.co/rest/v1";

// ================= ANSI COLORS =================
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define CYAN    "\033[36m"
#define BOLD    "\033[1m"
#define RESET   "\033[0m"

// ================= CALLBACK =================
size_t writeCallback(void* contents, size_t size, size_t nmemb, string* output) {
    output->append((char*)contents, size * nmemb);
    return size * nmemb;
}

// ================= HEADERS HELPER =================
struct curl_slist* buildHeaders() {
    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, ("apikey: " + API_KEY).c_str());
    headers = curl_slist_append(headers, ("Authorization: Bearer " + API_KEY).c_str());
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, "Prefer: return=minimal");
    return headers;
}

// ================= COMMON GET =================
string sendGet(string url) {
    CURL* curl = curl_easy_init();
    string response;

    if (curl) {
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, ("apikey: " + API_KEY).c_str());
        headers = curl_slist_append(headers, ("Authorization: Bearer " + API_KEY).c_str());

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        curl_easy_perform(curl);
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }
    return response;
}

// ================= COMMON POST =================
bool sendPost(string url, string json) {
    CURL* curl = curl_easy_init();
    string response;
    bool success = false;

    if (curl) {
        struct curl_slist* headers = buildHeaders();

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        long http_code = 0;
        CURLcode res = curl_easy_perform(curl);
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

        if (res != CURLE_OK) {
            cout << RED << "[X] CURL Error: " << curl_easy_strerror(res) << RESET << endl;
        } else if (http_code == 201 || http_code == 200 || http_code == 204) {
            cout << GREEN << "[OK] Data sent to cloud" << RESET << endl;
            success = true;
        } else {
            string msg = "Unknown error";
            size_t pos = response.find("\"message\":\"");
            if (pos != string::npos) {
                pos += 11;
                msg = response.substr(pos, response.find("\"", pos) - pos);
            }
            cout << RED << "[X] Error (" << http_code << "): " << msg << RESET << endl;
        }

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }
    return success;
}

// ================= COMMON PATCH =================
bool sendPatch(string url, string json) {
    CURL* curl = curl_easy_init();
    string response;
    bool success = false;

    if (curl) {
        struct curl_slist* headers = buildHeaders();

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PATCH");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        long http_code = 0;
        CURLcode res = curl_easy_perform(curl);
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

        if (res != CURLE_OK) {
            cout << RED << "[X] CURL Error: " << curl_easy_strerror(res) << RESET << endl;
        } else if (http_code == 200 || http_code == 204) {
            success = true;
        } else {
            string msg = "Unknown error";
            size_t pos = response.find("\"message\":\"");
            if (pos != string::npos) {
                pos += 11;
                msg = response.substr(pos, response.find("\"", pos) - pos);
            }
            cout << RED << "[X] Error (" << http_code << "): " << msg << RESET << endl;
        }

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }
    return success;
}

// ================= DATE HELPERS =================
string getDateInput(const string& prompt) {
    string date;
    while (true) {
        cout << prompt << " (YYYY-MM-DD): ";
        cin >> date;
        if (date.size() == 10 && date[4] == '-' && date[7] == '-')
            break;
        cout << RED << "[X] Invalid format. Use YYYY-MM-DD" << RESET << endl;
    }
    return date;
}

bool dateLeq(const string& a, const string& b) {
    return a <= b;
}

int daysBetween(const string& from, const string& to) {
    struct tm t1 = {}, t2 = {};
    sscanf(from.c_str(), "%d-%d-%d", &t1.tm_year, &t1.tm_mon, &t1.tm_mday);
    sscanf(to.c_str(),   "%d-%d-%d", &t2.tm_year, &t2.tm_mon, &t2.tm_mday);
    t1.tm_year -= 1900; t1.tm_mon--;
    t2.tm_year -= 1900; t2.tm_mon--;
    time_t tt1 = mktime(&t1);
    time_t tt2 = mktime(&t2);
    return (int)((tt2 - tt1) / 86400);
}

string today() {
    time_t now = time(0);
    struct tm* t = localtime(&now);
    char buf[11];
    strftime(buf, sizeof(buf), "%Y-%m-%d", t);
    return string(buf);
}

// ================= SANITIZE =================
string sanitize(const string& s) {
    string out;
    for (char c : s) {
        if (c == '"')  out += "\\\"";
        else if (c == '\\') out += "\\\\";
        else out += c;
    }
    return out;
}

// ================= CONFIRM =================
bool confirm(const string& msg) {
    cout << YELLOW << msg << " (y/n): " << RESET;
    char c; cin >> c; cin.ignore();
    return (c == 'y' || c == 'Y');
}

// ================= EXTRACT HELPERS =================
float extractFloat(const string& data, const string& key) {
    size_t pos = data.find("\"" + key + "\":");
    if (pos == string::npos) return 0;
    pos += key.size() + 3;
    size_t end = data.find_first_of(",}]", pos);
    try { return stof(data.substr(pos, end - pos)); }
    catch (...) { return 0; }
}

string extractString(const string& data, const string& key, size_t from = 0) {
    size_t pos = data.find("\"" + key + "\":\"", from);
    if (pos == string::npos) return "";
    pos += key.size() + 4;
    return data.substr(pos, data.find("\"", pos) - pos);
}

int extractInt(const string& data, const string& key, size_t from = 0) {
    size_t pos = data.find("\"" + key + "\":", from);
    if (pos == string::npos) return -1;
    pos += key.size() + 3;
    size_t end = data.find_first_of(",}]", pos);
    try { return stoi(data.substr(pos, end - pos)); }
    catch (...) { return -1; }
}

// ================= GET HELPERS =================
string getVehicleById(int vid) {
    return sendGet(BASE_URL + "/vehicles?vehicle_id=eq." + to_string(vid));
}

string getRentalById(int rid) {
    return sendGet(BASE_URL + "/rental?rental_id=eq." + to_string(rid));
}

// ================= UPDATE VEHICLE AVAILABILITY =================
void setVehicleAvailability(int vid, bool available) {
    string json = string("{\"is_available\":") + (available ? "true" : "false") + "}";
    sendPatch(BASE_URL + "/vehicles?vehicle_id=eq." + to_string(vid), json);
}

// ================================================
//                  FEATURES
// ================================================

// ================= 1. ADD CUSTOMER =================
void addCustomer() {
    int id;
    string name, phone, license;

    cout << CYAN << "\n--- ADD CUSTOMER ---" << RESET << endl;

    cout << "Customer ID: ";
    while (!(cin >> id)) { cin.clear(); cin.ignore(1000, '\n'); cout << "Invalid: "; }
    cin.ignore();

    cout << "Name: ";        getline(cin, name);
    cout << "Phone: ";       getline(cin, phone);
    cout << "License No: ";  getline(cin, license);

    if (!confirm("Add customer '" + name + "'?")) { cout << "Cancelled.\n"; return; }

    string json = "{";
    json += "\"customer_id\":" + to_string(id) + ",";
    json += "\"name\":\"" + sanitize(name) + "\",";
    json += "\"phone\":\"" + sanitize(phone) + "\",";
    json += "\"license_no\":\"" + sanitize(license) + "\"}";

    sendPost(BASE_URL + "/customer", json);
}

// ================= 2. VIEW ALL CUSTOMERS =================
void viewCustomers() {
    cout << CYAN << "\n--- ALL CUSTOMERS ---" << RESET << endl;
    string response = sendGet(BASE_URL + "/customer");

    if (response == "[]") { cout << "No customers found.\n"; return; }

    cout << BOLD;
    printf("%-8s %-20s %-15s %-15s\n", "ID", "Name", "Phone", "License");
    cout << string(60, '-') << "\n" << RESET;

    size_t pos = 0;
    while ((pos = response.find("\"customer_id\":", pos)) != string::npos) {
        int id         = extractInt(response, "customer_id", pos);
        string name    = extractString(response, "name", pos);
        string phone   = extractString(response, "phone", pos);
        string license = extractString(response, "license_no", pos);
        printf("%-8d %-20s %-15s %-15s\n", id, name.c_str(), phone.c_str(), license.c_str());
        pos++;
    }
    cout << string(60, '-') << "\n";
}

// ================= 3. ADD VEHICLE =================
void addVehicle() {
    int id;
    string brand, model;
    float rent;

    cout << CYAN << "\n--- ADD VEHICLE ---" << RESET << endl;

    cout << "Vehicle ID: ";
    while (!(cin >> id)) { cin.clear(); cin.ignore(1000, '\n'); cout << "Invalid: "; }

    if (getVehicleById(id) != "[]") {
        cout << RED << "[X] Vehicle ID already exists" << RESET << endl;
        cin.ignore(); return;
    }

    cin.ignore();
    cout << "Brand: ";  getline(cin, brand);
    cout << "Model: ";  getline(cin, model);
    cout << "Rent per day (Rs.): ";
    while (!(cin >> rent) || rent <= 0) { cin.clear(); cin.ignore(1000, '\n'); cout << "Invalid: "; }

    if (!confirm("Add vehicle " + brand + " " + model + "?")) {
        cin.ignore(); cout << "Cancelled.\n"; return;
    }

    string json = "{";
    json += "\"vehicle_id\":" + to_string(id) + ",";
    json += "\"brand\":\"" + sanitize(brand) + "\",";
    json += "\"model\":\"" + sanitize(model) + "\",";
    json += "\"rent_per_day\":" + to_string(rent) + ",";
    json += "\"is_available\":true}";

    sendPost(BASE_URL + "/vehicles", json);
    cin.ignore();
}

// ================= 4. VIEW VEHICLES =================
void getVehicles(bool availableOnly = false) {
    string url = BASE_URL + "/vehicles";
    if (availableOnly) url += "?is_available=eq.true";

    string response = sendGet(url);

    if (response == "[]") { cout << "No vehicles found.\n"; return; }

    cout << "\n" << BOLD;
    printf("%-6s %-15s %-15s %-10s %-15s\n", "ID", "Brand", "Model", "Rent/Day", "Status");
    cout << string(65, '-') << "\n" << RESET;

    size_t pos = 0;
    while ((pos = response.find("\"vehicle_id\":", pos)) != string::npos) {
        int id       = extractInt(response, "vehicle_id", pos);
        string brand = extractString(response, "brand", pos);
        string model = extractString(response, "model", pos);
        float rent   = extractFloat(response, "rent_per_day");
        size_t a     = response.find("\"is_available\":", pos);
        bool available = response.substr(a + 15, 5).find("true") != string::npos;

        printf("%-6d %-15s %-15s %-10.0f ", id, brand.c_str(), model.c_str(), rent);
        if (available)
            cout << GREEN << "Available" << RESET << "\n";
        else
            cout << RED << "Booked" << RESET << "\n";

        pos++;
    }
    cout << string(65, '-') << "\n";
}

// ================= 5. SEARCH VEHICLES =================
void searchVehicles() {
    cout << CYAN << "\n--- SEARCH VEHICLES ---" << RESET << endl;
    cin.ignore();
    cout << "Enter brand or model keyword: ";
    string keyword; getline(cin, keyword);

    string url = BASE_URL + "/vehicles?or=(brand.ilike.*" + keyword + "*,model.ilike.*" + keyword + "*)";
    string response = sendGet(url);

    if (response == "[]") {
        cout << "No vehicles found matching '" << keyword << "'.\n"; return;
    }

    cout << "\n" << BOLD;
    printf("%-6s %-15s %-15s %-10s %-15s\n", "ID", "Brand", "Model", "Rent/Day", "Status");
    cout << string(65, '-') << "\n" << RESET;

    size_t pos = 0;
    while ((pos = response.find("\"vehicle_id\":", pos)) != string::npos) {
        int id       = extractInt(response, "vehicle_id", pos);
        string brand = extractString(response, "brand", pos);
        string model = extractString(response, "model", pos);
        float rent   = extractFloat(response, "rent_per_day");
        size_t a     = response.find("\"is_available\":", pos);
        bool available = response.substr(a + 15, 5).find("true") != string::npos;

        printf("%-6d %-15s %-15s %-10.0f ", id, brand.c_str(), model.c_str(), rent);
        if (available)
            cout << GREEN << "Available" << RESET << "\n";
        else
            cout << RED << "Booked" << RESET << "\n";

        pos++;
    }
    cout << string(65, '-') << "\n";
}

// ================= 6. CREATE RENTAL =================
void createRental() {
    int rid, cid, vid;

    cout << CYAN << "\n--- CREATE RENTAL ---" << RESET << endl;

    cout << "Rental ID: ";
    while (!(cin >> rid)) { cin.clear(); cin.ignore(1000, '\n'); cout << "Invalid: "; }
    cout << "Customer ID: ";
    while (!(cin >> cid)) { cin.clear(); cin.ignore(1000, '\n'); cout << "Invalid: "; }
    cout << "Vehicle ID: ";
    while (!(cin >> vid)) { cin.clear(); cin.ignore(1000, '\n'); cout << "Invalid: "; }
    cin.ignore();

    string data = getVehicleById(vid);
    if (data == "[]") { cout << RED << "[X] Vehicle not found" << RESET << endl; return; }
    if (data.find("\"is_available\":false") != string::npos) {
        cout << RED << "[X] Vehicle is already booked" << RESET << endl; return;
    }

    float rent = extractFloat(data, "rent_per_day");

    string rentDate, returnDate;
    rentDate = getDateInput("Rent date");
    while (true) {
        returnDate = getDateInput("Return date");
        if (dateLeq(rentDate, returnDate)) break;
        cout << RED << "[X] Return date must be >= rent date" << RESET << endl;
    }

    int days   = daysBetween(rentDate, returnDate);
    float total = rent * max(days, 1);
    cout << YELLOW << "Total Cost = Rs." << total
         << " (" << days << " days @ Rs." << rent << "/day)" << RESET << endl;

    if (!confirm("Confirm rental?")) { cout << "Cancelled.\n"; return; }

    string json = "{";
    json += "\"rental_id\":" + to_string(rid) + ",";
    json += "\"customer_id\":" + to_string(cid) + ",";
    json += "\"vehicle_id\":" + to_string(vid) + ",";
    json += "\"rent_date\":\"" + rentDate + "\",";
    json += "\"return_date\":\"" + returnDate + "\",";
    json += "\"total_cost\":" + to_string(total) + ",";
    json += "\"status\":\"active\"}";

    if (sendPost(BASE_URL + "/rental", json)) {
        setVehicleAvailability(vid, false);
        cout << GREEN << "Rental created successfully!" << RESET << endl;
    }
}

// ================= 7. RETURN VEHICLE =================
void returnVehicle() {
    int rid;
    cout << CYAN << "\n--- RETURN VEHICLE ---" << RESET << endl;

    cout << "Rental ID: ";
    while (!(cin >> rid)) { cin.clear(); cin.ignore(1000, '\n'); cout << "Invalid: "; }
    cin.ignore();

    string data = getRentalById(rid);
    if (data == "[]") { cout << RED << "[X] Rental not found" << RESET << endl; return; }

    string status = extractString(data, "status");
    if (status == "returned")  { cout << RED << "[X] Already returned" << RESET << endl; return; }
    if (status == "cancelled") { cout << RED << "[X] Rental was cancelled" << RESET << endl; return; }

    int vid           = extractInt(data, "vehicle_id");
    string returnDate = extractString(data, "return_date");
    float totalCost   = extractFloat(data, "total_cost");
    float rentPerDay  = extractFloat(getVehicleById(vid), "rent_per_day");

    string actualReturn = getDateInput("Actual return date");
    float lateFee = 0;

    if (!dateLeq(actualReturn, returnDate)) {
        int lateDays = daysBetween(returnDate, actualReturn);
        lateFee = lateDays * rentPerDay * 1.5f;
        cout << RED << "WARNING: Late by " << lateDays
             << " days. Late fee: Rs." << lateFee << RESET << endl;
    }

    float grandTotal = totalCost + lateFee;
    cout << YELLOW << "Grand Total (inc. late fee): Rs." << grandTotal << RESET << endl;

    if (!confirm("Confirm return?")) { cout << "Cancelled.\n"; return; }

    string json = "{";
    json += "\"status\":\"returned\",";
    json += "\"actual_return_date\":\"" + actualReturn + "\",";
    json += "\"late_fee\":" + to_string(lateFee) + "}";

    if (sendPatch(BASE_URL + "/rental?rental_id=eq." + to_string(rid), json)) {
        setVehicleAvailability(vid, true);
        cout << GREEN << "[OK] Vehicle returned! Grand Total: Rs."
             << grandTotal << RESET << endl;
    }
}

// ================= 8. CANCEL RENTAL =================
void cancelRental() {
    int rid;
    cout << CYAN << "\n--- CANCEL RENTAL ---" << RESET << endl;

    cout << "Rental ID: ";
    while (!(cin >> rid)) { cin.clear(); cin.ignore(1000, '\n'); cout << "Invalid: "; }
    cin.ignore();

    string data = getRentalById(rid);
    if (data == "[]") { cout << RED << "[X] Rental not found" << RESET << endl; return; }

    string status = extractString(data, "status");
    if (status != "active") {
        cout << RED << "[X] Can only cancel active rentals (current: "
             << status << ")" << RESET << endl;
        return;
    }

    string rentDate = extractString(data, "rent_date");
    if (!dateLeq(today(), rentDate)) {
        cout << RED << "[X] Cannot cancel - rental period already started" << RESET << endl;
        return;
    }

    int vid = extractInt(data, "vehicle_id");

    if (!confirm("Cancel rental #" + to_string(rid) + "?")) { cout << "Cancelled.\n"; return; }

    string json = "{\"status\":\"cancelled\"}";
    if (sendPatch(BASE_URL + "/rental?rental_id=eq." + to_string(rid), json)) {
        setVehicleAvailability(vid, true);
        cout << GREEN << "[OK] Rental cancelled" << RESET << endl;
    }
}

// ================= 9. VIEW RENTAL HISTORY =================
void viewRentalHistory() {
    int cid;
    cout << CYAN << "\n--- RENTAL HISTORY ---" << RESET << endl;

    cout << "Customer ID: ";
    while (!(cin >> cid)) { cin.clear(); cin.ignore(1000, '\n'); cout << "Invalid: "; }
    cin.ignore();

    string response = sendGet(BASE_URL + "/rental?customer_id=eq."
                              + to_string(cid) + "&order=rent_date.desc");

    if (response == "[]") {
        cout << "No rentals found for customer " << cid << ".\n"; return;
    }

    cout << "\n" << BOLD;
    printf("%-8s %-8s %-12s %-12s %-10s %-10s %-12s\n",
           "R.ID", "V.ID", "Rent Date", "Return", "Total", "Late Fee", "Status");
    cout << string(75, '-') << "\n" << RESET;

    size_t pos = 0;
    while ((pos = response.find("\"rental_id\":", pos)) != string::npos) {
        int rid     = extractInt(response, "rental_id", pos);
        int vid     = extractInt(response, "vehicle_id", pos);
        string rd   = extractString(response, "rent_date", pos);
        string ret  = extractString(response, "return_date", pos);
        float total = extractFloat(response, "total_cost");
        float late  = extractFloat(response, "late_fee");
        string stat = extractString(response, "status", pos);

        printf("%-8d %-8d %-12s %-12s %-10.0f %-10.0f ",
               rid, vid, rd.c_str(), ret.c_str(), total, late);

        if (stat == "active")        cout << CYAN   << stat << RESET << "\n";
        else if (stat == "returned") cout << GREEN  << stat << RESET << "\n";
        else                         cout << RED    << stat << RESET << "\n";

        pos++;
    }
    cout << string(75, '-') << "\n";
}

// ================= 10. OVERDUE RENTALS =================
void viewOverdueRentals() {
    cout << CYAN << "\n--- OVERDUE RENTALS ---" << RESET << endl;

    string t   = today();
    string url = BASE_URL + "/rental?status=eq.active&return_date=lt." + t;
    string response = sendGet(url);

    if (response == "[]") {
        cout << GREEN << "[OK] No overdue rentals!" << RESET << endl; return;
    }

    cout << "\n" << BOLD;
    printf("%-8s %-10s %-10s %-12s %-12s %-10s\n",
           "R.ID", "Cust.ID", "Veh.ID", "Rent Date", "Due Date", "Days Late");
    cout << string(65, '-') << "\n" << RESET;

    size_t pos = 0;
    while ((pos = response.find("\"rental_id\":", pos)) != string::npos) {
        int rid    = extractInt(response, "rental_id", pos);
        int cid    = extractInt(response, "customer_id", pos);
        int vid    = extractInt(response, "vehicle_id", pos);
        string rd  = extractString(response, "rent_date", pos);
        string ret = extractString(response, "return_date", pos);
        int late   = daysBetween(ret, t);

        printf("%-8d %-10d %-10d %-12s %-12s ",
               rid, cid, vid, rd.c_str(), ret.c_str());
        cout << RED << late << " days" << RESET << "\n";
        pos++;
    }
    cout << string(65, '-') << "\n";
}

// ================= 11. MAKE PAYMENT =================
void makePayment() {
    int pid, rid;
    float amount;

    cout << CYAN << "\n--- MAKE PAYMENT ---" << RESET << endl;

    cout << "Payment ID: ";
    while (!(cin >> pid)) { cin.clear(); cin.ignore(1000, '\n'); cout << "Invalid: "; }
    cout << "Rental ID: ";
    while (!(cin >> rid)) { cin.clear(); cin.ignore(1000, '\n'); cout << "Invalid: "; }
    cout << "Amount (Rs.): ";
    while (!(cin >> amount) || amount <= 0) { cin.clear(); cin.ignore(1000, '\n'); cout << "Invalid: "; }
    cin.ignore();

    cout << "Payment Mode:\n";
    cout << "  1. Cash\n  2. Card\n  3. UPI\n";
    cout << "Choice: ";
    int modeChoice;
    while (!(cin >> modeChoice) || modeChoice < 1 || modeChoice > 3) {
        cin.clear(); cin.ignore(1000, '\n');
        cout << "Enter 1, 2 or 3: ";
    }
    cin.ignore();

    string modes[] = {"Cash", "Card", "UPI"};
    string mode = modes[modeChoice - 1];

    string payDate = getDateInput("Payment date");

    if (!confirm("Pay Rs." + to_string((int)amount) + " via " + mode + "?")) {
        cout << "Cancelled.\n"; return;
    }

    string json = "{";
    json += "\"payment_id\":" + to_string(pid) + ",";
    json += "\"rental_id\":" + to_string(rid) + ",";
    json += "\"amount\":" + to_string(amount) + ",";
    json += "\"payment_mode\":\"" + mode + "\",";
    json += "\"date_payment\":\"" + payDate + "\"}";

    sendPost(BASE_URL + "/payment", json);
}

// ================= 12. VIEW RENTAL DETAILS =================
void viewRentalDetails() {
    int rid;
    cout << CYAN << "\n--- RENTAL DETAILS ---" << RESET << endl;

    cout << "Rental ID: ";
    while (!(cin >> rid)) { cin.clear(); cin.ignore(1000, '\n'); cout << "Invalid: "; }
    cin.ignore();

    string data = getRentalById(rid);
    if (data == "[]") { cout << RED << "[X] Rental not found" << RESET << endl; return; }

    int cid     = extractInt(data, "customer_id");
    int vid     = extractInt(data, "vehicle_id");
    string rd   = extractString(data, "rent_date");
    string ret  = extractString(data, "return_date");
    string art  = extractString(data, "actual_return_date");
    float total = extractFloat(data, "total_cost");
    float late  = extractFloat(data, "late_fee");
    string stat = extractString(data, "status");

    string custData = sendGet(BASE_URL + "/customer?customer_id=eq." + to_string(cid));
    string vehData  = getVehicleById(vid);

    string custName  = extractString(custData, "name");
    string custPhone = extractString(custData, "phone");
    string brand     = extractString(vehData,  "brand");
    string model     = extractString(vehData,  "model");

    cout << "\n" << BOLD << "========== RENTAL #" << rid << " ==========" << RESET << "\n";
    cout << "Customer  : " << custName << " (ID: " << cid << ", Phone: " << custPhone << ")\n";
    cout << "Vehicle   : " << brand << " " << model << " (ID: " << vid << ")\n";
    cout << "Rent Date : " << rd  << "\n";
    cout << "Return    : " << ret << "\n";
    if (!art.empty())
        cout << "Actual Ret: " << art << "\n";
    cout << "Total Cost: Rs." << total << "\n";
    if (late > 0)
        cout << RED << "Late Fee  : Rs." << late << RESET << "\n";
    cout << "Grand Tot : Rs." << (total + late) << "\n";
    cout << "Status    : ";
    if      (stat == "active")   cout << CYAN  << stat << RESET << "\n";
    else if (stat == "returned") cout << GREEN << stat << RESET << "\n";
    else                         cout << RED   << stat << RESET << "\n";
    cout << BOLD << "=====================================" << RESET << "\n";
}

// ================================================
//                    MENUS
// ================================================

void customerMenu() {
    int choice;
    do {
        cout << BOLD << "\n--- CUSTOMER MENU ---" << RESET << "\n";
        cout << "1. Add Customer\n";
        cout << "2. View All Customers\n";
        cout << "0. Back\n";
        cout << "Choice: ";
        while (!(cin >> choice)) { cin.clear(); cin.ignore(1000, '\n'); }
        cin.ignore();

        switch(choice) {
            case 1: addCustomer(); break;
            case 2: viewCustomers(); break;
            case 0: break;
            default: cout << "Invalid\n";
        }
    } while (choice != 0);
}

void vehicleMenu() {
    int choice;
    do {
        cout << BOLD << "\n--- VEHICLE MENU ---" << RESET << "\n";
        cout << "1. Add Vehicle\n";
        cout << "2. View All Vehicles\n";
        cout << "3. View Available Only\n";
        cout << "4. Search by Brand/Model\n";
        cout << "0. Back\n";
        cout << "Choice: ";
        while (!(cin >> choice)) { cin.clear(); cin.ignore(1000, '\n'); }
        cin.ignore();

        switch(choice) {
            case 1: addVehicle(); break;
            case 2: getVehicles(false); break;
            case 3: getVehicles(true); break;
            case 4: searchVehicles(); break;
            case 0: break;
            default: cout << "Invalid\n";
        }
    } while (choice != 0);
}

void rentalMenu() {
    int choice;
    do {
        cout << BOLD << "\n--- RENTAL MENU ---" << RESET << "\n";
        cout << "1. Create Rental\n";
        cout << "2. Return Vehicle\n";
        cout << "3. Cancel Rental\n";
        cout << "4. View Rental Details\n";
        cout << "5. View Rental History (by customer)\n";
        cout << "6. View Overdue Rentals\n";
        cout << "0. Back\n";
        cout << "Choice: ";
        while (!(cin >> choice)) { cin.clear(); cin.ignore(1000, '\n'); }
        cin.ignore();

        switch(choice) {
            case 1: createRental(); break;
            case 2: returnVehicle(); break;
            case 3: cancelRental(); break;
            case 4: viewRentalDetails(); break;
            case 5: viewRentalHistory(); break;
            case 6: viewOverdueRentals(); break;
            case 0: break;
            default: cout << "Invalid\n";
        }
    } while (choice != 0);
}

// ================= MAIN =================
int main() {
    int choice;
    do {
        cout << BOLD << "\n====== VEHICLE RENTAL SYSTEM ======" << RESET << "\n";
        cout << "1. Customer Management\n";
        cout << "2. Vehicle Management\n";
        cout << "3. Rental Management\n";
        cout << "4. Make Payment\n";
        cout << "5. Exit\n";
        cout << "Choice: ";

        while (!(cin >> choice)) { cin.clear(); cin.ignore(1000, '\n'); cout << "Invalid: "; }
        cin.ignore();

        switch(choice) {
            case 1: customerMenu(); break;
            case 2: vehicleMenu(); break;
            case 3: rentalMenu(); break;
            case 4: makePayment(); break;
            case 5: cout << "Goodbye!\n"; break;
            default: cout << "Invalid\n";
        }
    } while (choice != 5);

    return 0;
}