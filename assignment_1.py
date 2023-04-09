import itertools

import numpy as np
import numpy.linalg

LETTER_FREQ = [
    .082, .015, .028, .043, .127, .022, .020, .061, .070, .002, .008, .040, .024,
    .067, .075, .019, .001, .060, .063, .091, .028, .010, .023, .001, .020, .001,
]


def assignment_1_5():
    encrypted = "BEEAKFYDJXUQYHYJIQRYHTYJIQFBQDUYJIIKFUHCQD"
    for i in range(26):
        print(chr(ord("A") + i), end='')
        print("".join([chr((ord(c) - ord('A') + i) % 26 + ord('A')) for c in encrypted]))


def assignment_1_16_b():
    encrypted = "ETEGENLMDNTNEOORDAHATECOESAHLRMI"
    offset = [3, -1, 3, -2, 2, -3, 1, -3]  # relative to index
    index = 0
    answer = ""
    for _ in encrypted:
        answer += encrypted[index + offset[index % 8]]
        index += 1
    print(answer)


def countfreq(s: str):
    l = [0, ] * 26
    for c in s:
        l[ord(c) - ord('A')] += 1
    return l


def assignment_1_21_a():
    encrypted = 'EMGLOSUDCGDNCUSWYSFHNSFCYKDPUMLWGYICOXYSIPJCKQPKUGKMGOLICGINCGACKSNISACYKZSCKXECJCKSHYSXCGOIDPKZCNKSHICGIWYGKKGKGOLDSILKGOIUSIGLEDSPWZUGFZCCNDGYYSFUSZCNXEOJNCGYEOWEUPXEZGACGNFGLKNSACIGOIYCKXCJUCIUZCFZCCNDGYYSFEUEKUZCSOCFZCCNCIACZEJNCSHFZEJZEGMXCYHCJUMGKUCY'
    cnt = countfreq(encrypted)

    encrypted = encrypted.replace('C', 'e')
    encrypted = encrypted.replace('F', 'w')

    encrypted = '#' + encrypted + '#'
    cnt = {}
    cnt_rev = {}
    for i in range(len(encrypted)):
        if encrypted[i] == 'e':
            cnt[encrypted[i + 1]] = cnt.get(encrypted[i + 1], 0) + 1
            cnt_rev[encrypted[i - 1]] = cnt_rev.get(encrypted[i - 1], 0) + 1

    """for i in range(len(encrypted)):
        if encrypted[i] == 'Z' and encrypted[i + 1] == 'e':
            print("\\item ...", encrypted[i - 3: i + 6], "...")"""
    encrypted = encrypted.replace('Z', 'h')
    encrypted = encrypted.replace('N', 'l')
    encrypted = encrypted.replace('Y', 'r')
    encrypted = encrypted.replace('S', 'o')
    encrypted = encrypted.replace('G', 'a')
    encrypted = encrypted.replace('D', 'b')
    encrypted = encrypted.replace('J', 'c')
    encrypted = encrypted.replace('O', 'n')
    encrypted = encrypted.replace('U', 't')
    encrypted = encrypted.replace('X', 'p')
    encrypted = encrypted.replace('H', 'f')
    encrypted = encrypted.replace('E', 'i')
    encrypted = encrypted.replace('W', 'g')

    for pair in [
        ('A', 'v'), ('K', 's'), ('P', 'u'), ('L', 'y'), ('I', 'd'), ('M', 'm'), ('Q', 'j')
    ]:
        encrypted = encrypted.replace(pair[0], pair[1])

    print(encrypted)


