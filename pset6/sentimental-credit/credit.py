from cs50 import get_int


def main():
    card = get_int("Number: ")

    first_number = card
    number_of_digits = 2

    while first_number >= 100:
        first_number = first_number // 10
        number_of_digits += 1

    stepOneSum = 0
    stepTwoSum = 0
    cpy = card

    while cpy > 0:
        stepTwoSum += cpy % 10
        cpy //= 10
        number = 2 * (cpy % 10)
        sum = 0
        while number > 0:
            sum += number % 10
            number //= 10
        stepOneSum += sum
        cpy //= 10

    finalSum = stepOneSum + stepTwoSum

    if finalSum % 10 == 0:
        if number_of_digits == 15 and (first_number == 34 or first_number == 37):
            print("AMEX")
        elif (number_of_digits == 13 or number_of_digits == 16) and first_number // 10 == 4:
            print("VISA")
        elif (number_of_digits == 16 and (first_number == 51 or first_number == 52 or first_number == 53 or first_number == 54 or first_number == 55)):
            print("MASTERCARD")
        else:
            print("INVALID")
    else:
        print("INVALID")


main()
