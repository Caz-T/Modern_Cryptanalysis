
# Mechanical structure of ENIGMA
from typing import List

ROTOR_1 = "EKMFLGDQVZNTOWYHXUSPAIBRCJ"
ROTOR_2 = "AJDKSIRUXBLHWTMCQGZNPYFVOE"
ROTOR_3 = "BDFHJLCPRTXVZNYEIWGAKMUSQO"
ROTORS = [ROTOR_1, ROTOR_2, ROTOR_3]
REFLECTOR = "YRUHQSLDPXNGOKMIEBFZCWVJAT"


class Enigma:
    def __init__(self, rotor_order: List[int], ring_setting: str, initial_pos: str, wiring_table: list):
        self._wiring_table = wiring_table
        self._initial_pos = [ord(c) - ord('A') for c in initial_pos]
        self._ring_setting = [ord(c) - ord('A') for c in ring_setting]
        self._rotors = [
            [ord(c) - ord('A') for c in ROTORS[rotor_order[i] - 1]]
            for i in range(3)
        ]
        self._rev_rotors = []
        for rotor in self._rotors:
            rev_r = [-1, ] * 26
            for i in range(26):
                rev_r[rotor[i]] = i
            self._rev_rotors.append(rev_r)
        self._reflector = [ord(c) - ord('A') for c in REFLECTOR]

        self._rotor_pos = [c for c in initial_pos]

    def reset(self):
        self._rotor_pos = [c for c in self._initial_pos]

    def _step(self):
        carry = 1
        for digit in range(3):
            self._rotor_pos[2 - digit] += carry
            self._rotor_pos[2 - digit] /= 26
            carry = 1 if self._rotor_pos[2 - digit] == self._ring_setting[2 - digit] else 0

    def encode(self, raw: str):
        ans = []
        for c in raw:
            c = ord(c)

            self._step()
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
                c = self._rotors[i][c]
                c += 26
                c -= self._rotor_pos[2 - i]
                c -= self._rotor_pos[i]
                c %= 26
            ans.append(chr(c + ord('A')))
        return "".join(ans)


if __name__ == '__main__':
    pass

