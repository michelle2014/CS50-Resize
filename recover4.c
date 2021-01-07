#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t  BYTE;

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: forensic image name\n");
        return 1;
    }

    // remember filename
    char *infile = argv[1];

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // char array to store name string
    char filename[50];

    // define a buffer
    BYTE buffer[512];

    // initialize image name from 0
    int i = 0;

    // output file
    FILE *outptr = NULL;

    // read infile by block
    while (fread(buffer, sizeof(buffer), 1, inptr) == 1)
    {

        // find a signature
        if (buffer[0] == 0xff &&
            buffer[1] == 0xd8 &&
            buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0)
        {
            // name outfile
            sprintf(filename, "%03i.jpg", i);
            ++i;

            // open outfile
            outptr = fopen(filename, "w");


            // write 512 bytes
            fwrite(buffer, sizeof(buffer), 1, outptr);
        }
        // if not signature, continue to write until find the next one
        else if (i > 0)
        {
            fwrite(buffer, sizeof(buffer), 1, outptr);
        }
    }

    // if at the end of file
    if (feof(inptr) == 0)
    {
        // close infile
        fclose(inptr);

        // close outfile
        fclose(outptr);
    }

    // success
    return 0;
}