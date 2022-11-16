#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // check command-line arguments
    if (argc != 2)
    {
        printf("Usage: ./recover card.raw\n");
        return 1;
    }

    // open file
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Error opening file.\n");
        return 1;
    }

    BYTE *buffer = malloc(512);
    int count = 0;
    char filename[8];
    sprintf(filename, "%03i.jpg", count);

    FILE *img = fopen(filename, "w");
    if (!img)
    {
        printf("Error opening output file\n");
        return 1;
    }

    while (fread(buffer, 512, 1, input)) // read block by block until EOF
    {
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && ((buffer[3] & 0xF0) == 0xE0)) // start of a new file
        {
            if (img)
            {
                fclose(img);
                sprintf(filename, "%03i.jpg", count);
                img = fopen(filename, "w");
                if (!img)
                {
                    printf("Error opening output file\n");
                    return 1;
                }
                printf("created a new file: %s\n", filename);
            }
            count++;
        }
        fwrite(buffer, 512, 1, img);
    }

    free(buffer);
    fclose(input);
    fclose(img);
    return 0;
}