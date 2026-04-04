#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "reader.h"
#include "utils.h"
//Function to read tag
char *read_tag(char *tag_name,FILE *file) {
    if (fread(tag_name, 1, 4, file) != 4) {
        display_error("Failed to read tag header.");
        fclose(file);
        return NULL;
    }
    tag_name[4] = '\0'; 
    return tag_name;
}
//Function to read size and convert to int
int read_size_and_convert(FILE *file) {
    unsigned char size_bytes[4];//for storing size as string in buffer before converting to size
    if (fread(size_bytes, 1, 4, file)!= 4) {
        display_error("Failed to read tag header size .");
        fclose(file);
        return -1;
    }
    //converting char into int size
    int size = (size_bytes[0] << 24) |(size_bytes[1] << 16) |(size_bytes[2] << 8)  | size_bytes[3];
    return size;
}
//Function to read tag data
char *read_data(FILE *file,int size) {
    char *buf = malloc(size);
    if(!buf) return NULL;
    if (fread(buf, 1, size, file) != size) {
        display_error("Failed to read tag header.");
        fclose(file); free(buf);
        return NULL;
    }
    buf[size] = '\0';
    return buf;
}
TagData* read_id3_tags(const char *filename) {
    FILE *file = fopen(filename, "rb");
    char header[10];//Temp buffer to store header 
    if (fread(header, 1, 10, file) != 10) {
        fclose(file);
        display_error("Failed to read ID3 header.");
        return NULL;
    } 
    if (strncmp(header, "ID3", 3) != 0) {
        fclose(file);
        display_error("No ID3 tag found.");
        return NULL;
    }
    //Memory initialized for Tag data structure
    TagData *data = create_tag_data();
    for(int i=0;i<6;i++) {
        char tag[5]; // Temp buffer to store 4 bytes of tag data in 5 bytes of char array
        if (!read_tag(tag, file)) break;
        int size = read_size_and_convert(file);
        if (size <= 0) break;
        fseek(file, 2, SEEK_CUR); // Skip 2 frame flags 
        char *value = read_data(file, size);
        if (!value) break;
        char *text = value+1; // Add 1 byte for null (text encoding byte)
        if (strcmp(tag, "TIT2") == 0)       data->title = strdup(text);
        else if (strcmp(tag, "TPE1") == 0)  data->artist = strdup(text);
        else if (strcmp(tag, "TALB") == 0)  data->album = strdup(text);
        else if (strcmp(tag, "TYER") == 0)  data->year = strdup(text);
        else if (strcmp(tag, "TCON") == 0)  data->genre = strdup(text);
        else if (strcmp(tag, "COMM") == 0)  data->comment = strdup(text);
        free(value);
    }
    fclose(file);   return data;
}
void display_metadata(const TagData *data) {
    printf("------------------------------------------------------------\n\n");
    printf("               SELECTED VIEW DETAILS\n\n");
    printf("------------------------------------------------------------\n");
    printf("         MP3 TAG READER AND EDITOR FOR ID3v2\n");
    printf("------------------------------------------------------------\n");
    printf("TITLE   : %s\n", data->title   ? data->title   : "N/A");
    printf("ARTIST  : %s\n", data->artist  ? data->artist  : "N/A");
    printf("ALBUM   : %s\n", data->album   ? data->album   : "N/A");
    printf("YEAR    : %s\n", data->year    ? data->year    : "N/A");
    printf("MUSIC   : %s\n", data->genre   ? data->genre   : "N/A");
    printf("COMMENT : %s\n", data->comment ? data->comment : "N/A");
    printf("------------------------------------------------------------\n\n");
    printf("---------------DETAILS DISPLAYED SUCCESSFULLY---------------\n");
}
void view_tags(const char *filename) {
    TagData *data = read_id3_tags(filename);
    if (!data) {
        display_error("Failed to read ID3 tags.");
        return;
    }
    display_metadata(data);
    free_tag_data(data);
}
/*
Great, this is a **classic ID3 bug** and your hex dumps make the reason *very clear*.
Let’s walk through it **slowly, byte-by-byte**, then I’ll tell you **exactly why one file works, the other fails**, and **how to make your code compatible with both**.

---

## 1️⃣ What is different between the two MP3 files?

### ✅ `sample.mp3` (WORKS)

Look at `TIT2` frame data:

```
54 49 54 32 00 00 00 2f 00 00
00 53 75 6e 6e 79 20 53 75 6e 6e 79 ...
^^
encoding = 0x00
```

📌 **Encoding byte = `0x00` → ISO-8859-1 / ASCII**

So text is:

```
Sunny Sunny - Yo Yo Honey Singh ...
```

Your code does:

```c
char *text = value + 1;
printf("%s", text);
```

✔ Works perfectly.

---

### ❌ `Daavudi.mp3` (FAILS)

Now look at the same frame:

```
01 ff fe 44 00 61 00 61 00 76 00 75 00 64 00 69 00
^^
encoding = 0x01
```

📌 **Encoding byte = `0x01` → UTF-16 with BOM**

Then:

```
ff fe = UTF-16 LE BOM
44 00 = 'D'
61 00 = 'a'
61 00 = 'a'
...
```

So the text is actually:

```
Daavudi (Kosh...)   ← UTF-16LE
```

---

## 2️⃣ Why your code prints `��D`, `��N`, etc.

Your code **assumes text is 1-byte per character**:

```c
char *text = value + 1;
printf("%s", text);
```

But in UTF-16:

```
D = 0x44 0x00
```

`printf("%s")` stops at the **first `0x00`**, so output becomes:

```
��D
```

That’s why:

```
TITLE  : ��D
ARTIST : ��N
```

Your parser is **not decoding UTF-16 at all**.

---

## 3️⃣ Why COMM shows only `eng`

COMM format:

```
[encoding][language(3)][description][0][comment]
```

In `Daavudi.mp3`:

```
01 | 65 6e 67 | 00 00 | ff fe 44 00 6f 00 77 00 ...
```

You printed:

```c
Tag data: eng
```

Because:

* You skipped only encoding byte
* Printed language field (`eng`)
* Never parsed UTF-16 comment text

---

## 4️⃣ Summary of WHY it fails

| File        | Encoding        | Your code | Result    |
| ----------- | --------------- | --------- | --------- |
| sample.mp3  | `0x00` (ASCII)  | OK        | ✅ Correct |
| Daavudi.mp3 | `0x01` (UTF-16) | WRONG     | ❌ Garbage |

👉 **Your code supports only ISO-8859-1, not UTF-16**

---

## 5️⃣ How to fix it (correct, proper way)

You must:

1. Read **encoding byte**
2. Handle based on encoding
3. Convert UTF-16 → UTF-8 / ASCII

---

## 6️⃣ Minimal fix logic (IMPORTANT)

### Step 1: Detect encoding

```c
unsigned char encoding = value[0];
```

### Step 2: Handle cases

```c
char *decoded = NULL;

if (encoding == 0x00) {
    // ISO-8859-1
    decoded = strdup(value + 1);
}
else if (encoding == 0x01) {
    // UTF-16 with BOM
    decoded = utf16_to_utf8(value + 1, size - 1);
}
```

---

## 7️⃣ UTF-16 → UTF-8 conversion (simple version)

For **English text only** (your files are English), UTF-16LE can be safely converted like this:

```c
char *utf16_to_utf8(unsigned char *data, int size)
{
    // skip BOM if present
    if (data[0] == 0xFF && data[1] == 0xFE)
        data += 2;

    char *out = malloc(size);
    int j = 0;

    for (int i = 0; i < size; i += 2) {
        if (data[i] == 0x00) break;
        out[j++] = data[i];
    }

    out[j] = '\0';
    return out;
}
```

✔ This alone will fix **Daavudi.mp3 completely**

---

## 8️⃣ Fix COMM parsing (mandatory)

```c
if (strcmp(tag, "COMM") == 0) {
    unsigned char enc = value[0];
    char *ptr = value + 1 + 3; // skip encoding + language

    // skip short description
    if (enc == 0x00)
        ptr += strlen(ptr) + 1;
    else
        ptr += 2; // UTF-16 empty desc

    data->comment = (enc == 0x00)
        ? strdup(ptr)
        : utf16_to_utf8((unsigned char *)ptr, size - (ptr - value));
}
```

---

## 9️⃣ Why this makes it compatible with BOTH files

| Feature            | sample.mp3 | Daavudi.mp3 |
| ------------------ | ---------- | ----------- |
| Encoding detection | ✔          | ✔           |
| ASCII handling     | ✔          | —           |
| UTF-16 handling    | —          | ✔           |
| COMM parsing       | ✔          | ✔           |
| Safe printing      | ✔          | ✔           |

---

## 🔟 Final takeaway (important for viva)

> **The failure happens because ID3v2 text frames support multiple encodings, but the parser assumes ASCII only. UTF-16 encoded frames contain null bytes, causing premature string termination. The fix is to detect the encoding byte and decode UTF-16 properly.**

If you want next, I can:

* Give you a **clean, reusable decoder**
* Make it **ID3v2.3 + v2.4 compliant**
* Help you write a **perfect project explanation**

Just say 👍

*/