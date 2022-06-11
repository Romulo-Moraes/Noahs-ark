#include <stdio.h>
#include <string.h>
#include "Payload.h"

int main(){
    FILE* FileToWrite = fopen("./EmbeddedHelloWorld.exe","wb");

    fwrite(PAYLOAD,1,40764,FileToWrite);

    fclose(FileToWrite);
    return 0;
}