def assignment_1_21_b():
    encrypted = "KCCPKBGUFDPHQTYAVINRRTMVGRKDNBVFDETDGILTXRGUDDKOTFMBPVGEGLTGCKQRACQCWDNAWCRXIZAKFTLEWRPTYCQKYVXCHKFTPONCQQRHJVAJUWETMCMSPKQDYHJVDAHCTRLSVSKCGCZQQDZXGSFRLSWCWSJTBHAFSIASPRJAHKJRJUMVGKMITZHFPDISPZLVLGWTFPLKKEBDPGCEBSHCTJRWXBAFSPEZQNRWXCVYCGAONWDDKACKAWBBIKFTIOVKCGGHJVLNHIFFSQESVYCLACNVRWBBIREPBBVFEXOSCDYGZWPFDTKFQIYCWHJVLNHIQIBTKHJVNPIST"

    for i in range(1, 10):
        print("d = %d: " % i, end='')
        for j in range(i):
            freq = [0, ] * 26
            index = j
            while index < len(encrypted):
                freq[ord(encrypted[index]) - ord('A')] += 1
                index += i
            total = sum(freq)
            print("%.4f " % (sum([f * (f - 1) for f in freq]) / total / (total - 1)), end='')
        print()

    chunks = [encrypted[i::6] for i in range(6)]
    most_freq = []
    for chunk in chunks:
        max_en = max(enumerate(countfreq(chunk)), key=lambda t: t[1])[0]
        most_freq.append(max_en)

    decrypt_chunks = []
    for i in range(6):
        res = []
        for offset in range(26):
            decrypted = "".join([chr((ord(c) - ord('A') - offset) % 26 + ord('A')) for c in chunks[i]])
            freq = [0, ] * 26
            for ch in decrypted:
                freq[ord(ch) - ord('A')] += 1
            total = sum(freq)
            # compute relativity index
            res.append((decrypted, sum([freq[j] * LETTER_FREQ[j] / total for j in range(26)])))
        decrypt_chunks.append(min(res, key=lambda t: abs(t[1] - 0.065))[0])
    product = "".join([decrypt_chunks[i % 6][int((i - i % 6) / 6)] for i in range(len(encrypted))])
    print(product)


def assignment_1_21_c():
    encrypted = "KQEREJEBCPPCJCRKIEACUZBKRVPKRBCIBQCARBJCVFCUPKRIOFKPACUZQEPBKRXPEIIEABDKPBCPFCDCCAFIEABDKPBCPFEQPKAZBKRHAIBKAPCCIBURCCDKDCCJCIDFUIXPAFFERBICZDFKABICBBENEFCUPJCVKABPCYDCCDPKBCOCPERKIVKSCPICBRKIJPKABI"
    freq = countfreq(encrypted)
    l1 = "频率&"
    l2 = "频率&"
    for i in range(13):
        l1 += "%.1f\\%%&" % (freq[i] / len(encrypted) * 100)
        l2 += "%.1f\\%%&" % (freq[i + 13] / len(encrypted) * 100)
    print(l1)
    print(l2)

    print("".join([chr(((ord(c) - ord('A')) * 11 + 8) % 26 + ord('a')) for c in encrypted]))


