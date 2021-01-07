if (fwrite(buffer, sizeof(buffer), 1, outptr) != sizeof(buffer))
                {
                    // close outfile
                    fclose(outptr);
                }

// the first three bytes of JPEGs are: 0xff 0xd8 0xff
// The fourth byte, meanwhile, is either 0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, or 0xef. 
// Put another way, the fourth byte’s first four bits are 1110.

#include <stdio.h>
#include <stdlib.h>

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

    for(int i = 0; i < 50; i++)
    {
        // name outfile
        sprintf(filename, "%03i.jpg", i);

        // open outfile
        FILE *outptr = fopen(filename, "w");
        if (outptr == NULL)
        {
        fprintf(stderr, "Could not open %s.\n", filename);
            return 3;
        }

        // define a buffer
        int block[512];

        // read infile by block
        fread(&block, 512, 1, inptr);

        // find a signature
        if (block[0] == 0xff &&
            block[1] == 0xd8 &&
            block[2] == 0xff &&
            (block[3] & 0xf0) == 0xe0)
        {

            size_t ret = fread(block, sizeof(*block), sizeof(block)/sizeof(*block), inptr);
            int readCount = ret*sizeof(*block);

            for (int j = 0; j < readCount; j++)
            {
                // write 512 bytes
                fwrite(&block, 512, 1, outptr);
            }

        }

        // close outfile
        fclose(outptr);
    }

    if(feof(inptr) == 0)
    {
        // close infile
        fclose(inptr);
    }


    // success
    return 0;
}
