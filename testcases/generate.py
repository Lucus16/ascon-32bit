#!/usr/bin/env python3

import sys

def chunks(l, n):
    return [l[i:i + n] for i in range(0, len(l), n)]

def hex_to_c(digits):
    lines = chunks(chunks(digits, 2), 8)
    lines = [' '.join('0x{},'.format(byte) for byte in data) for data in lines]
    return '{\n' + ''.join('        ' + line + '\n' for line in lines) + '    },'

def lens_to_c(vecs, i, name):
    start = 'const unsigned long long {}_sizes[] = '.format(name) + '{\n    '
    middle = ' '.join('{},'.format(len(vec[i]) // 2) for vec in vecs)
    end = '\n};\n\n'
    return start + middle + end

def vecs_to_c(vecs, i, name):
    sizes = ''
    if any(len(vec[i]) != len(vecs[0][i]) for vec in vecs):
        sizes = lens_to_c(vecs, i, name)
    else:
        sizes = '#define {}_SIZE {}\n'.format(name.upper(), len(vecs[0][i]) // 2)

    start = 'const unsigned char {}s[][{}] = '.format(name,
            max(len(vec[i]) for vec in vecs)) + '{\n    '
    middle = ' '.join(hex_to_c(digits[i]) for digits in vecs)
    end = '\n};\n'
    return sizes + start + middle + end

def generate(input_file, name):
    with open(input_file, 'r') as f:
        lines = f.readlines()

    vecs = [chunk[1:7] for chunk in chunks(lines, 8)]
    vecs = [tuple(line[10:].strip() for line in lines) for lines in vecs]
    vecs = list(dict.fromkeys(vecs))

    print('#define TESTCASE_COUNT {}\n'.format(len(vecs)))
    print(vecs_to_c(vecs, 0, 'key'))
    print(vecs_to_c(vecs, 1, 'pub_msg_num'))
    print(vecs_to_c(vecs, 2, 'additional'))
    print(vecs_to_c(vecs, 3, 'plain_text'))
    print(vecs_to_c(vecs, 4, 'cipher_text'))
    print(vecs_to_c(vecs, 5, 'auth_tag'))

generate(*sys.argv[1:])
