#!/usr/bin/python


import sys
import os
from optparse import OptionParser

parser = OptionParser(
	usage="%prog [options] audiofile1 audiofile2 ...",
	version="%prog 1.0")
parser.add_option("-s","--print-schema",
		action="store_true",
		default=False,
		dest="printSchema",
		help="Outputs the description schema for the extractor"
	)
parser.add_option("-f",# "--suffix",
		default=".pool",
		dest="suffix",
		help="Appends SUFFIX to the generated descriptors file (default: '%default')"
	)
(options, args) = parser.parse_args()




if options.printSchema :
	print """<?xml version='1.0' encoding='UTF-8' standalone='no' ?>
<DescriptionScheme>

 <Uri>descriptionScheme:www.iua.upf.edu:clam:OnsetExtraction</Uri>

 <Attributes>
   <Attribute name='Onsets' scope='Song' type='Segmentation'>
     <ChildScope></ChildScope>
     <SegmentationPolicy>Unsized</SegmentationPolicy>
   </Attribute>
 </Attributes>

</DescriptionScheme>

"""
	sys.exit()

dataTemplate = """<?xml version='1.0' encoding='UTF-8' standalone='no' ?>
<Description>

  <ScopePool name="Song" size="1">
    <AttributePool name="Onsets" size="%i">
		%s
    </AttributePool>
  </ScopePool>

</Description>
"""


for audiofile in args:
	f=os.popen('./OnsetExtractor %s'%audiofile,'r')
	onsets = [ onset for onset in f ]
	target = open(audiofile+options.suffix,'w')
	print >> target, dataTemplate%(len(onsets), " ".join(onsets))
	target.close()




