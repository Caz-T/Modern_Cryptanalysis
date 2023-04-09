import json
from typing import List

from enigma import Enigma


def decypher(loops: List[List[int]]):
    # We assume fixed ring orders 'DES'
    possibilities = []
    for rotor_order in [[1, 2, 3], [1, 3, 2], [2, 1, 3], [2, 3, 1], [3, 1, 2], [3, 2, 1]]:
        print("Rotor order: %d-%d-%d" % tuple(rotor_order), end='')
        print(", %d possibilities found" % len(possibilities))
        for i in range(26):
            print(chr(ord('A') + i), end='')
            for j in range(26):
                for k in range(26):
                    init_rotor = chr(ord('A') + i) + chr(ord('A') + j) + chr(ord('A') + k)
                    enigma = Enigma(rotor_order, "DES", init_rotor, "")

                    # construct test input
                    for ch in range(26):
                        flag = True
                        ch = chr(ch + ord('A'))

                        for loop in loops:
                            prev_fin = ch
                            for cnt in loop:
                                enigma.reset()
                                prev_fin = enigma.encrypt((cnt + 1) * prev_fin)[-1]
                            if prev_fin != ch:
                                flag = False
                                break

                        if flag:
                            possibilities.append((rotor_order, init_rotor, ch))
        print()
    return possibilities


if __name__ == '__main__':
    res = decypher([
        [7, 8, 1, 16, 6],
        [7, 11, 4],
        [7, 8, 9, 10, 12],
        [7, 8, 1, 2, 3, 16, 6],
        [7, 8, 9, 0, 1, 16, 6],
    ])
    with open('turing.json', mode='w', encoding='utf-8') as fo:
        json.dump(res, fo)



