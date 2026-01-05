# Address Book (C Project)

A simple **Address Book application written in C**.  
This project demonstrates structured programming in C, file handling, validation, and a basic text-based user interface.

---

## Features
- Add, view, search, edit, and delete contacts
- Input validation for contact details
- Persistent storage using files (CSV / text)
- Modular code structure using header (`.h`) and source (`.c`) files

---

## Screenshots

### Main Menu
![Main Menu](AddressBook/Screenshots/Main_Menu.png)

### Add Contact
![Add Contact](AddressBook/Screenshots/Add_Contact.png)

### List Contacts
![List Contacts](AddressBook/Screenshots/List_Contacts.png)

---

## Project Structure
```
AddressBook/
├── main.c              # Program entry point
├── contact.c/.h        # Contact structure and operations
├── file.c/.h           # File handling (read/write contacts)
├── validate.c/.h       # Input validation logic
├── populate.c          # Sample data population
├── ui.h                # User interface declarations
├── contacts.csv        # Stored contacts (CSV format)
├── contacts.txt        # Alternate text storage
├── screenshots/        # Application screenshots
│   ├── main_menu.png
│   ├── add_contact.png
│   └── list_contacts.png
└── a.out               # Compiled output (may vary by system)
```

---

## Requirements
- GCC compiler (Linux / macOS / Windows with MinGW)
- Basic knowledge of C programming

---

## How to Compile
Navigate to the `AddressBook` directory and run:

```bash
gcc *.c -o addressbook
```

---

## How to Run
```bash
./addressbook
```

**On Windows**
```bash
addressbook.exe
```

---

## Notes
- Existing contact data is stored in `contacts.csv`
- Delete `a.out` and recompile if moving between systems
- This project is intended for learning and academic practice

---

## Future Improvements
- Dynamic memory allocation for large contact lists
- Improved error handling
- Enhanced menu-driven UI
- Cross-platform build support

---

## Author
**Kishore Kasireddy**

---

Feel free to fork, modify, and improve this project.
