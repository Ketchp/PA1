import random
import sys

MAX_ID = 10**5
MAX_RANGE = 10**6

print("Zadaj rozsah a max id", file=sys.stderr)
rng, max_id = map(int, input().split())
max_id = min(max_id, MAX_ID)
rng = min(rng, MAX_RANGE)
for i in range(rng):
    print("+", random.randrange(0, max_id))
    start = random.randint(0, i)
    end = random.randint(start, i)
    print("?", start, end)