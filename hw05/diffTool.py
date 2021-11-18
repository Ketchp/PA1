import sys
import shutil


def printDiff(list1, list2):
    screen_width = shutil.get_terminal_size((80, 20)).columns
    for idx in range(max( len(list1), len(list2) )):
        line1 = list1[idx].strip().ljust( (screen_width-1) // 2)
        line2 = list2[idx].strip().ljust( (screen_width-1) // 2)
        print(line1 + (' ' if line1 == line2 else '|') + line2)

first = sys.argv[1]
second = sys.argv[2]
buffer1 = []
buffer2 = []
with open(first, "r") as file1:
    with open(second, "r") as file2:
        while True:
            new_line1 = file1.readline()
            new_line2 = file2.readline()
            if( new_line1 ): buffer1.append(new_line1)
            if( new_line2 ): buffer2.append(new_line2)
            if( new_line1 == new_line2 == '' ): break

            if new_line1 and new_line1[0] == "N" or new_line2 and new_line2[0] == "N":
                while(new_line1[0] != "N"):
                    new_line1 = file1.readline()
                    buffer1.append(new_line1)
                while(new_line2[0] != "N"):
                    new_line2 = file2.readline()
                    buffer2.append(new_line2)

                sor1 = sorted(buffer1)
                sor2 = sorted(buffer2)
                if(len(sor1) != len(sor2)):
                    printDiff(buffer1, buffer2)
                    buffer1 = []
                    buffer2 = []
                    continue

                for i in range(len(sor1)):
                    if(sor1[i] != sor2[i]):
                        printDiff(buffer1, buffer2)
                        buffer1 = []
                        buffer2 = []
                        break
                else:
                    buffer1 = []
                    buffer2 = []


