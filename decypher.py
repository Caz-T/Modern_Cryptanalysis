import json

from enigma import Enigma
from utils import get_loop


def decypher():
    # We assume fixed ring orders 'DES'
    catalogue = {}
    for rotor_order in [[1, 2, 3], [1, 3, 2], [2, 1, 3], [2, 3, 1], [3, 1, 2], [3, 2, 1]]:
        print("Rotor order: %d-%d-%d" % tuple(rotor_order))
        curr_dict = {}
        for i in range(26):
            print(chr(ord('A') + i), end='')
            for j in range(26):
                for k in range(26):
                    init_pos = chr(ord('A') + i) + chr(ord('A') + j) + chr(ord('A') + k)
                    enigma = Enigma(rotor_order, 'DES', init_pos, "")
                    pattern = [[-1, ] * 26] * 3
                    for c in range(26):
                        output = enigma.encode(chr(c + ord('A')) * 6)
                        enigma.reset()
                        for t in range(3):
                            pattern[t][ord(output[t]) - ord('A')] = ord(output[t + 3]) - ord('A')

                    # calculate loop info:
                    loop_patt = []
                    for t in range(3):
                        loops = get_loop("".join([chr(u + ord('A')) for u in pattern[t]]))
                        len_loops = [len(loop) for loop in loops]
                        len_loops.sort()
                        loop_patt.append("-".join([str(len_loop) for len_loop in len_loops]))

                    # add loop info to catalogue, using length patterns as key:
                    if loop_patt[0] not in curr_dict:
                        curr_dict[loop_patt[0]] = {}
                    if loop_patt[1] not in curr_dict[loop_patt[0]]:
                        curr_dict[loop_patt[0]][loop_patt[1]] = {}
                    if loop_patt[2] not in curr_dict[loop_patt[0]][loop_patt[1]]:
                        curr_dict[loop_patt[0]][loop_patt[1]][loop_patt[2]] = []
                    curr_dict[loop_patt[0]][loop_patt[1]][loop_patt[2]].append(init_pos)
        print()

        catalogue["-".join([str(t) for t in rotor_order])] = curr_dict

    with open('catalogue.json', mode='w', encoding='utf-8') as fo:
        json.dump(catalogue, fo)


if __name__ == '__main__':
    decypher()
