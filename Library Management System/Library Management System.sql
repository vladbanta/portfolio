CREATE DATABASE dbLibrary
ON 
PRIMARY ( NAME = LibraryDB,
          FILENAME = 'C:\\laboratorDB\\LibraryDB.mdf',
          SIZE = 10,
          MAXSIZE = 50,
          FILEGROWTH = 2)
LOG ON
( NAME = LibraryDB_log,
  FILENAME = 'C:\\laboratorDB\\LibraryDB_log.ldf',
  SIZE = 10,
  MAXSIZE = 25,
  FILEGROWTH = 2)
GO

USE dbLibrary
GO

CREATE TABLE dbo.tblStaff(
    IDStaff          CHAR(2) PRIMARY KEY CLUSTERED,
    FirstName        VARCHAR(20) NOT NULL,
    LastName         VARCHAR(20) NOT NULL,
    PhoneNumber      VARCHAR(10) NOT NULL,
    Email            VARCHAR(30),
    JobTitle         VARCHAR(20) NOT NULL
);
GO

CREATE TABLE dbo.tblBooks(
    IDBook           CHAR(3) PRIMARY KEY CLUSTERED,
    Title            VARCHAR(50) NOT NULL,
    Author           VARCHAR(30) NOT NULL,
    Genre            VARCHAR(20),
    YearPublished    INT,
    IsAvailable      BIT NOT NULL
);
GO

CREATE TABLE dbo.tblMembers(
    IDMember         CHAR(4) PRIMARY KEY CLUSTERED,
    FirstName        VARCHAR(40) NOT NULL,
    LastName         VARCHAR(40) NOT NULL,
    PhoneNumber      VARCHAR(10) NOT NULL,
    RegistrationDate DATETIME NOT NULL
);
GO

CREATE TABLE dbo.tblLoans(
    IDLoan           CHAR(4) PRIMARY KEY CLUSTERED,
    IDBook           CHAR(3) NOT NULL,
    IDMember         CHAR(4) NOT NULL,
    LoanDate         DATETIME NOT NULL,
    ReturnDate       DATETIME,

    FOREIGN KEY (IDBook) REFERENCES dbo.tblBooks(IDBook)
    ON DELETE CASCADE ON UPDATE CASCADE,
    FOREIGN KEY (IDMember) REFERENCES dbo.tblMembers(IDMember)
    ON DELETE CASCADE ON UPDATE CASCADE
);
GO

CREATE TABLE dbo.tblPayments(
    IDPayment        CHAR(5) PRIMARY KEY CLUSTERED,
    IDLoan           CHAR(4),
    PaymentDate      DATETIME,
    Amount           DECIMAL(8, 2),
    PaymentMethod    VARCHAR(15),
    PaymentStatus    VARCHAR(10),

    FOREIGN KEY (IDLoan) REFERENCES dbo.tblLoans(IDLoan)
    ON DELETE CASCADE ON UPDATE CASCADE
);
GO

-- Populating tblStaff
INSERT INTO dbo.tblStaff (IDStaff, FirstName, LastName, PhoneNumber, Email, JobTitle)
VALUES ('01', 'Ionescu', 'Maria', '0723456789', 'maria.ionescu@example.com', 'Librarian'),
       ('02', 'Popescu', 'Andrei', '0723987654', 'andrei.popescu@example.com', 'Manager'),
       ('03', 'Dumitrescu', 'Ana', '0723546789', 'ana.dumitrescu@example.com', 'Assistant'),
       ('04', 'Vlad', 'Ioana', '0724556789', 'ioana.vlad@example.com', 'Librarian'),
       ('05', 'Marin', 'George', '0725456789', 'george.marin@example.com', 'Technician'),
       ('06', 'Radulescu', 'Elena', '0726356789', 'elena.radulescu@example.com', 'Librarian'),
       ('07', 'Constantin', 'Mihai', '0727256789', 'mihai.constantin@example.com', 'Assistant'),
       ('08', 'Alexandrescu', 'Radu', '0728156789', 'radu.alexandrescu@example.com', 'Manager'),
       ('09', 'Georgescu', 'Silvia', '0729056789', 'silvia.georgescu@example.com', 'Technician'),
       ('10', 'Popa', 'Adriana', '0729956789', 'adriana.popa@example.com', 'Librarian');
GO

-- Populating tblBooks
INSERT INTO dbo.tblBooks (IDBook, Title, Author, Genre, YearPublished, IsAvailable)
VALUES ('001', 'Pride and Prejudice', 'Jane Austen', 'Romance', 1813, 1),
       ('002', '1984', 'George Orwell', 'Dystopian', 1949, 1),
       ('003', 'The Great Gatsby', 'F. Scott Fitzgerald', 'Classic', 1925, 0),
       ('004', 'To Kill a Mockingbird', 'Harper Lee', 'Classic', 1960, 1),
       ('005', 'The Hobbit', 'J.R.R. Tolkien', 'Fantasy', 1937, 1),
       ('006', 'War and Peace', 'Leo Tolstoy', 'Historical', 1869, 1),
       ('007', 'Moby Dick', 'Herman Melville', 'Adventure', 1851, 1),
       ('008', 'Crime and Punishment', 'Fyodor Dostoevsky', 'Philosophical', 1866, 1),
       ('009', 'Brave New World', 'Aldous Huxley', 'Dystopian', 1932, 1),
       ('010', 'Anna Karenina', 'Leo Tolstoy', 'Romance', 1878, 1);
GO

