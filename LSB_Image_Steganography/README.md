# 🔐 LSB Image Steganography

Hide secret messages inside BMP images using the **Least Significant Bit (LSB)** technique — completely invisible to the human eye.

---

## 📖 What is LSB Steganography?

Steganography is the art of hiding information within a cover medium. This project hides a secret text file **inside a BMP image** by replacing the least significant bit of each pixel's color byte with bits from the secret data.

Since changing just the last bit of a color value (e.g., `10110100` → `10110101`) alters the color by only 1 out of 255, the resulting image looks **identical** to the original to any human observer.

```
Original Pixel Byte:  1 0 1 1 0 1 0 [0]
Secret Bit:                           [1]
Stego Pixel Byte:     1 0 1 1 0 1 0 [1]  ← imperceptible change
```

---

## 📁 Project Structure

```
LSB_Image_Steganography/
│
├── main.c          # Entry point — parses arguments, routes to encode/decode
├── encode.c        # Encoding logic (hides secret data into BMP)
├── encode.h        # Encoding function prototypes and EncodeInfo struct
├── decode.c        # Decoding logic (extracts secret data from BMP)
├── decode.h        # Decoding function prototypes and decodeInfo struct
├── common.h        # Shared constants (MAGIC_STRING, buffer sizes)
├── types.h         # Custom types (Status, OperationType enums)
│
├── beautiful.bmp   # Sample cover image (for testing)
├── secret.txt      # Sample secret message file (for testing)
├── steged_img.bmp  # Sample output stego image (pre-generated)
└── decoded.txt     # Sample decoded output (pre-generated)
```

---

## ⚙️ How It Works

### Encoding (Hiding data)
1. Reads the BMP image header (54 bytes) and copies it unchanged to the output.
2. Embeds a **magic string** (`#*`) at the start of pixel data — used to verify during decoding.
3. Encodes the **secret file's extension** (e.g., `.txt`) so it can be restored.
4. Encodes the **size** of the secret file (so the decoder knows when to stop).
5. Encodes the **actual secret file data**, one bit per pixel byte.
6. Copies remaining image bytes unchanged.

### Decoding (Extracting data)
1. Skips the BMP header (54 bytes).
2. Reads and verifies the **magic string** — confirms the image was stego'd by this tool.
3. Extracts the **file extension**, **file size**, and **file data** in reverse order.
4. Writes the recovered data to an output file.

---

## 🛠️ Requirements

- GCC compiler (or any C99-compatible compiler)
- Linux / macOS / Windows (with MinGW or WSL)
- A `.bmp` image file as the cover image
- A `.txt` file containing your secret message

---

## 🚀 Getting Started

### 1. Clone the Repository

```bash
git clone https://github.com/KishoreKK275/Emertxe.git
cd LSB_Image_Steganography
```

### 2. Compile the Code

```bash
gcc main.c encode.c decode.c -o stego
```

This produces an executable named `stego` (on Windows it will be `stego.exe`).

---

## 💻 Usage

### 🔏 Encode — Hide a secret file inside a BMP image

```bash
./stego -e <cover_image.bmp> <secret_file.txt> [output_image.bmp]
```

| Argument | Description |
|---|---|
| `-e` | Flag to select **encoding** mode |
| `<cover_image.bmp>` | Source BMP image to hide data in |
| `<secret_file.txt>` | The text file containing your secret message |
| `[output_image.bmp]` | *(Optional)* Output stego image name. Defaults to `steged_img.bmp` |

**Example:**

```bash
./stego -e beautiful.bmp secret.txt output.bmp
```

**Expected output:**
```
INFO: Encoding selected
INFO: Opening required files
INFO: Opened beautiful.bmp
INFO: Opened secret.txt
INFO: Opened output.bmp
INFO: ## Encoding Completed Successfully ##
```

---

### 🔓 Decode — Extract the hidden secret from a stego BMP image

```bash
./stego -d <stego_image.bmp> [output_file]
```

| Argument | Description |
|---|---|
| `-d` | Flag to select **decoding** mode |
| `<stego_image.bmp>` | The stego BMP image containing hidden data |
| `[output_file]` | *(Optional)* Name for the recovered file. Defaults to `decoded.txt` |

**Example:**

```bash
./stego -d output.bmp recovered.txt
```

**Expected output:**
```
INFO: Decoding selected
INFO: ## Decoding Completed Successfully ##
```

---

## 🧪 Quick Test with Sample Files

The repo includes sample files so you can test right away:

```bash
# Step 1 — Compile
gcc main.c encode.c decode.c -o stego

# Step 2 — Encode: hide secret.txt inside beautiful.bmp
./stego -e beautiful.bmp secret.txt my_stego.bmp

# Step 3 — Decode: recover the hidden message
./stego -d my_stego.bmp my_recovered.txt

# Step 4 — Verify the recovered message matches the original
cat my_recovered.txt
```

---

## ⚠️ Limitations

- Only supports **BMP** image format (no JPEG, PNG, etc.)
- The cover image must be large enough to hold the secret data.
  - **Capacity rule:** `image_width × image_height × 3 bytes ≥ (secret_file_size + overhead) × 8`
- Only `.txt` files have been tested as secret files, though any file type should work in theory.
- The BMP image must use **24-bit color** (3 bytes per pixel: R, G, B).

---

## 🔍 Error Messages

| Error | Meaning |
|---|---|
| `ERROR: Unsupported operation` | Invalid flag used (only `-e` or `-d` allowed) |
| `ERROR: Unable to open file` | File not found or permission denied |
| `ERROR: Stego image must be a .bmp file` | Provided image is not a `.bmp` file |
| `ERROR: Image capacity is insufficient` | Cover image is too small for the secret file |
| `ERROR: Magic string not found` | The image was not encoded by this tool |

---

## 👤 Author

**Kishore**
Date: January 2026

---

## 📄 License

This project is open source and available under the [MIT License](LICENSE).
