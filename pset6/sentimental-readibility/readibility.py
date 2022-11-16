from cs50 import get_string


def main():
    # Prompt for user input
    text = get_string("Text: ")

    # count letters
    letter_count = count_letters(text)
    print("letter count:", letter_count)

    # count words
    word_count = count_words(text)
    print(f"word count: {word_count}")

    # count sentences
    sentence_count = count_sentences(text)
    print(f"sentence count: {sentence_count}")

    # calculate Coleman-Liau index
    L = letter_count / word_count * 100
    S = sentence_count / word_count * 100

    i = 0.0588 * L - 0.296 * S - 15.8
    index = round(i)

    if index < 1:
        print("Before Grade 1")
    elif index > 10:
        print("Grade 16+")
    else:
        print(f"Grade {index}")


def count_letters(str):
    count = 0
    for c in str:
        if c.isalpha():
            count += 1
    return count


def count_words(text):
    count = 1
    for c in text:
        if c.isspace():
            count += 1
    return count


def count_sentences(text):
    count = 0
    for c in text:
        if c == '.' or c == '!' or c == '?':
            count += 1
    return count


main()
