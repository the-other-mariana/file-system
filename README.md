# Virtual File System

This a simulation of a file system made in C++. <br />

*Note: I developed it alongside [@devtrillo](https://github.com/devtrillo), but I did it with my past account, so I forked it again.*

## Specifications

C++ version: `c++ 14`

## Get Started With CLion (Recommended)

### Windows 10

1. Download JetBrains CLion.
2. Activate your account (university email activates you a free package).
3. Go to `All Settings`.
4. Go to `Build, Execution and Deployment` option.
5. Go to `Tool Chains`
6. Click on the `+` button.
7. Choose MINGW (gcc compiler for Windows).
8. On `Environment` field, click on `Download`.
9. On the web page, click on `Sourceforge`
10. On the exe file that is downloaded, click on 86_64 option if your computer is 64 bits (most likely).
11. Go back to CLion and search in :C/Program Files/mingw-wg4 and choose the last folder named `mingw64`.
12. All other fields will be detected if this was correctly installed.
13. Click `Apply`.
14. Move the created MinGW to the upper position in the list, so that it is default.
15. Click on `OK`.
16. Create a new c++ executable project and it should look like this: <br />

![alt text]()<br/>

## Usage

These are the main commands that you can type once the file `main.cpp` is running. <br />

### 1. Create a virtual file system

To create your file system (empty initialization), go to the output program's directory and type: <br />

```
&: create <name> <blockSize> <numberOfBlocks>
```

### 2. Open a previously saved file system

```
&: open <name>
```

### 3. Load a file to your file system

```
&: load original.txt copyname.txt
```

### 4. Download a file from your file system to your machine

```
&: download copy.txt newCopy.txt
```

### 5. See a file's details

```
&: details myFile.txt
```

### 6. List all file system files

```
&: ls
```

### 7. See your file system's info

```
&: info
```
