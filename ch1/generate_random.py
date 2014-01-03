import random

lower, upper = 2 * 10 ** 6, 10 ** 7 - 1

for n in xrange(10 ** 7):
    print random.randint(lower, upper)
