# Personal Encryptor (C++)

![C++](https://img.shields.io/badge/language-C%2B%2B-blue)
![Build](https://img.shields.io/badge/build-makefile-green)
![Status](https://img.shields.io/badge/status-learning%20project-orange)

A lightweight **file encryption and decryption utility written in C++**.
This project focuses on understanding **modular C++ design, filesystem operations, and process-based task handling** while building a practical security-related tool.

---

# Purpose 

The purpose of this project is to gain hands-on experience with:

* Modular C++ project architecture
* File system traversal and file I/O
* Process/task management in C++
* Implementing a basic encryption–decryption workflow

---

---

# Project Workflow

The project follows a simple processing pipeline:

### 1. User Input

The user provides:

* A directory containing files
* An operation (`Encrypt` or `Decrypt`)

### 2. Directory Scanning

The program scans the given directory and identifies files that need processing.

### 3. Task Creation

Each file is converted into a **task** managed by the process module.

### 4. File Handling

Files are read using the file I/O module and passed to the encryption system.

### 5. Encryption / Decryption

The **Cryption module** performs the required transformation depending on the chosen operation.

### 6. Output

The processed data is written back to the filesystem.

---

# Project Structure

```
personal_encryptor
│
├── main.cpp
├── Makefile
│
├── src
│   └── app
│       ├── encryptDecrypt
│       │   ├── Cryption.cpp
│       │   └── Cryption.hpp
│       │
│       ├── fileHandling
│       │   ├── IO.cpp
│       │   └── IO.hpp
│       │
│       └── processes
│           ├── ProcessManagement.cpp
│           ├── ProcessManagement.hpp
│           └── Task.hpp
│
└── test
    └── sample files
```

The project is divided into separate modules:

| Module           | Purpose                                 |
| ---------------- | --------------------------------------- |
| `encryptDecrypt` | Handles encryption and decryption logic |
| `fileHandling`   | Manages file reading and writing        |
| `processes`      | Manages tasks and workflow execution    |


---

# Build Instructions

Clone the repository:

```bash
git clone git@github.com:SagnikSamanta445/personal_encryptor.git
cd personal_encryptor
```

Compile the project using the Makefile:

```bash
make
```

---

# Running the Program

Run the executable:

```bash
./encrypt_decrypt
```

The program will prompt for:

* Directory path
* Operation (`Encrypt` or `Decrypt`)

The selected files will then be processed automatically.

---

# Technologies Used

* **C++**
* **STL (Standard Template Library)**
* **Filesystem Library**
* **Makefile build system**

---

This project demonstrates how to design a **structured C++ application** that combines filesystem operations, task management, and modular architecture to implement a functional encryption utility.
