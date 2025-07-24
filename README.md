# Huffman-Compression

An efficient C implementation of the **Huffman compression algorithm** for lossless data compression.

-----

## Getting Started

### Compiling

Navigate to the project directory and compile using GCC:

```bash
gcc -Wall huffman.c -o huffman
```

This creates an executable named `huffman`.

### Usage

Run the `huffman` executable with an argument and your file name:

```bash
./huffman [argument] [fileName]
```

| Argument | Action         |
| :------- | :------------- |
| `-c`     | **Compress** |
| `-d`     | **Decompress** |

**Examples:**

  * **Compress `my_document.txt`:**
    ```bash
    ./huffman -c my_document.txt
    ```
  * **Decompress `compressed_file.bin`:**
    ```bash
    ./huffman -d compressed_file.bin
    ```
