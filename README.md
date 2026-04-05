# 🚗 Vehicle Rental System

### MIT World Peace University | OOP Project | Semester 4

---

## 👨‍💻 Team Members

| Sr.No. | Name              | PRN        |
| ------ | ----------------- | ---------- |
| 1      | **Anshu Singh**   | 1262243516 |
| 2      | Naina K. Sundrani | 1262252355 |
| 3      | Navya Shukla      | 1262243548 |
| 4      | Tanushree Walunj  | 1262243564 |

---

## 📌 Project Overview

This project is a **Vehicle Rental System** developed using **C++ (OOP)** and integrated with a **cloud database (Supabase)** using REST APIs.

The system automates vehicle rental operations including customer management, vehicle booking, payment processing, and real-time cloud storage.

---

## 🚀 Features

* Add Customer
* Add Vehicle
* Create Rental (Auto rent calculation)
* Make Payment
* Show Vehicles (Cloud data in tabular format)
* Prevent Double Booking
* Update Vehicle Availability
* Return Vehicle (Re-availability)

---

## ⚙️ Technologies Used

* C++ (Object-Oriented Programming)
* libcurl (HTTP requests)
* Supabase (PostgreSQL Cloud Database)
* REST API

---

## 🌐 System Architecture

```
C++ Program
   ↓
REST API (libcurl)
   ↓
Supabase (Cloud Database)
```

---

## 🧠 Key Concepts Used

* Object-Oriented Programming (OOP)
* REST API Integration
* Cloud Database Handling
* JSON Parsing
* Data Validation & Consistency

---

## 🗄️ Database Tables

* Customer
* Vehicles
* Rental
* Payment

---

## 🔧 How to Run

### 1. Open MinGW64 Terminal

### 2. Navigate to project

```
cd /c/Users/anshu/Desktop/oop
```

### 3. Compile

```
g++ oop_pro.cpp -o oop_pro -lcurl
```

### 4. Run

```
./oop_pro
```

---

## 🔑 Important Setup

* Replace `API_KEY` in the code with your Supabase anon key
* Ensure all tables are created in Supabase
* Disable Row Level Security (RLS)

---

## 🎯 Sample Output

```
=========== VEHICLE LIST ===========
ID    Brand     Model    Rent    Status
101   Mahindra  Thar     2000    Not Available
===================================
```

---

## 🧠 Viva Points

* Why cloud database? → Scalability & persistence
* Why REST API? → Connect C++ with backend
* Why libcurl? → HTTP requests in C++
* How booking works? → Fetch → Calculate → Update

---

## ⚠️ Limitations

* Basic JSON parsing
* Console-based interface
* No authentication system

---

## 🔮 Future Enhancements

* GUI (Qt / Web Interface)
* Advanced JSON parsing library
* User authentication system
* Search & filter features
* Full CRUD operations

---

## 📌 Conclusion

This project demonstrates a **real-world cloud-integrated system** combining C++ with modern backend technologies, ensuring scalability, efficiency, and maintainability.

---

## 🔗 GitHub Repository

https://github.com/anshusingh226-tech/vehicle-rental-system
