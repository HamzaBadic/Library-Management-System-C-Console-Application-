# Library-Management-System-Cpp-Console-Application-
A C++ program for managing books in a library with features like search, rent, return, and file persistence.

# 📚 Library Management System (C++ Console Application)

This is a simple console-based Library Management System written in C++.  
It allows users to manage books in a library by adding, searching, renting, and returning books.  
Books can be saved to and loaded from a file (`library.txt`) for persistence between runs.

---

## 🚀 Features

- Add new books (with optional auto-generated ISBN)
- Search books by:
  - Title
  - Type (Novel, Comedy, Science, Biography, Other)
- Rent and return books
- ISBN validation and generation
- Data persistence using `library.txt`

---

## 🧠 Book Types (Enum)

```cpp
enum bookType { NOVEL, COMEDY, SCIENCE, BIOGRAPHY, OTHER };
