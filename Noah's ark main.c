#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1
#define FILE_TEXT_CHUNK_SIZE 128

static char *TargetFile = NULL;

#define OUTPUT_FILE "./PayloadOutput.txt"
#define PAYLOAD_SIZE_OUTPUT_FILE "./PayloadSizeOutput.txt"

#define READ_BYTES "rb"
#define CLASSIC_WRITE "w"

void GetSignedNumberHex(int Value, char *OutputBuffer);
void IntToString(int Number,char *OutputBuffer);

int main(int argc, char *argv[]){
    if(argc == 2){
        TargetFile = argv[1];

        FILE *FileToTransform = fopen(TargetFile,READ_BYTES);
        FILE *FileToWrite = fopen(OUTPUT_FILE,CLASSIC_WRITE);
        FILE *FileToWritePayloadSize = fopen(PAYLOAD_SIZE_OUTPUT_FILE,CLASSIC_WRITE);
        long FileSize = 0;
        char FileText[FILE_TEXT_CHUNK_SIZE] = {0}, *FinalPayloadText = NULL,CurrentHexCode[6] = {0}, FileSizeAsString[128] = {0};
        size_t ReadBytes = 0;
        int i = 0;

        if(FileToTransform != NULL && FileToWrite != NULL && FileToWritePayloadSize != NULL){
            fseek(FileToTransform,0,SEEK_END);
            FileSize = ftell(FileToTransform);

            fseek(FileToTransform,0,SEEK_SET);


            while(feof(FileToTransform) == 0){
                ReadBytes = fread(FileText,1,FILE_TEXT_CHUNK_SIZE - 1,FileToTransform);

                for(i = 0; i < ReadBytes; i++){
                    if(FileText[i] < 0){
                        GetSignedNumberHex(FileText[i],CurrentHexCode);
                    }
                    else{
                        sprintf(CurrentHexCode,"\\x%x",FileText[i]);
                    }
                    fwrite(CurrentHexCode,1,strlen(CurrentHexCode),FileToWrite);
                    memset(CurrentHexCode,'\0',6);
                }

                memset(FileText,'\0',FILE_TEXT_CHUNK_SIZE);
            }

            IntToString(FileSize,FileSizeAsString);

            fwrite(FileSizeAsString,1,strlen(FileSizeAsString),FileToWritePayloadSize);
        }
        else{
            puts("Can't open source, output or payload output size files");
            return EXIT_FAILURE;
        }

        fclose(FileToWrite);
        fclose(FileToTransform);
        fclose(FileToWritePayloadSize);

        return EXIT_SUCCESS;
    }
    else{
        puts("You need pass a file target path as argument\n\nExample: Ark.exe \"Photo.jpg\"");
        return EXIT_FAILURE;
    }
}


void GetSignedNumberHex(int Value, char *OutputBuffer){
    char buffer[128] = {0},buffer2[128] = {0};
    int bufferIndex = 0;
    int i = 0, j = 0, k = 0;
    int finalNumber = 0;

    // This code segment will apply the two's complement

    // Transform the negative char number to positive
    Value *= -1;

    // Get the binary of this char
    while(Value != 0){
        buffer[bufferIndex] = (Value % 2) + '0';
        Value = Value / 2;
        bufferIndex++;
    }

    // If not reach 8 bits of size, then insert the remaining bits
    if(bufferIndex < 8){
        memset(&buffer[bufferIndex],'0',8 - bufferIndex);
        bufferIndex += 8 - bufferIndex;
    }

    // Revert the bits array because the process above get back wards bits
    for(bufferIndex = bufferIndex - 1; bufferIndex >= 0; bufferIndex--){
        buffer2[i] = buffer[bufferIndex];
        i++;
    }

    // Apply the two's complement
    for(j = 0; j < i; j++){
        switch(buffer2[j]){
            case '1':
                buffer2[j] = '0';
                break;
            case '0':
                buffer2[j] = '1';
                break;
        }
    }


    // Transform the bits to a real number
    i = 1;
    for(j = j - 1; j >= 0; j--){
        if(buffer2[j] == '1'){
            finalNumber += pow(i,1);
        }

        i *= 2;
    }

    // End of two's complement section
    finalNumber += 1;

    // Transform the result into a Hex representation and pop out of the function
    sprintf(OutputBuffer,"\\x%x",finalNumber);
}

void IntToString(int Number,char *OutputBuffer){
    char BackwardsString[128] = {0};
    int BackwardsStringIndex = 0, j = 0;

    while(Number != 0){
        BackwardsString[BackwardsStringIndex] = (Number % 10) + '0';
        BackwardsStringIndex++;
        Number = Number / 10;
    }

    for(int i = BackwardsStringIndex - 1; i >= 0; i--){
        OutputBuffer[j] = BackwardsString[i];
        j++;
    }
}