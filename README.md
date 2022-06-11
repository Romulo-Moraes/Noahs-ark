<div align="center">
    <h1>Noah's ark tool</h1>
    <img width="500px" src="./Assets/Ark.png"/>
    <p>Created by Rômulo Moraes</p>
</div>

#

## What's this
Noah's ark is a tool written in the C programming language, multiplatform source code that can make a program be embedded of another program. Where you can store a binary such as images files or a executable file inside a buffer.

## How to use
The main file of the project is the "Noah's ark main.c" this will make the whole process, the project already has the last version of that script pre-compiled, but feel free to re-compile the source code.

### Project requirements:

<ol>
    <li> math.h lib </li>
</ol>

### Using the executable
After compile the source code, or using the pre-compiled version, the only program argument that it requires is the binary target file that will be embedded, like the following example:

    Ark.exe "./Photo.png"

After of the above command the program will pop out two reserved files: "./PayloadOutput.txt" that is the transformed binary content and "./PayloadSizeOutput.txt" that is the size of the binary content. After this you have all you need to make a embedded program.

### Making a embedded program
Since you transpilated the binary source file you can just copy the content of "./PayloadOutput.txt" and put in a MACRO of the C language, or another thing of some language. For example i ran the code in a binary of a Hello world program, then the value was put in a MACRO, like that:

```c
#define PAYLOAD "\x4d\x5a\x90\x0\x3\x0\x0\x0\x4\xb8\x0\x0\x0......"
```

The PAYLOAD usually is big, or huge, depends of the transpilated program, then you need avoid put it in the Stack segment, this can make a Stack Overflow.

After of payload declared, just import the file to another C code file and use it like the following:
```c
#include <stdio.h>
#include <string.h>
#include "Payload.h"

int main(){
    FILE* FileToWrite = fopen("./EmbeddedHelloWorld.exe","wb");

    // The size to be write in the file is in "./PayloadSizeOutput.txt" file
    fwrite(PAYLOAD,1,40764,FileToWrite);

    fclose(FileToWrite);
    return 0;
}
```
this will recreate the transpilated code to a executable called EmbeddedHelloWorld.exe that just can be runned.

### The whole example of the tutorial is present in /Example