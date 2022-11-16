#include <cs50.h>
#include <stdio.h>

int main(void)
{
    long card = get_long("Number: ");

    long first_number = card;
    int number_of_digits = 2;

    while (first_number >= 100)
    {
        first_number /= 10;
        number_of_digits++;
    }
    // printf("Numebr of digits: %d\n", number_of_digits);

    int stepOneSum = 0;
    int stepTwoSum = 0;
    long cpy = card;
    // printf("%ld\n", cpy);
    while (cpy > 0)
    {
        stepTwoSum += cpy % 10;
        cpy /= 10;
        // printf("cpy is: %ld\n", cpy);
        int number = 2 * (cpy % 10);
        int sum = 0;
        while (number > 0)
        {
            sum += number % 10;
            number /= 10;
        }
        // printf("sum is: %d\n", sum);
        stepOneSum += sum;
        cpy /= 10;
        // printf("cpy is: %ld\n", cpy);
    }
    // printf("Stepone sum %d\nStep two sum: %d\n", stepOneSum, stepTwoSum);

    int finalSum = stepOneSum + stepTwoSum;
    // printf("finalsum: %d\n", finalSum);
    if (finalSum % 10 == 0)
    {
        if (number_of_digits == 15 && (first_number == 34 || first_number == 37))
        {
            printf("AMEX\n");
        }
        else if ((number_of_digits == 13 || number_of_digits == 16) && first_number / 10 == 4)
        {
            printf("VISA\n");
        }
        else if (number_of_digits == 16 && (first_number == 51 || first_number == 52 || first_number == 53 || first_number == 54 || first_number == 55))
        {
            printf("MASTERCARD\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }
}