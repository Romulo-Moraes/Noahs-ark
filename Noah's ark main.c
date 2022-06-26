#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>

/* Macros definition */
#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1
#define FILE_TEXT_CHUNK_SIZE 128
#define CURRENT_OUTPUT_FILENAME_BUFFER_SIZE 128

#define OUTPUT_FILE "./PayloadOutput"
#define OUTPUT_FILE_EXTENSION ".txt"
#define PAYLOAD_SIZE_OUTPUT_FILE "./PayloadSizeOutput.txt"

#define READ_BYTES "rb"
#define CLASSIC_WRITE "w"
/* End macros definition */

// Some useful global variables
static char *TargetFile = NULL;
static char CurrentOutputFilename[CURRENT_OUTPUT_FILENAME_BUFFER_SIZE] = {0};
static int OutputFileChunks = 0;

/* Function prototypes */
void GetSignedNumberHex(int Value, char *OutputBuffer);
void IntToString(int Number, char *OutputBuffer);
void PrepareOutputFilename(char *FilenameBuffer, int *CurrentIndex);

int main(int argc, char *argv[])
{
    if (argc == 2 || argc == 3)
    {
        // Get the file name that will be embedded by command line
        TargetFile = argv[1];

        // Variables used in the entire program
        FILE *FileToTransform = fopen(TargetFile, READ_BYTES);
        FILE *FileToWrite = NULL;
        FILE *FileToWritePayloadSize = fopen(PAYLOAD_SIZE_OUTPUT_FILE, CLASSIC_WRITE);
        long FileSize = 0;
        char FileText[FILE_TEXT_CHUNK_SIZE] = {0}, *FinalPayloadText = NULL, CurrentHexCode[6] = {0}, FileSizeAsString[128] = {0};
        size_t ReadBytes = 0;
        int i = 0, ChunkSizeCopy = 0, r = 0;

        // Get the size of transformed file
        fseek(FileToTransform, 0, SEEK_END);
        FileSize = ftell(FileToTransform);
        fseek(FileToTransform, 0, SEEK_SET);

        // Code segment that will be runned if the whole embedded file can be put in just a file
        if (argc == 2)
        {
            PrepareOutputFilename(CurrentOutputFilename, &r);
            FileToWrite = fopen(CurrentOutputFilename, CLASSIC_WRITE);

            if (FileToWrite != NULL)
            {
                // Read the file until reach the end
                while (feof(FileToTransform) == 0)
                {
                    ReadBytes = fread(FileText, 1, FILE_TEXT_CHUNK_SIZE - 1, FileToTransform);

                    // For each byte transform it to hardcoded hex representation
                    for (i = 0; i < ReadBytes; i++)
                    {
                        if (FileText[i] < 0)
                        {
                            GetSignedNumberHex(FileText[i], CurrentHexCode);
                        }
                        else
                        {
                            sprintf(CurrentHexCode, "\\x%x", FileText[i]);
                        }

                        // Write the hex content in output file and clear your memory space
                        fwrite(CurrentHexCode, 1, strlen(CurrentHexCode), FileToWrite);
                        memset(CurrentHexCode, '\0', 6);
                    }

                    memset(FileText, '\0', FILE_TEXT_CHUNK_SIZE);
                }
            }
        }
        else
        {
            // Code segment that will be runned with a defined chunk of bytes to each output file

            // Get the chunk size by command line
            OutputFileChunks = atoi(argv[2]);

            printf("You sure that will wan't write in each file %d of chunk size ? (y / n)\n", OutputFileChunks);
            if (getchar() == 'y')
            {
                // Run until reach end of file
                while (feof(FileToTransform) == 0)
                {
                    PrepareOutputFilename(CurrentOutputFilename, &r);

                    // For each loop a new output file is created
                    FileToWrite = fopen(CurrentOutputFilename, CLASSIC_WRITE);

                    ChunkSizeCopy = OutputFileChunks;
                    while (ChunkSizeCopy != 0 && feof(FileToTransform) == 0)
                    {
                        if (ChunkSizeCopy < FILE_TEXT_CHUNK_SIZE)
                        {
                            ReadBytes = fread(FileText, 1, ChunkSizeCopy, FileToTransform);
                            ChunkSizeCopy = 0;
                        }
                        else
                        {
                            ReadBytes = fread(FileText, 1, FILE_TEXT_CHUNK_SIZE, FileToTransform);
                            ChunkSizeCopy -= FILE_TEXT_CHUNK_SIZE;
                        }

                        for (i = 0; i < ReadBytes; i++)
                        {
                            if (FileText[i] < 0)
                            {
                                GetSignedNumberHex(FileText[i], CurrentHexCode);
                            }
                            else
                            {
                                sprintf(CurrentHexCode, "\\x%x", FileText[i]);
                            }

                            fwrite(CurrentHexCode, 1, strlen(CurrentHexCode), FileToWrite);
                            memset(CurrentHexCode, '\0', 6);
                        }
                    }
                    fclose(FileToWrite);
                }
            }
            else
            {
                exit(0);
            }
        }

        // Transform and write the file size to file
        IntToString(FileSize, FileSizeAsString);
        fwrite(FileSizeAsString, 1, strlen(FileSizeAsString), FileToWritePayloadSize);

        // Close all handlers
        fclose(FileToTransform);
        fclose(FileToWritePayloadSize);
    }
    else
    {
        puts("Can't open source, output or payload output size files");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void GetSignedNumberHex(int Value, char *OutputBuffer)
{
    char buffer[128] = {0}, buffer2[128] = {0};
    int bufferIndex = 0;
    int i = 0, j = 0, k = 0;
    int finalNumber = 0;

    // This code segment will apply the two's complement

    // Transform the negative char number to positive
    Value *= -1;

    // Get the binary of this char
    while (Value != 0)
    {
        buffer[bufferIndex] = (Value % 2) + '0';
        Value = Value / 2;
        bufferIndex++;
    }

    // If not reach 8 bits of size, then insert the remaining bits
    if (bufferIndex < 8)
    {
        memset(&buffer[bufferIndex], '0', 8 - bufferIndex);
        bufferIndex += 8 - bufferIndex;
    }

    // Revert the bits array because the process above get back wards bits
    for (bufferIndex = bufferIndex - 1; bufferIndex >= 0; bufferIndex--)
    {
        buffer2[i] = buffer[bufferIndex];
        i++;
    }

    // Apply the two's complement
    for (j = 0; j < i; j++)
    {
        switch (buffer2[j])
        {
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
    for (j = j - 1; j >= 0; j--)
    {
        if (buffer2[j] == '1')
        {
            finalNumber += pow(i, 1);
        }

        i *= 2;
    }

    // End of two's complement section
    finalNumber += 1;

    // Transform the result into a Hex representation and pop out of the function
    sprintf(OutputBuffer, "\\x%x", finalNumber);
}

void IntToString(int Number, char *OutputBuffer)
{
    char BackwardsString[128] = {0};
    int BackwardsStringIndex = 0, j = 0;

    while (Number != 0)
    {
        BackwardsString[BackwardsStringIndex] = (Number % 10) + '0';
        BackwardsStringIndex++;
        Number = Number / 10;
    }

    for (int i = BackwardsStringIndex - 1; i >= 0; i--)
    {
        OutputBuffer[j] = BackwardsString[i];
        j++;
    }
}

void PrepareOutputFilename(char *FilenameBuffer, int *CurrentIndex)
{
    memset(FilenameBuffer, '\0', CURRENT_OUTPUT_FILENAME_BUFFER_SIZE);
    sprintf(FilenameBuffer, "%s%d%s", OUTPUT_FILE, *CurrentIndex, OUTPUT_FILE_EXTENSION);
    *CurrentIndex += 1;
}
