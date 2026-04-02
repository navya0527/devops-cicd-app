#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class Vehicle {
public:
    int vehicleID;
    string model;
    bool isAvailable;
    float rentPerDay;

    void addVehicle(int id, string m, float rent) {
        vehicleID = id;
        model = m;
        rentPerDay = rent;
        isAvailable = true;
    }

    void displayVehicle() {
        cout << "\nID: " << vehicleID
             << "\nModel: " << model
             << "\nRent/day: " << rentPerDay
             << "\nStatus: " << (isAvailable ? "Available" : "Not Available") << endl;
    }
};

class Customer {
public:
    int customerID;
    string name;

    void addCustomer(int id, string n) {
        customerID = id;
        name = n;
    }
};

class Booking {
public:
    int bookingID;
    int vehicleID;
    int customerID;
    int days;
    float totalAmount;
    bool isPaid;

    void createBooking(int bID, Vehicle &v, Customer &c, int d) {
        if (v.isAvailable) {
            bookingID = bID;
            vehicleID = v.vehicleID;
            customerID = c.customerID;
            days = d;

            totalAmount = days * v.rentPerDay;
            isPaid = false;

            v.isAvailable = false;

            generateBill(v, c);
        } else {
            cout << "\nVehicle not available!\n";
        }
    }

    // 🔥 FULL PAYMENT ONLY
    void makePayment() {
        if (totalAmount == 0) {
            cout << "No booking found!\n";
            return;
        }

        float amount;

        while (!isPaid) {
            cout << "\nTotal Amount to Pay: " << totalAmount << endl;
            cout << "Enter FULL Payment Amount: ";
            cin >> amount;

            if (amount == totalAmount) {
                isPaid = true;
                cout << "Payment successful!\n";
            } else {
                cout << "Invalid amount! Please enter exact amount.\n";
            }
        }
    }

    void generateBill(Vehicle v, Customer c) {
        cout << "\n===== BILL =====\n";
        cout << "Booking ID: " << bookingID << endl;
        cout << "Customer: " << c.name << endl;
        cout << "Vehicle: " << v.model << endl;
        cout << "Days: " << days << endl;
        cout << "Total: " << totalAmount << endl;
        cout << "Payment Status: " << (isPaid ? "Paid" : "Pending") << endl;
        cout << "================\n";
    }
};

// 🔐 Admin Login
bool login() {
    string username, password;
    cout << "Enter Username: ";
    cin >> username;
    cout << "Enter Password: ";
    cin >> password;

    if (username == "admin" && password == "1234") {
        cout << "Login Successful!\n";
        return true;
    } else {
        cout << "Invalid Login!\n";
        return false;
    }
}

// ➕ Add Vehicle
void addNewVehicle(Vehicle v[], int &n) {
    int id;
    string model;
    float rent;

    cout << "Enter Vehicle ID: ";
    cin >> id;
    cout << "Enter Model: ";
    cin >> model;
    cout << "Enter Rent per day: ";
    cin >> rent;

    v[n].addVehicle(id, model, rent);
    n++;

    cout << "Vehicle added successfully!\n";
}

// 📋 Display Vehicles
void displayVehicles(Vehicle v[], int n) {
    for (int i = 0; i < n; i++) {
        v[i].displayVehicle();
    }
}

// 🔍 Search Vehicle
void searchVehicle(Vehicle v[], int n) {
    int id;
    cout << "Enter Vehicle ID: ";
    cin >> id;

    for (int i = 0; i < n; i++) {
        if (v[i].vehicleID == id) {
            v[i].displayVehicle();
            return;
        }
    }
    cout << "Vehicle not found!\n";
}

// 🔄 Return Vehicle
void returnVehicle(Vehicle v[], int n) {
    int id;
    cout << "Enter Vehicle ID: ";
    cin >> id;

    for (int i = 0; i < n; i++) {
        if (v[i].vehicleID == id) {
            v[i].isAvailable = true;
            cout << "Vehicle returned!\n";
            return;
        }
    }
}

// 💾 Save to file
void saveToFile(Vehicle v[], int n) {
    ofstream file("vehicles.txt");

    for (int i = 0; i < n; i++) {
        file << v[i].vehicleID << " "
             << v[i].model << " "
             << v[i].rentPerDay << " "
             << v[i].isAvailable << endl;
    }

    file.close();
    cout << "Data saved!\n";
}

// 📂 Load from file
void loadFromFile(Vehicle v[], int &n) {
    ifstream file("vehicles.txt");

    n = 0;
    while (file >> v[n].vehicleID >> v[n].model >> v[n].rentPerDay >> v[n].isAvailable) {
        n++;
    }

    file.close();
    cout << "Data loaded!\n";
}

int main() {
    if (!login()) return 0;

    Vehicle v[50];
    Customer c[5];
    Booking b;

    int vCount = 0;

    c[0].addCustomer(1, "Anshu");

    int choice;

    do {
        cout << "\n===== MENU =====\n";
        cout << "1. Add Vehicle\n";
        cout << "2. Display Vehicles\n";
        cout << "3. Search Vehicle\n";
        cout << "4. Book Vehicle\n";
        cout << "5. Return Vehicle\n";
        cout << "6. Make Payment\n";
        cout << "7. Save Data\n";
        cout << "8. Load Data\n";
        cout << "9. Exit\n";

        cin >> choice;

        switch (choice) {

        case 1:
            addNewVehicle(v, vCount);
            break;

        case 2:
            displayVehicles(v, vCount);
            break;

        case 3:
            searchVehicle(v, vCount);
            break;

        case 4: {
            int vid, days;
            cout << "Enter Vehicle ID: ";
            cin >> vid;
            cout << "Enter Days: ";
            cin >> days;

            for (int i = 0; i < vCount; i++) {
                if (v[i].vehicleID == vid) {
                    b.createBooking(1001, v[i], c[0], days);
                }
            }
            break;
        }

        case 5:
            returnVehicle(v, vCount);
            break;

        case 6:
            b.makePayment();
            break;

        case 7:
            saveToFile(v, vCount);
            break;

        case 8:
            loadFromFile(v, vCount);
            break;

        }

    } while (choice != 9);

    return 0;
}