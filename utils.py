def get_loop(line: str):
    numbered = [ord(c) - ord('A') for c in line]
    probed = set()
    i = 0
    loops = []
    for i in range(26):
        if i in probed:
            continue
        probed.add(i)
        loop = [i, ]
        i = numbered[i]
        while i not in probed:
            loop.append(i)
            probed.add(i)
            i = numbered[i]
        loop.append(i)
        loops.append(loop)

    return [
        "".join([chr(i + ord('A')) for i in loop])
        for loop in loops
    ]