def assignment_1_21_d():
    encrypted = "BNVSNSIHQCEELSSKKYERIFJKXUMBGYKAMQLJTYAVFBKVTDVBPVVRJYYLAOKYMPQSCGDLFSRLLPROYGESEBUUALRWXMMASAZLGLEDFJBZAVVPXWICGJXASCBYEHOSNMULKCEAHTQOKMFLEBKFXLRRFDTZXCIWBJSICBGAWDVYDHAVFJXZIBKCGJIWEAHTTOEWTUHKRQVVRGZBXYIREMMASCSPBHLHJMBLRFFJELHWEYLWISTFVVYEJCMHYUYRUFSFMGESIGRLWALSWMNUHSIMYYITCCQPZSICEHBCCMZFEGVJYOCDEMMPGHVAAUMELCMOEHVLTIPSUYILVGFLMVWDVYDBTHFRAYISYSGKVSUUHYHGGCKTMBLRX"
    freq = countfreq(encrypted)
    l1 = "频率&"
    l2 = "频率&"
    for i in range(13):
        l1 += "%.1f\\%%&" % (freq[i] / len(encrypted) * 100)
        l2 += "%.1f\\%%&" % (freq[i + 13] / len(encrypted) * 100)
    print(l1)
    print(l2)

    for i in range(1, 10):
        print("d = %d: " % i, end='')
        for j in range(i):
            freq = [0, ] * 26
            index = j
            while index < len(encrypted):
                freq[ord(encrypted[index]) - ord('A')] += 1
                index += i
            total = sum(freq)
            print("%.4f " % (sum([f * (f - 1) for f in freq]) / total / (total - 1)), end='')
        print()

    chunks = [encrypted[i::6] for i in range(6)]
    most_freq = []
    for chunk in chunks:
        max_en = max(enumerate(countfreq(chunk)), key=lambda t: t[1])[0]
        most_freq.append(max_en)

    decrypt_chunks = []
    for i in range(6):
        res = []
        for offset in range(26):
            decrypted = "".join([chr((ord(c) - ord('A') - offset) % 26 + ord('A')) for c in chunks[i]])
            freq = [0, ] * 26
            for ch in decrypted:
                freq[ord(ch) - ord('A')] += 1
            total = sum(freq)
            # compute relativity index
            res.append((decrypted, sum([freq[j] * LETTER_FREQ[j] / total for j in range(26)])))
        decrypt_chunks.append(min(res, key=lambda t: abs(t[1] - 0.065))[0])
    product = "".join([decrypt_chunks[i % 6][int((i - i % 6) / 6)] for i in range(len(encrypted))])
    print(product)


def assignment_1_25():
    encrypted = "LMQETXYEAGTXCTUIEWNCTXLZEWUAISPZYVAPEWLMGQWYAXFTCJMSQCADAGTXLMDXNXSNPJQSYVAPRIQSMHNOCVAXFV"
    enc_grouped = [np.mat([ord(encrypted[2 * i]) - ord('A'),
                           ord(encrypted[2 * i + 1]) - ord('A')]
                          ) for i in range(int(len(encrypted) / 2))
                   ]
    cnt = {}
    for i in range(len(encrypted) - 1):
        cnt[encrypted[i: i + 2]] = cnt.get(encrypted[i: i + 2], 0) + 1
    key_freqs = []
    for key in cnt:
        key_freqs.append((key, cnt[key]))
    key_freqs.sort(key=lambda t: t[1], reverse=True)
    crypto_mat = np.mat([
        [ord(c) - ord('A') for c in key_freqs[0][0]],
        [ord(c) - ord('A') for c in key_freqs[1][0]],
    ])
    print(crypto_mat)
    # the following line is computed by hand, since crypto_mat = [[19, 23], [11, 12]] is fixed
    crypto_mat_inv = np.mat([[12, 3], [15, 19]])

    hi_freqs = ['TH', 'HE', 'IN', 'ER']
    # hi_freqs = ['TH', 'HE', 'IN', 'ER', 'AN', 'RE', 'DE', 'ON', 'ES', 'ST', 'EN', 'AT', 'TO', 'NT', 'HA',
    #             'ND', 'OU', 'EA', 'NG', 'AS', 'OR', 'TI', 'IS', 'ET', 'IT', 'AR', 'TE', 'SE', 'HI', 'OF']
    for c1, c2 in itertools.product(hi_freqs, hi_freqs):
        if c1 == c2:
            continue
        origin_mat = np.mat([
            [ord(c) - ord('A') for c in c1],
            [ord(c) - ord('A') for c in c2],
        ])
        # Actually k is K^{-1} in textbook
        k = np.matmul(crypto_mat_inv, origin_mat)
        for i in range(2):
            for j in range(2):
                k[i, j] %= 26
        res = ''
        for group in enc_grouped:
            resmat = np.matmul(group, k)
            for j in range(2):
                resmat[0, j] %= 26
            res += chr(resmat[0, 0] + ord('A'))
            res += chr(resmat[0, 1] + ord('A'))
        if len(res):
            print("%s %s: %s" % (c1, c2, res))


if __name__ == '__main__':
    assignment_1_25()
