#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "editor.h"
#include "utils.h"
TagData *edit_tag(const char *option, const char *new_text, const char *filename) {
    char frame_id[5];//Temp buf to store 4 bytes of tag name and 1 byte of null
    if (!option || !new_text || !filename) {
        display_error("Invalid arguments.");
        return NULL;
    }
    if (strlen(new_text) == 0) {
        display_error("New text cannot be empty.");
        return NULL;
    }
    if      (strcmp(option, "-t") == 0) strcpy(frame_id, "TIT2");
    else if (strcmp(option, "-a") == 0) strcpy(frame_id, "TPE1");
    else if (strcmp(option, "-A") == 0) strcpy(frame_id, "TALB");
    else if (strcmp(option, "-y") == 0) strcpy(frame_id, "TYER");
    else if (strcmp(option, "-n") == 0) strcpy(frame_id, "TCON");
    else if (strcmp(option, "-c") == 0) strcpy(frame_id, "COMM");
    else {
        display_error("Invalid edit option.");
        return NULL;
    }
    FILE *src  = fopen(filename, "rb");
    FILE *dest = fopen("Temp.mp3", "wb");//Temp file to perform operations
    if(!src || !dest) {
        display_error("Invalid file");
        fclose(src);    fclose(dest);
        return NULL;
    }
    unsigned char header[10];//Temp buffer to hold header
    fread(header, 1, 10, src);//read header
    if (strncmp(header, "ID3", 3) != 0) {
        display_error("No ID3 tag found.");
        fclose(src); fclose(dest);
        return NULL;
    }
    fwrite(header, 1, 10, dest);//write header
    //Convert 4 bytes of size (char) into int 
    long old_tag_size =((header[6] & 0x7F) << 21) |((header[7] & 0x7F) << 14) |((header[8] & 0x7F) << 7)  |(header[9] & 0x7F);
    long tag_end = 10 + old_tag_size;//header+tag data(meta data)
    int size_diff = 0, flag =0;//temp variables== flag - to write only one data, size_diff - to find size diff. of old & new data
    while (ftell(src) < tag_end) {
        //temp buffers to hold tag name,size,flag
        unsigned char tag[5];
        unsigned char size_bytes[4];
        unsigned char flags[2];
        if (fread(tag, 1, 4, src) != 4)//read tag
            break;
        if (tag[0] == '\0')//To Find end of tags and skip
            break;
        tag[4] = '\0';
        if (fread(size_bytes, 1, 4, src) != 4)//read size
            break;
        //find size of each tag and convert to int
        int size = (size_bytes[0] << 24) |(size_bytes[1] << 16) |(size_bytes[2] << 8)  |(size_bytes[3]);
        if (fread(flags, 1, 2, src) != 2)//read flag
            break;
        if (!strcmp(tag, frame_id) && !flag) {
            int new_size = 1 + strlen(new_text);// size of new text + 1 (NULL)
            //convert size into 4 bytes of char
            unsigned char new_size_bytes[4] ={(new_size >> 24) & 0xFF,(new_size >> 16) & 0xFF,(new_size >> 8)  & 0xFF,new_size & 0xFF};
            fwrite(tag, 1, 4, dest);//write tag
            fwrite(new_size_bytes, 1, 4, dest);//write size
            fwrite(flags, 1, 2, dest);//write flags
            fputc(0x00, dest);//write null after flags & before tag data
            fwrite(new_text, 1, strlen(new_text), dest);//write new_tag_data
            fseek(src, size, SEEK_CUR);// skip old data in source file
            size_diff += (new_size - size);//find size difference between new data and old data
            flag = 1;//temp variable to make sure only one edit is done 
        }
        else {
            fwrite(tag, 1, 4, dest);//write tag
            fwrite(size_bytes, 1, 4, dest);//write size
            fwrite(flags, 1, 2, dest);//write flags
            char *buf = malloc(size);//buffer to hold tag data 
            fread(buf, 1, size, src);//read tag data
            fwrite(buf, 1, size, dest);//write tag data
            free(buf);//free buffer
        }
    }
    unsigned char buffer[1024 * 64];//buffer to hold auido data
    size_t  audio_data; // Size of original audio data
    while((audio_data = fread(buffer,1,sizeof(buffer),src)) >0)
        fwrite(buffer,1,sizeof(buffer),dest);
    int new_tag_size = old_tag_size + size_diff;
    // store this new size in header in char data type
    header[6] = (new_tag_size >> 21) & 0x7F;
    header[7] = (new_tag_size >> 14) & 0x7F;
    header[8] = (new_tag_size >> 7)  & 0x7F;
    header[9] =  new_tag_size        & 0x7F;
    rewind(dest);
    fwrite(header, 1, 10, dest);//write header into dest again(rewrite size of meta data) with new meta data size
    fclose(src); fclose(dest);//Close files
    remove(filename);//Remove old file
    rename("Temp.mp3", filename);//Rename temp file with old file name(replace)
    printf("INFO: Tag edited successfully. File updated: %s\n", filename);
}
int edit_tags(const char *option, const char *new_text, const char *filename) {
    if(edit_tag(option, new_text, filename)==NULL)
    {
        display_error("Failed to edit ID3 tags.");
        return 1;
    }
    return 0;
}