-- Populating tblMembers
INSERT INTO dbo.tblMembers (IDMember, FirstName, LastName, PhoneNumber, RegistrationDate)
VALUES ('0001', 'Vasile', 'Ion', '0721000000', '2023-01-01'),
       ('0002', 'Alexandru', 'Maria', '0722000000', '2023-03-15'),
       ('0003', 'Georgescu', 'Andrei', '0723000000', '2023-05-10'),
       ('0004', 'Mihai', 'Silvia', '0724000000', '2023-06-20'),
       ('0005', 'Ana', 'Popa', '0725000000', '2023-07-25'),
       ('0006', 'Cristian', 'Vlad', '0726000000', '2023-08-30'),
       ('0007', 'Elena', 'Constantin', '0727000000', '2023-09-10'),
       ('0008', 'Radu', 'Alexandrescu', '0728000000', '2023-10-05'),
       ('0009', 'George', 'Radulescu', '0729000000', '2023-11-15'),
       ('0010', 'Silvia', 'Marin', '0730000000', '2023-12-01');
GO

-- Populating tblLoans
INSERT INTO dbo.tblLoans (IDLoan, IDBook, IDMember, LoanDate, ReturnDate)
VALUES ('L001', '003', '0001', '2023-06-01', '2023-06-15'),
       ('L002', '001', '0002', '2023-07-10', NULL),
       ('L003', '005', '0003', '2023-07-20', '2023-08-05'),
       ('L004', '002', '0004', '2023-08-10', '2023-08-25'),
       ('L005', '004', '0005', '2023-09-01', '2023-09-15'),
       ('L006', '007', '0006', '2023-09-20', NULL),
       ('L007', '008', '0007', '2023-10-01', '2023-10-15'),
       ('L008', '006', '0008', '2023-10-10', NULL),
       ('L009', '009', '0009', '2023-11-01', '2023-11-20'),
       ('L010', '010', '0010', '2023-12-01', NULL);
GO


-- Populating tblPayments
INSERT INTO dbo.tblPayments (IDPayment, IDLoan, PaymentDate, Amount, PaymentMethod, PaymentStatus)
VALUES ('P001', 'L001', '2023-06-16', 10.00, 'Cash', 'Completed'),
       ('P002', 'L002', '2023-08-01', 5.00, 'Card', 'Pending'),
       ('P003', 'L003', '2023-08-06', 8.00, 'Cash', 'Completed'),
       ('P004', 'L004', '2023-08-26', 6.00, 'Card', 'Completed'),
       ('P005', 'L005', '2023-09-16', 7.00, 'Cash', 'Completed'),
       ('P006', 'L006', '2023-09-30', 9.00, 'Card', 'Pending'),
       ('P007', 'L007', '2023-10-16', 10.00, 'Cash', 'Completed'),
       ('P008', 'L008', '2023-10-25', 4.00, 'Card', 'Pending'),
       ('P009', 'L009', '2023-11-21', 3.00, 'Cash', 'Completed'),
       ('P010', 'L010', '2023-12-15', 12.00, 'Card', 'Pending');
GO

-- Queries
-- 1. List all available books
SELECT Title, Author, Genre, YearPublished 
FROM dbo.tblBooks
WHERE IsAvailable = 1;
GO

-- 2. List members with active loans
SELECT m.FirstName, m.LastName, l.LoanDate
FROM dbo.tblLoans l
JOIN dbo.tblMembers m ON l.IDMember = m.IDMember
WHERE l.ReturnDate IS NULL;
GO

-- 3. Show payments grouped by status
SELECT PaymentStatus, COUNT(*) AS TotalPayments, SUM(Amount) AS TotalAmount
FROM dbo.tblPayments
GROUP BY PaymentStatus;
GO

-- 4. Find the most borrowed book
SELECT b.Title, COUNT(l.IDLoan) AS LoanCount
FROM dbo.tblLoans l
JOIN dbo.tblBooks b ON l.IDBook = b.IDBook
GROUP BY b.Title
ORDER BY LoanCount DESC;
GO

-- 5. List loans by members, ordered by loan date
SELECT m.FirstName, m.LastName, b.Title, l.LoanDate
FROM dbo.tblLoans l
JOIN dbo.tblBooks b ON l.IDBook = b.IDBook
JOIN dbo.tblMembers m ON l.IDMember = m.IDMember
ORDER BY l.LoanDate DESC;
GO

-- 6. Show overdue loans
SELECT m.FirstName, m.LastName, b.Title, l.LoanDate, l.ReturnDate
FROM dbo.tblLoans l
JOIN dbo.tblBooks b ON l.IDBook = b.IDBook
JOIN dbo.tblMembers m ON l.IDMember = m.IDMember
WHERE l.ReturnDate IS NULL AND l.LoanDate < GETDATE() - 30;
GO

-- 7. Count total books by genre
SELECT Genre, COUNT(*) AS TotalBooks
FROM dbo.tblBooks
GROUP BY Genre;
GO

-- 8. Show total payments by member
SELECT m.FirstName, m.LastName, SUM(p.Amount) AS TotalPaid
FROM dbo.tblPayments p
JOIN dbo.tblLoans l ON p.IDLoan = l.IDLoan
JOIN dbo.tblMembers m ON l.IDMember = m.IDMember
GROUP BY m.FirstName, m.LastName;
GO

-- 9. List staff members ordered by job title
SELECT FirstName, LastName, JobTitle
FROM dbo.tblStaff
ORDER BY JobTitle;
GO

-- 10. Show books borrowed but not yet returned
SELECT b.Title, m.FirstName, m.LastName, l.LoanDate
FROM dbo.tblLoans l
JOIN dbo.tblBooks b ON l.IDBook = b.IDBook
JOIN dbo.tblMembers m ON l.IDMember = m.IDMember
WHERE l.ReturnDate IS NULL;
GO
