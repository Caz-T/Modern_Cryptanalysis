
# Mechanical structure of ENIGMA
from typing import List

ROTOR_1 = "EKMFLGDQVZNTOWYHXUSPAIBRCJ"
ROTOR_2 = "AJDKSIRUXBLHWTMCQGZNPYFVOE"
ROTOR_3 = "BDFHJLCPRTXVZNYEIWGAKMUSQO"
ROTORS = [ROTOR_1, ROTOR_2, ROTOR_3]
NOTCH = "RFW"
REFLECTOR = "YRUHQSLDPXNGOKMIEBFZCWVJAT"


class Enigma:
    def __init__(self, rotor_order: List[int], ring_setting: str, initial_pos: str, wiring_table: str):
        self._wiring_table = [i for i in range(26)]
        for pair in wiring_table.split():
            self._wiring_table[ord(pair[0]) - ord('A')] = ord(pair[1]) - ord('A')
            self._wiring_table[ord(pair[1]) - ord('A')] = ord(pair[0]) - ord('A')
        self._initial_pos = [ord(c) - ord('A') for c in initial_pos]
        self._ring_setting = [ord(c) - ord('A') for c in ring_setting]
        self._rotors = [
            [ord(c) - ord('A') for c in ROTORS[rotor_order[i] - 1]]
            for i in range(3)
        ]
        self._notch = [ord(NOTCH[rotor_order[i] - 1]) - ord('A') for i in range(3)]
        self._rev_rotors = []
        for rotor in self._rotors:
            rev_r = [-1, ] * 26
            for i in range(26):
                rev_r[rotor[i]] = i
            self._rev_rotors.append(rev_r)
        self._reflector = [ord(c) - ord('A') for c in REFLECTOR]
        self._rotor_pos = [ord(c) - ord('A') for c in initial_pos]

    def reset(self):
        self._rotor_pos = [c for c in self._initial_pos]

    def _step(self):
        # rightmost rotor:
        self._rotor_pos[2] += 1
        self._rotor_pos[2] %= 26
        if self._rotor_pos[2] == self._notch[2]:
            self._rotor_pos[1] += 1
            self._rotor_pos[1] %= 26
        else:
            if self._rotor_pos[1] == (self._notch[1] + 25) % 26:
                self._rotor_pos[1] += 1
                self._rotor_pos[1] %= 26
                self._rotor_pos[0] += 1
                self._rotor_pos[0] %= 26

    def encode(self, raw: str):
        ans = []
        for c in raw:
            c = ord(c) - ord('A')

            self._step()

            # wiring table
            c = self._wiring_table[c]

            # rotors
            for i in range(3):
                c -= self._ring_setting[2 - i]
                c += self._rotor_pos[2 - i]
                c += 26
                c %= 26
                c = self._rotors[2 - i][c]
                c += 26
                c -= self._rotor_pos[2 - i]
                c += self._ring_setting[2 - i]
                c %= 26

            # reflector
            c = self._reflector[c]

            # reverse rotors
            for i in range(3):
                c -= self._ring_setting[i]
                c += self._rotor_pos[i]
                c += 26
                c %= 26
                c = self._rev_rotors[i][c]
                c += 26
                c -= self._rotor_pos[i]
                c += self._ring_setting[i]
                c %= 26

            # wiring table
            c = self._wiring_table[c]

            ans.append(chr(c + ord('A')))
        return "".join(ans)


if __name__ == '__main__':
    enigma = Enigma([2, 3, 1], 'DES', 'AAA', 'BX GK WY EF PQ SN')
    while True:
        enigma.reset()
        q = input()
        print(enigma.encode(q))

