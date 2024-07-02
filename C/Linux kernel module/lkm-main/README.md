# Linux Kernel Module

Hello and welcome to Mathias and Gustafs Linux Kernel Module

## Starting the program

Compile and run the server
```bash
gcc server.c -o server
./server
```
Compile and run the client
```bash
gcc myApp.c -o myApp
./myApp < ip-address > < port >
```
Compile and insert the kernel module

```bash
sudo make
sudo insmod myModule.ko
```

## Usage
Input should be entered in the client. Reading the output in the terminal should be sufficient instructions, otherwise contact either of us :)
