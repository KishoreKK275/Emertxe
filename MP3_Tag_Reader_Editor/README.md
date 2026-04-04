# 🎵 MP3 Tag Reader & Editor

A lightweight command-line tool written in **C** to **read** and **edit ID3v2 metadata tags** embedded inside `.mp3` files — no external libraries required.

---

## 📖 What are ID3 Tags?

Every MP3 file contains a hidden metadata block called an **ID3 tag**. This stores information like the song title, artist name, album, year, genre, and comments. Music players (like VLC, Spotify, or Windows Media Player) read these tags to display song information.

This tool lets you **view** and **modify** those tags directly from the terminal.

```
┌──────────────────────────────────────┐
│           MP3 File Layout            │
├──────────────┬───────────────────────┤
│  ID3v2 Header│  10 bytes             │
├──────────────┼───────────────────────┤
│  TIT2 Frame  │  Song Title           │
│  TPE1 Frame  │  Artist Name          │
│  TALB Frame  │  Album Name           │
│  TYER Frame  │  Year                 │
│  TCON Frame  │  Genre                │
│  COMM Frame  │  Comment              │
├──────────────┼───────────────────────┤
│  Audio Data  │  (MP3 sound bytes)    │
└──────────────┴───────────────────────┘
```

---

## 📁 Project Structure

```
Mp3TagReader/
│
├── main.c       # Entry point — argument parsing and routing
├── reader.c     # Reads and displays ID3v2 tag frames from an MP3
├── reader.h     # Reader function prototypes
├── editor.c     # Edits a specific ID3v2 tag frame in-place
├── editor.h     # Editor function prototypes
├── utils.c      # Shared utilities: TagData struct, error/help display
├── utils.h      # TagData struct definition and utility prototypes
│
└── sample.mp3   # Sample MP3 file for testing
```

---

## ⚙️ How It Works

### Reading Tags
1. Opens the MP3 file in binary mode.
2. Reads the **10-byte ID3v2 header** and verifies the `"ID3"` magic string.
3. Iterates through each **frame** (TIT2, TPE1, TALB, TYER, TCON, COMM), reading the 4-byte frame ID, 4-byte size, 2-byte flags, and data.
4. Stores all values in a `TagData` struct and prints them to the terminal.

### Editing Tags
1. Opens the original MP3 as source and creates a temporary file `Temp.mp3` as destination.
2. Copies the ID3 header, then scans frame by frame.
3. When the target frame is found, writes the **new text** with updated size bytes instead of the old data.
4. All other frames and the full audio data are copied unchanged.
5. Updates the header's total tag size to reflect the change.
6. Deletes the original file and renames `Temp.mp3` to the original filename.

---

## 🛠️ Requirements

- GCC compiler (or any C99-compatible compiler)
- Linux / macOS / Windows (with MinGW or WSL)
- A valid `.mp3` file with **ID3v2** tags

---

## 🚀 Getting Started

### 1. Clone the Repository

```bash
git clone https://github.com/KishoreKK275/Emertxe.git
cd MP3_Tag_Reader_Editor
```

### 2. Compile the Code

```bash
gcc main.c reader.c editor.c utils.c -o mp3tag
```

This produces an executable named `mp3tag`.

---

## 💻 Usage

### 📋 View Help Menu

```bash
./mp3tag --help
```

**Output:**
```
-----------------------HELP MENU-----------------------
1. -v --> To view MP3 File Contents
USAGE: ./mp3tag -v <mp3filename>
2. -e -> To edit MP3 File Contents
USAGE: ./mp3tag -e <options> <new_text> <mp3filename>
OPTIONS: 2.1 -t : To edit song Title
         2.2 -a : To edit song Artist Name
         2.3 -A : To edit Album Name
         2.4 -y : To edit Year of Release
         2.5 -n : To edit Genre
         2.6 -c : To edit Comment
```

---

### 👁️ View Tags — Read all metadata from an MP3

```bash
./mp3tag -v <file.mp3>
```

**Example:**
```bash
./mp3tag -v sample.mp3
```

**Output:**
```
------------------------------------------------------------

               SELECTED VIEW DETAILS

------------------------------------------------------------
         MP3 TAG READER AND EDITOR FOR ID3v2
------------------------------------------------------------
TITLE   : Sunny Sunny
ARTIST  : Yo Yo Honey Singh
ALBUM   : Cocktail
YEAR    : 2012
MUSIC   : Pop
COMMENT : Official Track
------------------------------------------------------------

---------------DETAILS DISPLAYED SUCCESSFULLY---------------
```

---

### ✏️ Edit Tags — Modify a specific metadata field

```bash
./mp3tag -e <option> <new_value> <file.mp3>
```

| Option | Field Edited | ID3 Frame |
|--------|-------------|-----------|
| `-t`   | Song Title   | `TIT2`    |
| `-a`   | Artist Name  | `TPE1`    |
| `-A`   | Album Name   | `TALB`    |
| `-y`   | Year         | `TYER`    |
| `-n`   | Genre        | `TCON`    |
| `-c`   | Comment      | `COMM`    |

**Examples:**

```bash
# Change the song title
./mp3tag -e -t "My New Title" sample.mp3

# Change the artist name
./mp3tag -e -a "New Artist" sample.mp3

# Change the album
./mp3tag -e -A "New Album" sample.mp3

# Change the year
./mp3tag -e -y "2025" sample.mp3

# Change the genre
./mp3tag -e -n "Rock" sample.mp3

# Change the comment
./mp3tag -e -c "Edited with mp3tag tool" sample.mp3
```

**Output on success:**
```
INFO: Tag edited successfully. File updated: sample.mp3
```

---

## 🧪 Quick Test with the Sample File

```bash
# Step 1 — Compile
gcc main.c reader.c editor.c utils.c -o mp3tag

# Step 2 — View original tags
./mp3tag -v sample.mp3

# Step 3 — Edit the title
./mp3tag -e -t "My Edited Song" sample.mp3

# Step 4 — Verify the change
./mp3tag -v sample.mp3
```

---

## ⚠️ Limitations

- Only supports **ID3v2** tags (not ID3v1 tags at the end of the file).
- Best compatibility with **ISO-8859-1 (ASCII)** encoded MP3 files. MP3 files with **UTF-16** encoded tags (common in files tagged by Windows tools) may display incorrectly.
- Only the first 6 standard frames (Title, Artist, Album, Year, Genre, Comment) are read during viewing.
- The tool edits the file **in-place** — the original is replaced. Always keep a backup before editing.

---

## 🔍 Error Messages

| Error | Meaning |
|---|---|
| `No arguments provided.` | Run with no flags — use `--help` for usage |
| `Invalid arguments.` | Wrong flag combination or argument count |
| `Invalid file` | File not found, not readable, or not a `.mp3` file |
| `No ID3 tag found.` | The MP3 file does not contain an ID3v2 header |
| `Invalid edit option.` | Edit flag other than `-t`, `-a`, `-A`, `-y`, `-n`, `-c` used |
| `New text cannot be empty.` | Empty string passed as new tag value |
| `Failed to read tag header.` | File is corrupted or truncated mid-read |

---

## 👤 Author

**Kishore**  
Date: February 2026

---

## 📄 License

This project is open source and available under the [MIT License](LICENSE).
