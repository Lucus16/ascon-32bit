#!/usr/bin/env python3

import sys
import re

alldata = bytearray()
def data_index(datum):
    global alldata
    datum = bytes.fromhex(datum)
    index = alldata.find(datum)
    if index < 0:
        index = len(alldata)
        alldata += datum
    return index

def chunks(l, n):
    return [l[i:i + n] for i in range(0, len(l), n)]

def convert_test(lines):
    return dict([x.strip() for x in line.split('=')]
            for line in lines if '=' in line)

def hex_to_c(data):
    lines = chunks(data, 8)
    lines = [' '.join('0x{:02x},'.format(byte) for byte in data) for data in lines]
    lines = ''.join('    ' + line + '\n' for line in lines)
    return '{\n' + lines + '}\n'

def fixed_sizes(tests):
    return [field for field in tests[0]
            if len({len(test[field]) for test in tests}) == 1]

def test_struct(tests, fixed):
    struct = '#define TESTCASE_COUNT {}\n'.format(len(tests))
    for field in fixed:
        struct += '#define {}_SIZE {}\n'.format(field.upper(),
                len(tests[0][field]) // 2)
    struct += '\nstruct testcase {\n'
    for field in tests[0]:
        struct += '    const unsigned char     *{};\n'.format(field)
        if field not in fixed:
            struct += '    const unsigned long long {}_size;\n'.format(field)
    struct += '};\n'
    return struct

def show_test(test, fixed):
    struct = '{\n'
    for field, value in test.items():
        struct += '        .{:7} = test_data + {},\n'.format(field, data_index(value))
        if field not in fixed:
            struct += '        .{}_size = {},\n'.format(field, len(value) // 2)
    struct += '    },'
    return struct

def show_test_data():
    result = 'const unsigned char test_data[] = {}'.format(hex_to_c(alldata))
    return result + ';'

def show_tests(tests, fixed):
    result = 'const struct testcase testcases[] = {\n    '
    result += ' '.join(show_test(test, fixed) for test in tests)
    result += '\n};\n'
    return result

def generate(input_file):
    with open(input_file, 'r') as f:
        lines = f.readlines()

    test_starts = [i for i, line in enumerate(lines) if line.startswith('####')]
    test_ends = test_starts[1:] + [len(lines)]
    tests = [tuple(lines[i + 1:j]) for i, j in zip(test_starts, test_ends)]
    tests = list(dict.fromkeys(tests))
    tests = [convert_test(test) for test in tests]
    assert len(set(frozenset(d.keys()) for d in tests)) == 1

    fixed = fixed_sizes(tests)
    print(test_struct(tests, fixed))
    tests = show_tests(tests, fixed)
    print(show_test_data())
    print(tests)

generate(*sys.argv[1:])
