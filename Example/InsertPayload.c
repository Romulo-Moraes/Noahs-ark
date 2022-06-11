#include <stdio.h>
#include <string.h>
#include "Payload.h"

int main(){
    FILE* FileToWrite = fopen("./EmbeddedHelloWorld","wb");

    fwrite(PAYLOAD,1,16696,FileToWrite);

    fclose(FileToWrite);
    return 0;
}