/*
🧱 Example MP3 File (Before Edit)

Assume this file:

song.mp3
Title  : Hello
Artist : Adele
Album  : 25
Year   : 2015


We run:

./mp3tag -e -t "Hello World" song.mp3

🧩 1️⃣ MP3 FILE LAYOUT (BEFORE)
|<----------- ID3 TAG ---------->|<------ AUDIO ------>|
|                                |                     |
+----+---------------------------+---------------------+
|HDR |  FRAMES (metadata)        |  MP3 audio frames   |
+----+---------------------------+---------------------+
0   10                          tag_end

🧩 2️⃣ ID3 HEADER (10 bytes)
Offset  Bytes     Meaning
--------------------------------
0–2     ID3       Identifier
3       03        Version (ID3v2.3)
4       00        Revision
5       00        Flags
6–9     00 00 00 40   ← TAG SIZE = 64 bytes


🔴 Tag size = size of all frames + padding, NOT header

🧩 3️⃣ FRAME STRUCTURE (Before Edit)
TIT2 (Title frame)
Offset   Data
-----------------------------
+0       'T' 'I' 'T' '2'
+4       00 00 00 06   ← size = 6 bytes
+8       00 00         ← flags
+10      00            ← encoding
+11      H e l l o


So visually:

TIT2 | 00 00 00 06 | 00 00 | 00 Hello

🧩 4️⃣ What your EDIT code does
Step-by-step logic:
✅ Step A: Open files
src  = song.mp3
dest = Out.mp3

✅ Step B: Copy ID3 header (temporarily)
fread(header, 10, src);
fwrite(header, 10, dest);


⚠️ Header still has OLD tag size
(we will fix it later)

✅ Step C: Start reading frames
while (ftell(src) < tag_end)


This ensures:

You only read metadata

You never touch audio here

🧩 5️⃣ Frame-by-frame processing (VISUAL)
🎯 When frame ≠ target (e.g., TPE1)
SRC:  TPE1 | size | flags | data
DEST: TPE1 | size | flags | data


➡️ Copied exactly, byte-for-byte.

🎯 When frame == target (TIT2)
Old frame
TIT2 | size=6 | flags | 00 Hello

New text
"Hello World"


Length:

strlen("Hello World") = 11
+ 1 encoding byte
------------------
new_size = 12

New frame written to dest
TIT2 | 00 00 00 0C | 00 00 | 00 Hello World

Size difference calculated
old_size = 6
new_size = 12

size_diff = +6


This is CRITICAL.

Source file pointer move
fseek(src, old_size, SEEK_CUR);


Why?

Because:

You already wrote new data

Old data must be skipped

Otherwise you duplicate content

🧩 6️⃣ What happens in memory (VISUAL)
SRC FILE                 DEST FILE
-------------------------------------------------
TIT2 size=6             TIT2 size=12
"Hello"                 "Hello World"


Everything else is identical.

🧩 7️⃣ Padding detection

If frame ID becomes:

00 00 00 00


It means:

No more frames

Your fixed code stops reading frames here.

🧩 8️⃣ Copying AUDIO (IMPORTANT)

Once:

ftell(src) >= tag_end


Everything else is pure MP3 audio.

So your code does:

while (EOF)
    copy byte


This guarantees:

Audio is untouched

Bit-perfect copy

No re-encoding

🧩 9️⃣ Fixing ID3 HEADER (MOST IMPORTANT STEP)

Original tag size:

64 bytes


Size difference:

+6 bytes


New tag size:

70 bytes


Converted to syncsafe integer:

70 decimal = 0x46

header[6] = 00
header[7] = 00
header[8] = 00
header[9] = 46


Then written back:

fseek(dest, 0, SEEK_SET);
fwrite(header, 10, dest);

🧩 🔟 Final MP3 Layout (AFTER)
|<------------- ID3 TAG (70 bytes) ------------>|<-- AUDIO -->|
+----+------------------------------------------+-------------+
|HDR | TIT2 (Hello World) + other frames        | MP3 frames  |
+----+------------------------------------------+-------------+


🎉 MP3 players now read:

Correct metadata

Correct audio start

No corruption
*/


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
