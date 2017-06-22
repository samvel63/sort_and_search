#!/usr/bin/python

from random import *
from string import ascii_letters

template = '{key} {data}\n'

with open('input2', 'w') as f:
    for i in range(1, 100000):
        if i % 100000 == 0:
            print i
        f.write(template.format(**{
            'key': ''.join(choice(ascii_letters) for _ in range(1)),
            'data': ''.join(choice(ascii_letters) for _ in range(1))
}))
