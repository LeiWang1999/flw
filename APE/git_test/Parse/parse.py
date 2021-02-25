import os
import numpy as np
import argparse

if __name__ == '__main__':

    parser = argparse.ArgumentParser(description = "Argument Info")
    parser.add_argument('-t', '--file_name', type=str, default="objdump.txt")
    parser.add_argument('-m', '--Mode', type=str, default="back")
    parser.add_argument('-e', '--Endian', type=str, default="big")
    args = parser.parse_args()

    with open(args.file_name) as txt:
        content = txt.readlines()
        txt.close()

    lines = np.array(content)
    lines_of_txt = lines.size
    print("lines of txt : ", lines_of_txt)

    list = []
    list_deplay = [[] for i in range(10)]

    for i in range(10):
        list_deplay[i].append("section_init_0x" + str(i) + "0000000.txt\n")

    for i in range(0, lines_of_txt):

        if (lines[i][0].isdigit() and lines[i][0] != '\n'):
            each_line = lines[i].replace('\t', '')

            each_line = each_line.split(":", 1)
            str_line = str(each_line[1][0:8])

            # add '0' at the end
            if (args.Mode == "back"):
                if(str_line != ' '):
                    str_line = str_line.replace(" ", '0')

            # add '0' in front
            if (args.Mode == "front"):
                if(str_line != ' '):
                    str_line = str_line.rstrip()
                    str_line = str_line.rjust(8, '0')

            # Remove spaces
            if (str_line == '\n'):
                str_line = ""
            else:
            # Big Endian
                if (args.Endian == "big"):
                    str_line = str_line + '\n'
                    list.append(str_line)

                # Small Endian
                if (args.Endian == "small"):
                    list_str_line = []
                    for i in range (0, len(str_line), 2):
                        list_str_line.append(str_line[i:i+2])

                    str_line = ""
                    i = len(list_str_line) - 1
                    while i >= 0:
                        str_line += list_str_line[i]
                        i -= 1
                    str_line = str_line + '\n'
                    list.append(str_line)
            # the full message of each_line
            # each_line = each_line[0] + ": " + str_line
            # index = int(each_line[0][0])
            # list_deplay[index].append(str_line)


    with open("BigEndine.txt", 'w') as out_file:

        # add print message
        # for i in range(10):
        #     if (len(list_deplay[i]) > 1):
        #         out_file.writelines(list_deplay[i])

        out_file.writelines(list)
        out_file.close()


