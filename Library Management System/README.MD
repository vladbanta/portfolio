# Library Management System

## 📌 Description
The Library Management System is a database project developed during the third year of study at the Faculty of Automation, Computers, and Electronics in Craiova. The project is designed to manage library operations such as book loans, member registrations, staff information, and payment processing.

---

## 🗂️ Database Structure
The database consists of the following tables:

- **tblBooks:** Stores information about books (ID, Title, Author, Genre, Year, Availability).
- **tblMembers:** Stores information about library members (ID, Name, Contact, Registration Date).
- **tblStaff:** Stores information about library staff (ID, Name, Job Title, Contact).
- **tblLoans:** Stores information about book loans (ID, Book ID, Member ID, Loan Date, Return Date).
- **tblPayments:** Stores information about fines and payments (ID, Loan ID, Amount, Payment Method, Status).

---

## 📂 Features
- **Database Design:** 
  - Created the **Entity-Relationship Diagram (ERD)** and **Relational Model**.
- **Schema Implementation:** 
  - Built the database with **5 main tables**, defining **primary and foreign keys**, and adding **constraints**.
- **Data Population:** 
  - Populated tables with a minimum of **10 records each**, totaling over **50 records**.
- **SQL Queries:** 
  - Implemented **10 complex SQL queries** including:
    - Listing available books.
    - Finding the most borrowed books.
    - Displaying overdue loans.
    - Summarizing payments.
    - Showing active loans.
- **Documentation:** 
  - Project documentation includes:
    - ERD and Relational Model diagrams.
    - SQL code for database creation and population.
    - SQL queries and their results.

---

## ⚙️ Technologies Used
- **Database Management System:** Microsoft SQL Server
- **Query Language:** SQL
- **Database Design:** ERD, Relational Model
- **Tools:** SQL Server Management Studio (SSMS)

