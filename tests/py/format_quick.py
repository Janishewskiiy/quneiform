#!@PYTHON_EXECUTABLE@
# -*- coding: utf-8 -*-

import sys
import cf

FORMATS = ('text',
    'textdebug',
    'smarttext',
    'html',
    'hocr',
    'odf',
    'summary',
    'native',
    'native-xml',
    'djvutxt',
    'djvuxml')

def test():
    fmtTest = cf.Tester('lang.diftest')
    
    for key in FORMATS:
        fmtTest.setFormat(key)
        fmtTest.cuneiformTest(fmtTest.makeFullImageName('german.bmp'))
    
    if fmtTest.passed():
        return True
    else:
        if __name__ == '__main__':
            sys.exit(1)
        else:
            return False
    
if __name__ == "__main__":
    test()


