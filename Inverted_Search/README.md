# 🔍 Inverted Search Engine

A command-line **Inverted Index Search Engine** written in **C** that scans multiple `.txt` files, builds a searchable word database, and tells you exactly **which files contain a word and how many times** it appears — just like how a book index works.

---

## 📖 What is an Inverted Index?

A regular (forward) index maps: **file → list of words it contains**  
An inverted index maps: **word → list of files that contain it**

This is the exact same technique used by **Google**, **Elasticsearch**, and every modern search engine.

```
Forward Index:               Inverted Index:
f1.txt → [hi, hello, bye]    hi    → [f1.txt(3), f2.txt(1)]
f2.txt → [hi, hello, good]   hello → [f1.txt(3), f2.txt(1)]
f3.txt → [hi, hello, bye]    bye   → [f1.txt(2), f2.txt(1), f3.txt(1)]
                              good  → [f2.txt(2)]
```

With the inverted index, searching for `"hi"` instantly returns all files — no scanning needed.

---

## 📁 Project Structure

```
Inverted_Search/
│
├── main.c        # Entry point — argument parsing, menu loop, memory cleanup
├── inverted.h    # Struct definitions (F_node, M_node, S_node) and all prototypes
├── create.c      # Builds the inverted index from given text files
├── display.c     # Prints the full database as a formatted table
├── search.c      # Searches for a word and shows which files contain it
├── save.c        # Saves the in-memory database to a file
├── update.c      # Loads a saved database file back into memory
│
├── f1.txt        # Sample text file 1
├── f2.txt        # Sample text file 2
├── f3.txt        # Sample text file 3
└── database.txt  # Sample saved database (pre-generated)
```

---

## ⚙️ How It Works — Internal Design

### Data Structures

The database uses a **Hash Table of 27 linked lists**:
- Index `0–25` → words starting with `a–z` (case-insensitive)
- Index `26` → words starting with a non-alphabetic character

Each slot in the hash table holds a chain of **Main Nodes** (one per unique word).  
Each Main Node holds a chain of **Sub Nodes** (one per file that contains the word).

```
HT[7]  →  [M_node: "hi", files:2]  →  [M_node: "hello", files:2]  → NULL
                   |                              |
             [S_node: f1.txt, count:3]      [S_node: f1.txt, count:3]
                   |                              |
             [S_node: f2.txt, count:1]      [S_node: f2.txt, count:1]
                   |                              |
                  NULL                           NULL
```

### Struct Definitions

```c
// File linked list — holds the list of input file names
typedef struct file {
    char f_name[20];
    struct file *link;
} F_node;

// Main node — one per unique word in the database
typedef struct Main {
    char word[25];        // the word itself
    int file_count;       // how many files contain this word
    S_node *sub_link;     // points to first sub node
    struct Main *main_link;
} M_node;

// Sub node — one per file that contains the word
typedef struct Sub {
    int word_count;       // how many times the word appears in this file
    char filename[20];    // the file name
    struct Sub *sub_link;
} S_node;
```

---

## 🛠️ Requirements

- GCC compiler (or any C99-compatible compiler)
- Linux / macOS / Windows (with MinGW or WSL)
- One or more `.txt` files containing text

---

## 🚀 Getting Started

### 1. Clone the Repository

```bash
git clone https://github.com/YOUR_USERNAME/inverted-search-engine.git
cd inverted-search-engine
```

### 2. Compile the Code

```bash
gcc main.c create.c display.c search.c save.c update.c -o search
```

---

## 💻 Usage

```bash
./search <file1.txt> <file2.txt> <file3.txt> ...
```

You can pass **any number of `.txt` files**. The program will:
1. Validate each file (must exist, must be `.txt`, must not be empty).
2. Open an **interactive menu** where you choose what to do.

---

## 🖥️ Interactive Menu — All 6 Options Explained

```
Select your choice among following operations:
1. Create Database
2. Display Database
3. Save Database
4. Search
5. Update Database
6. Exit

Enter your choice :
```

---

### Option 1 — Create Database

Reads all the input `.txt` files you provided and builds the inverted index in memory.

```
Enter your choice : 1
Data Base Created Successfully!
```

> ⚠️ Can only be run **once per session**. If you try again it will say `"Database already created"`.  
> Must be done **before** Display, Save, or Search.

---

### Option 2 — Display Database

Prints the full inverted index as a formatted table.

```
Enter your choice : 2
-----------------------------------------------------------------
| Index | Word            | File count | Filename   | Word count |
-----------------------------------------------------------------
| 7     | hi              | 2          | f1.txt     | 3          |
|       |                 |            | f2.txt     | 1          |
-----------------------------------------------------------------
| 7     | hello           | 2          | f1.txt     | 3          |
|       |                 |            | f2.txt     | 1          |
-----------------------------------------------------------------
| 1     | bye             | 2          | f1.txt     | 2          |
|       |                 |            | f2.txt     | 1          |
-----------------------------------------------------------------
```

**Column meanings:**

