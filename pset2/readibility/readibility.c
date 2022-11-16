#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);
int main(void)
{
    // prompt for input
    string text = get_string("Text: ");

    // count letters
    int letter_count = count_letters(text);
    printf("letter count: %d\n", letter_count);

    // count words
    int word_count = count_words(text);
    printf("word count: %d\n", word_count);

    // count sentences
    int sentence_count = count_sentences(text);
    printf("sentence count: %d\n", sentence_count);

    // index = 0.0588 * L - 0.296 * S - 15.8

    float L = letter_count / (float)word_count * 100;
    float S = sentence_count / (float)word_count * 100;

    printf("L is: %f\nS is: %f\n", L, S);
    float i = 0.0588 * L - 0.296 * S - 15.8;
    int index = round(i);

    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index > 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %d\n", index);
    }
}

int count_letters(string text)
{
    int count = 0;
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        // only count alphabets as letters
        if (isalpha(text[i]))
        {
            count++;
        }
    }
    return count;
}

int count_words(string text)
{
    int count = 1;
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        // spaces separate words
        if (isspace(text[i]))
        {
            count++;
        }
    }
    return count;
}

int count_sentences(string text)
{
    int count = 0;
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        // for this problem, a sentence ends with . ! ? only
        if ((int)text[i] == (int)'.' || (int)text[i] == (int)'!' || (int)text[i] == (int)'?')
        {
            count++;
        }
    }
    return count;
}