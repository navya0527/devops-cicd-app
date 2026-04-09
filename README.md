# Vehicle Rental System

### MIT World Peace University | OOP Project | Semester 4

---

## Team Members

| Sr.No. | Name              | PRN        |
| ------ | ----------------- | ---------- |
| 1      | **Anshu Singh**   | 1262243516 |
| 2      | Naina K. Sundrani | 1262252355 |
| 3      | Navya Shukla      | 1262243548 |
| 4      | Tanushree Walunj  | 1262243564 |

---

## Project Overview

This project is a **Vehicle Rental System** developed using **C++ (OOP)** and integrated with a **cloud database (Supabase)** using REST APIs.

The system automates vehicle rental operations including customer management, vehicle booking, return processing, payment handling, and real-time cloud storage.

---

## Features

**Customer Management**
- Add Customer
- View All Customers

**Vehicle Management**
- Add Vehicle (with duplicate ID check)
- View All Vehicles (tabular, color-coded status)
- View Available Vehicles Only
- Search Vehicles by Brand or Model

**Rental Management**
- Create Rental (auto cost calculation based on days)
- Return Vehicle (with late fee at 1.5x rate)
- Cancel Rental (only before rental start date)
- View Rental Details (customer + vehicle + cost summary)
- View Rental History by Customer
- View Overdue Rentals Report

**Payment**
- Make Payment (Cash / Card / UPI)

**System**
- Confirm prompts before every write operation
- Input validation with proper error handling
- JSON sanitization (prevents injection on special characters)
- ANSI color-coded terminal output
- Prevent double booking

---

## Technologies Used

- C++ (Object-Oriented Programming)
- libcurl (HTTP requests)
- Supabase (PostgreSQL Cloud Database)
- REST API (GET, POST, PATCH)

---

## System Architecture

    C++ Program
       |
    REST API (libcurl)  --  GET / POST / PATCH
       |
    Supabase (PostgreSQL Cloud Database)

---

## Key Concepts Used

- Object-Oriented Programming (OOP)
- REST API Integration (GET, POST, PATCH)
- Cloud Database Handling
- Manual JSON Parsing & Building
- Input Validation & Stream Error Recovery
- Date Arithmetic (late fee calculation)
- JSON Injection Sanitization

---

## Database Tables

| Table    | Key Columns                                                                                                   |
|----------|---------------------------------------------------------------------------------------------------------------|
| customer | customer_id, name, phone, license_no                                                                          |
| vehicles | vehicle_id, brand, model, rent_per_day, is_available                                                         |
| rental   | rental_id, customer_id, vehicle_id, rent_date, return_date, actual_return_date, total_cost, late_fee, status |
| payment  | payment_id, rental_id, amount, payment_mode, date_payment                                                    |

---

## Schema Setup (Run in Supabase SQL Editor)

    ALTER TABLE rental
    ADD CONSTRAINT valid_dates CHECK (return_date >= rent_date);

    ALTER TABLE rental
    ADD COLUMN status VARCHAR(20) DEFAULT 'active'
    CHECK (status IN ('active', 'returned', 'cancelled'));

    ALTER TABLE rental
    ADD COLUMN actual_return_date DATE;

    ALTER TABLE rental
    ADD COLUMN late_fee NUMERIC DEFAULT 0;

    ALTER TABLE payment
    ADD CONSTRAINT payment_mode_check
    CHECK (payment_mode IN ('Cash', 'Card', 'UPI'));

---

## How to Run

### 1. Open MinGW64 Terminal

### 2. Navigate to project

    cd /c/Users/anshu/Desktop/oop

### 3. Install libcurl (if not already installed)

    pacman -S mingw-w64-x86_64-curl

### 4. Compile

    g++ oop_pro.cpp -o oop_pro -lcurl

### 5. Run

    ./oop_pro

---

## Important Setup

- Replace `API_KEY` in the code with your Supabase anon key
- Replace `BASE_URL` with your Supabase project URL
- Run all schema SQL above in Supabase SQL Editor
- Disable Row Level Security (RLS) on all tables

---

## Sample Output

    ====== VEHICLE RENTAL SYSTEM ======
    1. Customer Management
    2. Vehicle Management
    3. Rental Management
    4. Make Payment
    5. Exit

    --- VEHICLE MENU ---
    ID     Brand           Model           Rent/Day   Status
    -----------------------------------------------------------------
    101    Mahindra        Thar            2000       Available
    108    Maruti          Swift           2400       Booked

    --- RENTAL DETAILS ---
    ========== RENTAL #5 ==========
    Customer  : Anshu Singh (ID: 1234, Phone: 9644504950)
    Vehicle   : Mahindra Thar (ID: 101)
    Rent Date : 2026-04-09
    Return    : 2026-04-12
    Total Cost: Rs.6000
    Status    : active
    =====================================

---

## Viva Points

| Question | Answer |
|----------|--------|
| Why cloud database? | Scalability, persistence, accessible from anywhere |
| Why REST API? | Connects C++ with Supabase backend over HTTP |
| Why libcurl? | Industry-standard HTTP client library for C++ |
| How is booking prevented? | Fetch vehicle -> check is_available -> reject if false |
| How is late fee calculated? | (actual_return - return_date) * rent_per_day * 1.5 |
| How is JSON built? | Manual string construction with sanitization |
| What does PATCH do? | Partially updates a record (availability, status, late_fee) |
| Why sanitize input? | Prevents JSON injection from special characters like " and \ |

---

## Limitations

- Manual JSON parsing (no library like nlohmann/json)
- Console-based interface only
- No user authentication or login system
- Dates entered manually (no calendar picker)
- No pagination for large datasets

---

## Future Enhancements

- GUI using Qt or a Web Interface
- JSON parsing library (nlohmann/json)
- User authentication and login
- PDF receipt generation on payment
- SMS/email notification on booking
- Advanced search and filter options

---

## Conclusion

This project demonstrates a **real-world cloud-integrated system** combining C++ with modern backend technologies (Supabase + REST API), implementing core OOP principles with practical features like late fee calculation, rental lifecycle management, and input-safe cloud operations.

---

## GitHub Repository

https://github.com/anshusingh226-tech/vehicle-rental-system