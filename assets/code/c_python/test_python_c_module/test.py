#!coding=utf-8
#!/bin/env python

import cmathapi

def test(a, b):
    """hello"""
    c = cmathapi.add(a, b)
    print("{0} + {1} = {2}".format(a, b, c))
    d = cmathapi.sub(a, b)
    print("{0} - {1} = {2}".format(a, b, d))

if __name__ == '__main__':
    test(5, 7)
