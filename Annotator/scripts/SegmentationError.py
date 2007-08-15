#!/usr/bin/python

# Copyright (c) 2001-2006 MUSIC TECHNOLOGY GROUP (MTG)
#                         UNIVERSITAT POMPEU FABRA
#
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA


from Pool import *
import sys
from xml.dom.ext.reader import Sax2

def extractAttribute(pool, scope, attribute) :	
	return pool.SelectAttributes(scope,attribute)[0].firstChild.data.split()

def extractEnumAttribute(pool, scope, attribute) :	
	return [e.firstChild.data for e in pool.SelectAttributes(scope,attribute)[0].childNodes if e.nodeType == e.ELEMENT_NODE  ]

computedFile = '../example-data/SongsTest/Debaser-CoffeeSmell.mp3.chords'
trueFile = '../example-data/SongsTest/Debaser-WoodenHouse.mp3.chords'

computedPool = Pool(computedFile)
truePool = Pool(trueFile)

computedSegmentationString = extractAttribute(computedPool, "Song", "Chords_Harte")
computedSegmentation = [ float(a) for a in extractAttribute(computedPool, 'Song','Chords_Harte') ]
computedRoots = extractEnumAttribute(computedPool, "ExtractedChord", "Root")
computedModes = extractEnumAttribute(computedPool, "ExtractedChord", "Mode")
computedChords = []
for i in range(0,len(computedRoots)):
	computedChords.append(computedRoots[i]+' '+computedModes[i])

trueSegmentationString = extractAttribute(truePool, "Song", "Chords_Harte")
trueSegmentation = [ float(a) for a in extractAttribute(truePool, 'Song','Chords_Harte') ]
trueRoots = extractEnumAttribute(truePool, "ExtractedChord", "Root")
trueModes = extractEnumAttribute(truePool, "ExtractedChord", "Mode")
trueChords = []
for i in range(0,len(trueRoots)):
	trueChords.append(trueRoots[i]+' '+trueModes[i])
