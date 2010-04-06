#!/usr/bin/env python 
# -*- coding: utf-8 -*-

import os

os.chdir("@CMAKE_BINARY_DIR@/py/")
 
def header(msg):
    print 
    print msg
    print "============================="
    
header("Align test")
import align
align.test()

header("Format test")
import format_quick
format_quick.test()

header("Language quick test")
import lang_quick
lang_quick.test()

header("Language regression test")
import lang_regression
lang_regression.test()

header("Image formats test")
import image_formats
image_formats.test()

#header("Summary:")
#print "Test total: %d, passed: %d, failed: %d" % ()
