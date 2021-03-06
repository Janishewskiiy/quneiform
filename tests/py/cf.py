#!@PYTHON_EXECUTABLE@
# -*- coding: utf-8 -*-

import os, sys
import zipfile
import platform
from subprocess import *
from xml.dom import minidom
import re
import glob

os.environ['CF_DATADIR'] = "@CMAKE_SOURCE_DIR@/datafiles"
os.environ['PATH'] = os.environ['PATH'] + ":@CMAKE_BINARY_DIR@"

# globals exe's and paths
CUNEIFORM = "@CUNEIFORM_EXE@"
CUNEIFORM_PROCESS = "@CUNEIFORM_PROCESS@"
ACCSUM = "@CMAKE_BINARY_DIR@/cf_accsum"
ACCURACY = "@CMAKE_BINARY_DIR@/cf_accuracy"
IMAGEDIR = "@CMAKE_SOURCE_DIR@/images"
DIFFOPTS = '@CF_DIFFOPTS@'

class bcolor:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OK = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    END = '\033[0m'

    def disable(self):
        self.HEADER = ''
        self.OKBLUE = ''
        self.OK = ''
        self.WARNING = ''
        self.FAIL = ''
        self.END = ''

    @staticmethod
    def clear():
        bcolor.HEADER = ''
        bcolor.OKBLUE = ''
        bcolor.OK = ''
        bcolor.WARNING = ''
        bcolor.FAIL = ''
        bcolor.END = ''

if platform.system() == 'Windows':
    bcolor.clear()


