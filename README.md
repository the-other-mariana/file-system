# Virtual File System

This a simulation of a file system made in C++. <br />

*Note: I developed it alongside [@devtrillo](https://github.com/devtrillo), but I did it with my past account, so I forked it again.*

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
