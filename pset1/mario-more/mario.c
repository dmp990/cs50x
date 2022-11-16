#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int height = 0;
    // prompt user to input height -> [1, 8]
    do
    {
        height = get_int("Height: ");
    } while (height < 1 || height > 8);

    // build the pyramid
    for (int i = 1; i <= height; i++)
    {
        // print spaces on left
        for (int k = 0; k < height - i; k++)
        {
            printf(" ");
        }
        // print hashes for the left half
        for (int j = 0; j < i; j++)
        {
            printf("#");
        }

        // add the middle space
        printf("  ");

        // print hashes for the right half
        for (int l = 0; l < i; l++)
        {
            printf("#");
        }

        printf("\n");
    }
}