class Tester:
    _imagedir = ''
    _images = []
    _version = None
    _tests_passed = 0
    _tests_failed = 0
    _language = None
    _output = None
    _output_image_dir = None
    _format = None
    _line_breaks = False
    _sample_ext = None
    _args = []
    _turn = 0
    _pagenum = 0
    
    def __init__(self, imagedir=''):
        self._imagedir = os.path.join(IMAGEDIR, imagedir)
        
    def __del__(self):
        self.printTestStat()
        
    def accuracy(self, img):
        output = self.makeOutput(img)
        report_str = Popen([ACCURACY, self.makeSampleName(img), output], stdout=PIPE).communicate()[0]
        
        report_file = open("%s.acc" % output, 'w')
        report_file.write(report_str)
        
        lines = report_str.split('\n')
        if len(lines) > 4:
            print lines[3]  
            print lines[4]

    def addArg(self, arg):
        self._args.append(arg)

    def addFail(self):
        self._tests_failed += 1

    def addImage(self, img):
        self._images.append(img)
        
    ''' adds image to tester '''
    def addImages(self, files):
        self._images += files    
      
    ''' removes all images from tester '''  
    def clear(self):
        self._images = []
        
    def cuneiform(self, args, **kwargs):
        cmd = [CUNEIFORM] + args
        retcode = call(cmd, **kwargs)
        if retcode != 0:
            print ' '.join(cmd)
        return retcode

    def cuneiformProcess(self, args, **kwargs):
        cmd = [CUNEIFORM_PROCESS] + args
        retcode = call(cmd, **kwargs)
        if retcode != 0:
            print ' '.join(cmd)
        return retcode
    
    def cuneiformTest(self, img, process=False):
        if not os.path.exists(img):
            self.printError("image file not exists: %s\n" % img)
            return False
            
        if self._format is None:
            self.printError('output file not specified and output format is not set')
            return  False
            
        self._output = self.makeOutput(img)
        
        if process:
            retcode = self.cuneiformProcess(self.makeArgs(img), stdout=PIPE, stderr=PIPE)
        else:
            retcode = self.cuneiform(self.makeArgs(img), stdout=PIPE, stderr=PIPE)

        if retcode != 0:
            self.printFail(img, "")
            self._tests_failed += 1
            return False
            
        if os.path.getsize(self._output) == 0:
            self.printFail(img, "No output")
            self._tests_failed += 1
            return False
        else:
            self.printOk(img)
            self._tests_passed += 1
        
        return True
    
    def diff(self, first, second, **kwargs):
        cmd = ['diff', DIFFOPTS, first, second]
        #print cmd
        return call(cmd, **kwargs)

    def diffNative(self, first, second, **kwargs):
        self.fileReplace(second, r'serialization::archive \d+', 'serialization::archive 9')
        return self.diff(first, second, **kwargs)
    
    def diffOdf(self, first, second, **kwargs):        
        first_odf = zipfile.ZipFile(first)
        second_odf = zipfile.ZipFile(second)
        
        first_content = first_odf.read('content.xml')
        second_content = second_odf.read('content.xml')
        
        if first_content == second_content:
            return 0
        else:
            first_xml = os.path.basename(first) + '.xml'
            f = open(first_xml,'w')
            f.write(first_content)
            f.close()
                
            second_xml = second + '.xml'
            f = open(second_xml, 'w')
            f.write(second_content)
            f.close()        
            return self.diff(first_xml, second_xml, **kwargs)

    def diffXml(self, xml1, xml2, **kwargs):
        first_xml = open(xml1, 'r')
        second_xml = open(xml2, 'r')
        dom1 = minidom.parseString(first_xml.read())
        dom2 = minidom.parseString(second_xml.read())

        self.unsetBoostVersion(dom1)
        self.unsetBoostVersion(dom2)

        first_xml.close()
        second_xml.close()

        res = self.isEqualElement(dom1.documentElement, dom2.documentElement)
        if res == True:
            return 0
        else:
            self.diff(xml1, xml2, **kwargs)
            return 1

    def diffTestContent(self, img, content):
        if not self.cuneiformTest(img):
            return False

        self._format = 'text'

        if not os.path.exists(self._output):
            self.printFail(img, "\n(output not exists: %s)" % self._output)
            self._tests_failed += 1
            return False

        second_file = open(self._output, 'r')
        second_content = second_file.read().strip()
        second_file.close()

        os.unlink(self._output)

        if second_content == content:
            self._tests_passed += 1
            return True
        else:
            print second_content, '!=', content
            self._tests_failed += 1
            self.printFail(img, '(not equal)')
            return False
    
    def diffTest(self, img):
        if not self.cuneiformTest(img):
            return False
        
        if self._format == 'odf':
            self.setSampleExt('odt')
        elif self._sample_ext == 'odt':
            self._sample_ext = None
        
        sample_name = self.makeSampleName(img)
        if not os.path.exists(sample_name):
            self.printFail(img, "\n(sample output not exists: %s)" % sample_name)
            self._tests_failed += 1
            return False
        
        diff_name = self.makeDiffName()
        diff_output = open(diff_name, 'w')
        
        if self._format == 'odf':
            retcode = self.diffOdf(sample_name, self._output, stdout=diff_output)
        elif self._format == 'native':
            retcode = self.diffNative(sample_name, self._output, stdout=diff_output)
        elif self._format == 'native-xml':
            retcode = self.diffXml(sample_name, self._output, stdout=diff_output)
        else:  
            retcode = self.diff(sample_name, self._output, stdout=diff_output)
            
        if retcode != 0:
            diff_output.close()
            self.printFail(img, '(difference found)')
            print ' '.join([CUNEIFORM] + self.makeArgs(img))
            self._tests_failed += 1
            return False
        else:
            self._tests_passed += 1
            diff_output.close()
            os.unlink(self._output)
            os.unlink(diff_name)
            return True

    def fileReplace(self, filename, pattern, to):
        f = open(filename, 'r')
        new_f = re.sub(pattern, to, f.read())
        f.close()
        f = open(filename, 'w')
        f.write(new_f)
        f.close()

    def isEqualXML(a, b):
        da, db = minidom.parseString(a), minidom.parseString(b)
        return isEqualElement(da.documentElement, db.documentElement)

    def isEqualElement(self, a, b):
        if a.tagName != b.tagName:
            print "          [XML] Different tag names: %s %s" % (a.tagName, b.tagName)
            return False

        if sorted(a.attributes.items()) != sorted(b.attributes.items()):
            print "          [XML] Different attributes..."
            return False

        if len(a.childNodes) != len(b.childNodes):
            return False

        for ac, bc in zip(a.childNodes, b.childNodes):
            if ac.nodeType != bc.nodeType:
                return False
            if ac.nodeType == ac.TEXT_NODE and ac.data != bc.data:
                print "        [XML] Different values: '%s' and '%s' in tag <%s>" % (ac.data, bc.data, a.tagName)
                return False
            if ac.nodeType == ac.ELEMENT_NODE and not self.isEqualElement(ac, bc):
                return False
        return True
            
    def makeArgs(self, img):
        args = []
        args.extend(self._args)

        if self._language is not None:
            args += ['--language', self._language]
        if self._output is not None:
            args += ['--output', self._output]
        if self._format is not None:
            args += ['--format', self._format]
        if self._output_image_dir is not None:
            args += ['--output-image-dir', self._output_image_dir]
            
        if self._line_breaks:
            args.append('--preserve-line-breaks')

        if self._turn:
            args += ['--turn', str(self._turn)]

        if self._pagenum > 0:
            args += ['--page-number', str(self._pagenum)]

        args.append('--no-bom')
        args.append('--no-meta-generator')
        args.append('--test-output')
            
        args.append(img)
            
        return args
    
    def makeDiffName(self):
        return "%s.diff" % self._output
    
    def makeFullImageName(self, image):
        return os.path.join(IMAGEDIR, self._imagedir, image)
    
    def makeOutput(self, image):
        return '%s.%s.%s' % (os.path.splitext(os.path.basename(image))[0], self.version(), self._format)
    
    def makeSampleName(self, img):
        path = os.path.split(img)
        name = os.path.join(path[0], "%s.sample." % os.path.splitext(path[1])[0]) 
        if self._sample_ext is not None:
            name += self._sample_ext
        else:
            name += self._format
        
        return name

    def output(self):
        return self._output
    
    def passed(self):
        return self._tests_failed == 0
    
    def printError(self, msg):
        print "%s Error: %s %s" % (bcolor.FAIL, bcolor.END, msg)
    
    def printFail(self, img, msg):
        print "%-35s %-15s %s FAIL %s %s" % (os.path.basename(img), 'OCR(%s)' % self._format, bcolor.FAIL, bcolor.END, msg)
    
    def printOk(self, img):
        print "%-35s %-15s %s Ok %s" % (os.path.basename(img), 'OCR(%s)' % self._format, bcolor.OK, bcolor.END)
    
    def printTestStat(self):
        print "Tests total: %d, passed: %d, failed: %d" % (self.total(), self._tests_passed, self._tests_failed)
            
    def removeArg(self, arg):
        self._args.remove(arg)

    def runDiffTest(self):
        for img in self._images:
            self.diffTest(img)
            
    def setFormat(self, format):
        self._format = format
        
    def setImageOutputDir(self, path):
        self._output_image_dir = path
            
    def setLanguage(self, lang):
        self._language = lang
        
    def setLineBreaks(self, value):
        if value:
            self._line_breaks = True
        else:
            self._line_breaks = False
            
    def setSampleExt(self, ext):
        self._sample_ext = ext

    def setTurn(self, angle):
        self._turn = angle

    def setPageNumber(self, number):
        self._pagenum = number
        
    def total(self):
        return self._tests_failed + self._tests_passed

    def unsetBoostVersion(self, dom):
        tag = dom.getElementsByTagName('boost_serialization')[0]
        tag.removeAttribute('version')
                
    ''' returns cuneiform version ''' 
    def version(self):
        if self._version is None:
            self._version = Popen([CUNEIFORM, '-V'], stdout=PIPE).communicate()[0].split('\n')[0].split()[-1]
        
        return self._version

    def removeOutput(self, images, formats):
        for img in images:
            for format in formats:
                fname = "%s.*.%s" % (os.path.splitext(os.path.basename(self.makeFullImageName(img)))[0], format)

                for f in glob.glob(fname):
                    os.unlink(f)
                    print "File '%s' removed" % (f)

                fname += ".diff"
                for f in glob.glob(fname):
                    os.unlink(f)
                    print "File '%s' removed" % (f)

