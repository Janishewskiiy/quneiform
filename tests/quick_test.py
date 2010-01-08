#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
import sys
import subprocess

CMD = "./cuneiform -l %s -o %s -f text '../images/lang.diftest/%s.bmp'"
DATA = {'bul' : 'Bulgarian',
        'hrv' : 'Croatian',
        'cze' : 'Czech',
        'dan' : 'Danish',
        'dut' : 'Dutch',
        'eng' : 'English2cl',
        'est' : 'Estonian',
        'fra' : 'French',
        'ger' : 'German', 
        'hun' : 'Hungarian',
        'ita' : 'Italian',
        'lav' : 'Latvian',
        'lit' : 'Lithuanian',
        'pol' : 'Polish',
        'por' : 'Portuguese',
        'rum' : 'Romanian',
        'rus' : 'Russian',
        'slo' : 'Slovenian',
        'spa' : 'Spanish',
        'swe' : 'Swedish',
        'ukr' : 'Ukrainian'                 
         }
OUTPUT = "tmp.txt"
CMD += " 2>/dev/null"

os.environ['CF_DATADIR'] = "../datafiles"
tests_passed = 0
tests_failed = 0

for key, v in sorted(DATA.iteritems()):
    cmd = CMD % (key, OUTPUT, v.lower())
    # print cmd
    retcode = subprocess.call(cmd, shell=True)
    if retcode > 0:
        print "%s failed" % v
	tests_failed = tests_failed + 1
        print cmd
        continue
        #sys.exit(1)
    elif retcode < 0:
        sys.exit(1)
        
    tests_passed = tests_passed + 1

    if os.path.getsize(OUTPUT) == 0:
        print "%s failed. not output" % v
        sys.exit(1)
        
    if retcode == 0:
        print "%-15s Ok" % v
        
print "Tests passed: %d, failed: %d\n" % (tests_passed, tests_failed)

os.unlink(OUTPUT)    
       
if tests_failed > 0:
    sys.exit(1)

