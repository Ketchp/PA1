import random
import sys

"""
stdin -> test input
stderr -> ref output
"""

MAX_SIZE = 5
SMALLEST_CHAR = ord('A')
LARGEST_CHAR = ord('Z')

rounds = int(input())

ranks = int(input())

items = dict()

def getRandString( max_size ):
    if random.randrange(0,100) > 20 and items.keys():
        return random.choice( list( items.keys() ) )
    size = random.randint(1, max_size)
    return ''.join([ chr( random.randint(SMALLEST_CHAR, LARGEST_CHAR)) for i in range(size)])

print( "Pocet sledovanych:", file = sys.stderr )
print( ranks )
print( "Pozadavky:", file = sys.stderr )

for round in range(rounds):
    string = getRandString( MAX_SIZE )

    if string not in items:
        items[string] = 0
    items[string] += 1

    print( f'+ {string}' )
    print( '#' )

    all_ranks = []
    ranked = dict()
    for k, v in items.items():
        if v not in ranked:
            ranked[v] = []
            all_ranks.append(v)
        ranked[v].append(k)

    all_ranks.sort(reverse=True)

    ranks_so_far = 1
    sold = 0
    for i in all_ranks:
        for string in ranked[i]:
            if len(ranked[i]) > 1:
                print( f"{ranks_so_far}.-{ranks_so_far + len(ranked[i]) - 1}. {string}, {i}x", file = sys.stderr )
            else:
                print( f"{ranks_so_far}. {string}, {i}x", file = sys.stderr )
        ranks_so_far += len(ranked[i])
        sold += i * len(ranked[i])
        if ranks_so_far > ranks:
            break


    print( f"Nejprodavanejsi zbozi: prodano {sold} kusu", file = sys.stderr )
