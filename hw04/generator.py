import random

MAX_ID = 10**5
MAX_RANGE = 10**6


for i in range(MAX_RANGE):
    print("+", random.randrange(0, MAX_ID))
    start = random.randint(0, i)
    end = random.randint(start, i)
    print("?", start, end)