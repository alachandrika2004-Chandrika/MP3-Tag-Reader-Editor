# 🎵 MP3 Tag Header Editor (ID3 Editor in C)

## 📌 Overview
This project is a **command-line based MP3 tag editor** written in C.  
It allows users to **view and modify ID3 metadata** of MP3 files.

You can read and update tags like Title, Artist, Album, Year, Comment, and Genre using simple command-line arguments.

---

## 🚀 Features

- 📖 Read MP3 tag metadata  
- ✏️ Edit specific ID3 tags  
- ⚙️ Command-line argument handling  
- 🛡️ Error handling and validation  
- 🧩 Modular code structure  

---

## 🛠️ Technologies Used

- C Programming  
- File Handling (`fopen`, `fread`, `fwrite`)  
- Structures & Pointers  
- Command Line Arguments (`argc`, `argv`)  

---

## 📂 Project Structure

├── main.c
├── read.c
├── edit.c
├── header.h
├── types.h
├── song2.mp3
├── images/
│ └── output.png
└── README.md

## ⚙️ How to Compile

```bash
gcc *.c -o mp3_tag_editor
▶️ Usage
🔍 View Tags
./mp3_tag_editor -r song2.mp3
✏️ Edit Tags
./mp3_tag_editor -w -c telugu song2.mp3
📖 Supported Tags
Tag	Description
TIT2	Title
TPE1	Artist
TALB	Album
TYER	Year
COMM	Comment
TCON	Genre

🧾 Example Output
Read and validation completed

Details of TAG TALB
Album : Aarya - (2004)

Details of TAG TPE1
Artist : Malathi, Ranjith

Details of TAG COMM
Comment : adhish

Details of TAG TCON
Content Type : telugu

Details of TAG TIT2
Title : [iSongs.info] 06 - Aa Ante Amalapuram

Details of TAG TYER
Year : 2004

Tag updated successfully
⚠️ Error Handling

Invalid arguments detection

File format validation

Tag validation

Safe update mechanism

🎯 Learning Outcomes

This project helped in understanding:

Binary file handling in C

Memory management

Structures and pointers

Real-world system-level programming

🔮 Future Improvements

GUI-based application

Support for more ID3 versions

Batch editing multiple files

Cross-platform support

📜 License

This project is open-source and free to use.
#Author
Chandrika ala
