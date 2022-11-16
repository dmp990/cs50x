from cs50 import get_int


def main():
    while True:
        height = get_int("Height: ")
        if height > 0 and height < 9:
            break

    for i in range(1, height + 1):

        # print spaces to the left
        for k in range(0, height - i):
            print(" ", end="")

        # print hashes for the left half
        for j in range(0, i):
            print("#", end="")

        # print the two spaces in middle
        print("  ", end="")

        # print hashes for the right half
        for l in range(0, i):
            print("#", end="")

        # print a new line
        print()


main()
