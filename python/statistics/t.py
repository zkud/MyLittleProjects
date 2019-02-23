#!python3

"""
t coeff calculator for Lesha
"""

from scipy.stats import t


def t_coef(confidence: float, n: int) -> float:
    return t.ppf((confidence + 1) / 2, n - 1)


def main():
    try:
        n = int(input("input n:"))
        confidence = float(
            input("input confidence (0, 1):").replace(',', '.')
        )

        assert ((n > 0) and (0 < confidence < 1))

        print("t value is {}".format(t_coef(confidence, n)))
    except:
        print("input values are incorrect")


if __name__ == "__main__":
    main()
