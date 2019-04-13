"""
functional programming on Python example: check if string is a palendrome

dependencies:
  pyfpm for pattern matching
  python 3.7.1

author:
  L1ttl3S1st3r
  4/14/2019
"""
from pyfpm.matcher import Matcher
import typing


class IncorrectArgException(ValueError):
    pass


# watch a descriptions in pyfmp docs
is_palendrome = Matcher()


# if arg is an empty string
@is_palendrome.handler("''")
def empty_string() -> bool:
    return True

# if arg matchs to default string
@is_palendrome.handler('string: str')
def correct_string(string) -> bool:
    if string[0] == string[-1]:
        return is_palendrome(string[1:-1])
    else:
        return False


@is_palendrome.handler("_")
def random_arg() -> bool:
    raise IncorrectArgException("incorrect input type")


def main() -> None:
    tests = ["abc", "b", "",
             "abcd", "abba", 1,
             None, object]

    for test in tests:
        try:
            if is_palendrome(test):
                print(str(test) + " is a palendrome")
            else:
                print(str(test) + " is not a palendrome")
        except IncorrectArgException:
            print(str(test) + " has incorrect type:" + str(type(test)))


if __name__ == "__main__":
    main()
