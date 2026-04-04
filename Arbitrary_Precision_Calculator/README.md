# 🔢 Arbitrary Precision Calculator (APC)

A command-line calculator written in **C** that performs arithmetic on **integers of any size** — far beyond what `int`, `long`, or even `long long` can hold. Built using a **doubly linked list** to store each digit individually.

---

## 📖 What is Arbitrary Precision?

Standard C integer types have fixed limits:

| Type        | Max Value                    |
|-------------|------------------------------|
| `int`       | ~2.1 billion                 |
| `long long` | ~9.2 quintillion             |
| **APC**     | ♾️ **No limit**              |

This calculator stores numbers **digit by digit** in a doubly linked list, so it can handle numbers with hundreds or thousands of digits — useful in cryptography, competitive programming, and scientific computing.

```
Number: 9 8 7 6 5 4 3 2 1
         ↓   ↓   ↓
  [9] ↔ [8] ↔ [7] ↔ [6] ↔ ... ↔ [1]
  head                             tail
```

Arithmetic is done from **tail → head** (right to left), just like manual paper arithmetic.

---

## 📁 Project Structure

```
Arbitrary_Precision_Calculator/
│
├── main.c    # Entry point — argument parsing, sign detection, operator routing
├── apc.c     # Core: linked list operations, input validation, utility functions
├── apc.h     # Struct definition (node), all function prototypes, constants
├── add.c     # Addition algorithm with carry
├── sub.c     # Subtraction algorithm with borrow
├── mul.c     # Long multiplication with partial products
├── div.c     # Long division (digit-by-digit subtraction method)
└── mod.c     # Modulus (remainder after division)
```

---

## ⚙️ How It Works

### Data Structure
Each operand is stored as a **doubly linked list** of single digits:

```c
typedef struct node {
    struct node *prev;
    int data;       // single digit: 0–9
    struct node *next;
} node;
```

### Pipeline for Every Calculation
1. **Validate** all arguments (digit-only strings, valid operator).
2. **Detect signs** — track `+`/`-` flags for each operand separately.
3. **Create linked lists** — one node per digit, left to right.
4. **Remove leading zeros** (e.g., `007` becomes `7`).
5. **Compare** operands to determine which is larger (needed for subtraction sign logic).
6. **Execute** the arithmetic operation, building a result linked list.
7. **Print** the result with correct sign.
8. **Free** all three linked lists.

### Sign Logic
| Operation | Condition | Result Sign |
|-----------|-----------|-------------|
| `(+A) + (+B)` | Same sign | `+` |
| `(-A) + (-B)` | Same sign | `-` |
| `(+A) + (-B)` | A > B | `+` |
| `(-A) + (+B)` | B > A | `+` |
| `A x B` | Different signs | `-` (XOR of signs) |
| `A / B` | Different signs | `-` (XOR of signs) |
| `A % B` | A is negative | `-` |

---

## 🛠️ Requirements

- GCC compiler (or any C99-compatible compiler)
- Linux / macOS / Windows (with MinGW or WSL)

---

## 🚀 Getting Started

### 1. Clone the Repository

```bash
git clone https://github.com/KishoreKK275/Emertxe.git
cd Arbitrary_Precision_Calculator
```

### 2. Compile the Code

```bash
gcc *.c -o apc
```

---

## 💻 Usage

```bash
./apc <operand1> <operator> <operand2>
```

| Operator | Operation      | Notes                          |
|----------|---------------|-------------------------------|
| `+`      | Addition       |                               |
| `-`      | Subtraction    |                               |
| `x` or `X` | Multiplication |                             |
| `/`      | Division       | Returns integer quotient only |
| `%`      | Modulus        | Returns remainder only        |

> ⚠️ The `*` operator is reserved by the shell. Use `x` or `X` for multiplication.

---

## 🧪 Examples

### Addition
```bash
./apc 99999999999999999999 + 1
```
```
Num1: +99999999999999999999
Num2: +1
Result: 100000000000000000000
```

### Subtraction
```bash
./apc 1000000000000 - 999999999999
```
```
Num1: +1000000000000
Num2: +999999999999
Result: 1
```

### Multiplication
```bash
./apc 123456789 x 987654321
```
```
Num1: +123456789
Num2: +987654321
Result: 121932631112635269
```

### Division
```bash
./apc 1000000000000000000 / 3
```
```
Num1: +1000000000000000000
Num2: +3
Result: 333333333333333333
```

### Modulus
```bash
./apc 999999999999999999 % 7
```
```
Num1: +999999999999999999
Num2: +7
Result: 5
```

### Negative Numbers
```bash
./apc -500 + 200
./apc -300 x -400
./apc -999 - -1
```
```
Result: -300
Result: +120000
Result: -998
```

### Very Large Numbers
```bash
./apc 123456789012345678901234567890 x 987654321098765432109876543210
```
```
Result: 121932631137021795226185032733622923332237463801111263526900
```

---

## ⚠️ Error Cases

| Input | Error |
|-------|-------|
| `./apc 10 + ` | `Error: Invalid Operand 2` |
| `./apc abc + 5` | `Error: Invalid Operand 1` |
| `./apc 10 * 5` | `Error: Invalid Operator` (use `x`) |
| `./apc 10 / 0` | `Error: Division by zero` |
| `./apc 10 % 0` | `Error: Modulus by zero` |
| `./apc 10 +` | `USAGE: ./apc <operand1> <operator> <operand2>` |

---

## 👤 Author

**Kishore**  
Date: March 2026

---

## 📄 License

This project is open source and available under the [MIT License](LICENSE).
