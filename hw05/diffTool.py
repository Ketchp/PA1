import sys
import shutil
import re

contex = []

def srt(string):
    m = re.search("\.", string)
    if m:
        return int(string[:m.start()])
    return 10000

def printDiff(list1, list2, sep):
    width = shutil.get_terminal_size((80, 20)).columns // 2 - 3
    for a,b in contex:
        for idx in range(max( len(a), len(b) )):
            line1 = a[idx][:width].strip().ljust( width ) if len(a) > idx else ''
            line2 = b[idx][:width].strip().ljust( width ) if len(b) > idx else ''
            print(line1 + (' '*len(sep) if line1 == line2 else sep) + line2)

    list1 = sorted(sorted(list1) , key=srt)
    list2 = sorted(sorted(list2) , key=srt)
    for idx in range(max( len(list1), len(list2) )):
        line1 = list1[idx][:width].strip().ljust( width ) if len(list1) > idx else ''
        line2 = list2[idx][:width].strip().ljust( width ) if len(list2) > idx else ''
        print(line1 + (' '*len(sep) if line1 == line2 else sep) + line2)
    exit( 1 )

first = sys.argv[1]
second = sys.argv[2]
with open(first, "r") as file1:
    with open(second, "r") as file2:
        while True:
            buffer1 = []
            buffer2 = []
            new_line1 = file1.readline()
            new_line2 = file2.readline()
            while( new_line1 ):
                buffer1.append(new_line1)
                if new_line1[0] == "N": break
                new_line1 = file1.readline()

            while( new_line2 ):
                buffer2.append(new_line2)
                if new_line2[0] == "N": break
                new_line2 = file2.readline()

#            print("buf1", buffer1)
#            print("buf2", buffer2)
            if( new_line1 == '' and new_line2 == '' ):
                break

            if( new_line1 == '' and new_line2 == '' ):
                printDiff(buffer1, buffer2, ' a ')
                continue

            sor1 = sorted(buffer1)
            sor2 = sorted(buffer2)
            if(len(sor1) != len(sor2)):
                printDiff(buffer1, buffer2, ' b ')
                continue

            for i in range(len(sor1)):
                if(sor1[i] != sor2[i]):
                    print(sor1)
                    print(sor2)
                    printDiff(buffer1, buffer2, ' c ')
                    break

            contex.insert(0, (buffer1, buffer2))
            if len(contex) > 1:
                contex.pop()
exit(0)