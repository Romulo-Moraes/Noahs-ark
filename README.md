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

Another method of write the content without the size constant is using the sizeof macro of C language.

```c
#include <stdio.h>
#include <string.h>
#include "Payload.h"

int main(){
    FILE* FileToWrite = fopen("./EmbeddedHelloWorld.exe","wb");

    // The size to be write in the file is in "./PayloadSizeOutput.txt" file
    fwrite(PAYLOAD,1,sizeof(PAYLOAD) - 1,FileToWrite);

    fclose(FileToWrite);
    return 0;
}
```
Always remember that the sizeof take the last null byte that comes in PAYLOAD definition, then the "sizeof() - 1" become useful

After of write the payload in disk just execute it!
```ps
.\EmbeddedHelloWorld.exe
```
the program will run normally, program output:
```ps
Hello world!
```


### Output chunk size feature
Some C compilers like the CL of Microsoft has a limitation in size of a constant in the code, with this new version is possible split the output of Noah's ark in more than one file, each file with a specified size of bytes.

```sh
# To use this feature, put the max chunk size that each output file can hold
# Like in the example below

./Ark ./Executable 8040
```

A question will pop up asking if the chunk size is correct, a small size can spawn a lot of files in the current directory. Just give a 'y' to continue or 'n' to exit.


### Using a script language
The root directory of project has a Python version of the C program, the use is the same of the previous explanation above.
```sh
python3 "./Noah's ark main.py" <FileToTransform> <Chunk size (if necessary)>
```