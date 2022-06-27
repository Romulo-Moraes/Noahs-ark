from io import SEEK_END, SEEK_SET
import math
import sys

# Constants definition 
EXIT_SUCCESS = 0
EXIT_FAILURE = 1
FILE_TEXT_CHUNK_SIZE = 128
CURRENT_OUTPUT_FILENAME_BUFFER_SIZE = 128

OUTPUT_FILE = "./PayloadOutput"
OUTPUT_FILE_EXTENSION = ".txt"
PAYLOAD_SIZE_OUTPUT_FILE = "./PayloadSizeOutput.txt"

READ_BYTES = "rb"
CLASSIC_WRITE = "w"

#Global variables
TargetFile = None
CurrentOutputFilename = None
OutputFileChunks = None

def main():
    argc = len(sys.argv)
    if(argc == 2 or argc == 3):

        TargetFile = sys.argv[1]
        print(sys.argv)
        FileToTransform = open(TargetFile,READ_BYTES)
        FileToWrite = None
        FileToWritePayloadSize = open(PAYLOAD_SIZE_OUTPUT_FILE,CLASSIC_WRITE)
        FileSize = 0
        FileText = None
        i = 0
        r = 0
        ChunkSizeCopy = 0

        FileToTransform.seek(0,SEEK_END)
        FileSize = FileToTransform.tell()
        FileToTransform.seek(0,SEEK_SET)

        if(argc == 2):
            [CurrentOutputFilename,r] = PrepareOutputFilename(r)
            FileToWrite = open(CurrentOutputFilename,CLASSIC_WRITE)

            if(FileToWrite != None):
                FileText = FileToTransform.read(FILE_TEXT_CHUNK_SIZE)
                while FileText:
                    for i in range(len(FileText)):
                        FileToWrite.write(f"\\{hex(FileText[i])[1:]}")
                    FileText = FileToTransform.read(FILE_TEXT_CHUNK_SIZE)

        else:
            if(sys.argv[2].isnumeric()):
                OutputFileChunks = int(sys.argv[2])
                if(OutputFileChunks != 0):
                    print(f"You sure that will wan't write in each file {OutputFileChunks} of chunk size ? (y / n)\n")
                    if(input("") == "y"):
                        NecessaryFileCount =  math.ceil(FileSize / OutputFileChunks)
                        print(FileSize)
                        print(NecessaryFileCount)
                        for i in range(NecessaryFileCount):
                            print("Penis")
                            ChunkSizeCopy = OutputFileChunks
                            [CurrentOutputFilename,r] = PrepareOutputFilename(r)
                            print(CurrentOutputFilename)
                            FileToWrite = open(CurrentOutputFilename,CLASSIC_WRITE)

                            while ChunkSizeCopy != 0:
                                if(ChunkSizeCopy < FILE_TEXT_CHUNK_SIZE):
                                    FileText = FileToTransform.read(ChunkSizeCopy)
                                    ChunkSizeCopy = 0
                                else:
                                    FileText = FileToTransform.read(FILE_TEXT_CHUNK_SIZE)
                                    ChunkSizeCopy -= FILE_TEXT_CHUNK_SIZE
                                for k in range(len(FileText)):
                                    FileToWrite.write(f"\\{hex(FileText[k])[1:]}")
                            FileToWrite.close();
                    else:
                        exit(EXIT_SUCCESS)
                else:
                    pass
            else:
                print("File chunks is not numeric. Exiting...")
                sys.exit(EXIT_FAILURE)

        FileToWritePayloadSize.write(str(FileSize))
        FileToTransform.close()
        FileToWritePayloadSize.close()

    else:
        pass


def PrepareOutputFilename(CurrentIndex):
    ListOfReturn = [f"{OUTPUT_FILE}{CurrentIndex}{OUTPUT_FILE_EXTENSION}"]
    CurrentIndex += 1
    ListOfReturn.append(CurrentIndex)

    return ListOfReturn

if(__name__ == "__main__"):
    main()
    
