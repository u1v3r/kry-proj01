#!/usr/bin/env python
# -*- coding: utf-8 -*-
 
 
from difflib import SequenceMatcher
import subprocess
import glob
import os
 
 
def _wrap_with(code):
    def inner(text, bold=False):
        c = code
        if bold:
            c = "1;%s" % c
        return "\033[%sm%s\033[0m" % (c, text)
    return inner
 
 
red = _wrap_with('31')
green = _wrap_with('32')
yellow = _wrap_with('33')
blue = _wrap_with('34')
magenta = _wrap_with('35')
cyan = _wrap_with('36')
white = _wrap_with('37')
 
 
def colorize(key, correct_key):
    s = SequenceMatcher(a=key, b=correct_key)
    diff = list(key)
    for tag, i1, i2, j1, j2 in s.get_opcodes():
        if tag == 'replace':
            diff[i1:i2] = map(yellow, diff[i1:i2])
 
        elif tag == 'insert':
            if (i1 == 0):
                diff[i1] = green(correct_key[j1:j2]) + diff[i1]
            else:
                diff[i1 - 1] += green(correct_key[j1:j2])
 
        elif tag == 'delete':
            diff[i1:i2] = map(red, diff[i1:i2])
 
    return ''.join(diff)
 
 
def main():
    passed = 0
    failed = 0
    total = 0
 
    for name in glob.iglob('tests/*.cip'):
        with open(name, 'rt') as cipherfile:
            correct_key = os.path.basename(name)[:-4]
            output = subprocess.check_output(['./kry'], stdin=cipherfile).strip()
            friedman, kasiski, length, key = output.split(';')
 
            if (key == correct_key):
                print green("[X]"), " %s == %s (%s)" % (correct_key, key, output)
                passed += 1
 
            else:
                print red("[!]"), " %s != %s (%s)" % (correct_key, colorize(key, correct_key), output)
                failed += 1
 
            total += 1
 
    print "\n"
 
    if failed == 0:
        print green("COOL. Passed all %d tests" % (total,))
 
    elif passed == 0:
        print red("SCHEIßE. Failed all %d tests" % (total,))
 
    else:
        print yellow("SO-SO. Passed %d/%d tests" % (passed, total))
 
 
 
if __name__ == '__main__':
    main()
  