| Column     | Meaning |
|------------|---------|
| Index      | Hash table bucket index (0=a, 1=b, ..., 25=z, 26=non-alpha) |
| Word       | The unique word found across all files |
| File count | Number of files that contain this word |
| Filename   | Name of a file containing the word |
| Word count | How many times the word appears in that specific file |

---

### Option 3 — Save Database

Saves the current in-memory database to a `.txt` file you name.

```
Enter your choice : 3
Enter the file name to save database: mydb.txt
Data Base Saved Successfully!
```

The saved file format looks like this:

```
#7;hi;2;f1.txt;3;f2.txt;1;#
#7;hello;2;f1.txt;3;f2.txt;1;#
#1;bye;2;f1.txt;2;f2.txt;1;#
#6;good;1;f2.txt;2;#
```

**Format breakdown:**
```
#<index>;<word>;<file_count>;<filename1>;<word_count1>;<filename2>;<word_count2>;#
```

Each record starts with `#` and ends with `#\n`. This file can later be loaded back using Option 5.

---

### Option 4 — Search 🔍

**This is the main feature.** Enter any word to find which files contain it and how many times.

```
Enter your choice : 4
Enter the word to search : hello
Word found in 2 files
f1.txt : 3
f2.txt : 1
```

```
Enter your choice : 4
Enter the word to search : kishore
Word found in 1 files
f2.txt : 1
```

```
Enter your choice : 4
Enter the word to search : python
Word not found
```

**How the search works internally:**
1. Takes your input word.
2. Computes the hash index from the first character (e.g., `hello` → index `7` for `h`).
3. Goes directly to `HT[7]` — no scanning of other buckets.
4. Traverses only that bucket's linked list comparing word by word.
5. When found, prints the file count and each sub-node's filename + count.

> **Search is case-sensitive.** Searching `Hello` and `hello` may give different results depending on how the words appear in your files.

---

### Option 5 — Update Database

Loads a previously saved database file (from Option 3) back into memory — without re-reading the original `.txt` files.

```
Enter your choice : 5
Enter file name: mydb.txt
Update Done Successfully!
```

**Important rules for Update:**
- Only works if **Create (Option 1) has NOT been run** in this session.
- Can only be run **once per session**.
- The file must start with `#` — the program checks this to validate it's a proper saved database.
- The file must be `.txt`, must exist, and must not be empty.

**Why use Update instead of Create?**  
If you have already saved a database from a previous session, you can restore it instantly using Update instead of re-scanning all your text files from scratch.

---

### Option 6 — Exit

Frees all allocated memory (all M_nodes and S_nodes) and exits cleanly.

```
Enter your choice : 6
Exiting...
```

---

## 🧪 Full Walkthrough with Sample Files

### Step 1 — Check the sample files
```bash
cat f1.txt
# hi hello
# bye
# hi hi
# hello hello
# bye

cat f2.txt
# hai hi
# good morning
# hello
# good bye
# kishore kk

cat f3.txt
# hi hello bye hai goodnight
```

### Step 2 — Compile and run
```bash
gcc main.c create.c display.c search.c save.c update.c -o search
./search f1.txt f2.txt f3.txt
```

### Step 3 — Create the database (Option 1)
```
Enter your choice : 1
Data Base Created Successfully!
```

### Step 4 — Display it (Option 2)
```
Enter your choice : 2
(table showing all words, files, and counts)
```

### Step 5 — Search for words (Option 4)
```
Enter your choice : 4
Enter the word to search : hi
Word found in 3 files
f1.txt : 3
f2.txt : 1
f3.txt : 1
```

### Step 6 — Save the database (Option 3)
```
Enter your choice : 3
Enter the file name to save database: mydb.txt
Data Base Saved Successfully!
```

### Step 7 — Exit and reload in a new session
```
Enter your choice : 6
Exiting...

# In a new session — no need to pass text files again:
./search dummy.txt
Enter your choice : 5
Enter file name: mydb.txt
Update Done Successfully!

# Now search works from the saved data
Enter your choice : 4
Enter the word to search : hello
Word found in 2 files
...
```

---

## ⚠️ Limitations

- Word buffer is limited to **30 characters** — longer words will be truncated.
- Filename buffer is limited to **20 characters**.
- Search is **case-sensitive** (`Hi` ≠ `hi`).
- Punctuation attached to words (e.g., `hello,` or `hello.`) is treated as part of the word.
- **Create and Update are mutually exclusive** in a single session — you can only run one per execution.

---

## 🔍 Error Messages

| Message | Meaning |
|---------|---------|
| `Insufficient arguments` | No `.txt` files passed at startup |
| `Database already created` | Option 1 selected twice |
| `Database not created` | Options 2/3/4 selected before Option 1 or 5 |
| `Cannot update after creation` | Option 5 selected after Option 1 already ran |
| `Database already updated` | Option 5 selected twice |
| `Word not found` | Searched word does not exist in any indexed file |
| `File not available` | File passed to Update does not exist |
| `Invalid DB file` | File passed to Update was not saved by this program |
| `INFO : File is empty` | File passed to Update is empty |

---

## 👤 Author

**Kishore**  
Date: March 2026

---

## 📄 License

This project is open source and available under the [MIT License](LICENSE).
