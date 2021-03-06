#!/usr/bin/python
# -*- coding: utf-8 -*-

copyright = u"""\
Copyright (C) 2010, Fundacio Barcelona Media.
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.
Licence GPLv2+: GNU GPL version 2 or later <http://gnu.org/licenses/gpl2.html>

Written by Angelo Scorza and David Garcia Garzon.
"""

description = """\
This script takes a CLAM network xml file and generates
C++ code to build such a network. The resulting network
should be equivalent to load the xml with CLAM storage
methods.

The generated code defines a class named CLAM::NetworkDefinition_<suffix>
where suffix is either the second parameter classNameSuffix or
the basename of the networkfile if the suffix is not provided.
Such class has a method called setUp that fills a network with
the proper processings, description, connections and so on.
"""

epilog = """\
Bear in mind the following limitations:
- Text items are not implemented
- Just configurations with simple objects are supported (no subelements, no attributes).
- Processing names may colide when having non alpha numeric chars in the same positions.
- Spaces are stripped by sax (there might be a problem when parsing xmlentities surrounded by spaces).
"""

import xml.sax
import cStringIO
import sys
import re
import os



class NetworkHandler(xml.sax.handler.ContentHandler) :
	def __init__(self) :
		self._result = ""
		self._currentPath = []
		self._connectionIn = None
		self._connectionOut = None
		self._processingName = None

	def network(self, clamVersion="1.4.1", id="Unnamed") :
		self._result += "\t\tnetwork.SetName(\"" + id + "\");\n"
#		print "Network:", id, "version", clamVersion

	def description(self) : pass

	def _formatId(self, id) : return re.sub('\W', '_',id.strip())

	def processing(self, id, type, position="(10,10)", size="(10,10)") :
		formattedId = self._formatId(id)
		self._result += '\t\tProcessing & _%s = network.AddProcessing("%s", "%s");\n'%(
			formattedId, id, type)
		self._result += '\t\tProcessingConfig * _%s_config = dynamic_cast<ProcessingConfig *>( _%s.GetConfig().DeepCopy()) ;\n'%(
			formattedId, formattedId)
		self._result += '\t\tloadConfig(*_%s_config,\n'%formattedId
		self._result += '\t\t\t"<Configuration>\\n"\n'
		self._processingName = id

	def end_processing(self) :
		formattedId = self._formatId(self._processingName)
		self._result += '\t\t\t"</Configuration>\\n");\n'
		self._result += '\t\t_%s.Configure(*_%s_config);\n' % (
			formattedId, formattedId)
		self._result += '\t\tdelete _%s_config;\n\n' % (formattedId)
		self._processingName = None

	def end_control_connection(self) :
		self._result += "\t\tnetwork.ConnectControls(\"%s\", \"%s\");\n"%(
			self._connectionOut,
			self._connectionIn,
			)
		self._connectionOut = None
		self._connectionIn = None

	def end_port_connection(self) :
		self._result += "\t\tnetwork.ConnectPorts(\"%s\", \"%s\");\n"%(
			self._connectionOut,
			self._connectionIn,
			)
		self._connectionOut = None
		self._connectionIn = None

	def startElement(self, name, attrs) :
#		print "Start:", name
		self._currentPath.append(name)
		if not self._processingName :
			try : getattr(self,name)(**attrs)
			except AttributeError : pass
			return
		self._content = ""

	def endElement(self, name) :
		if self._processingName and name != "processing" :
			self._result += '\t\t\t\t"<%s>%s</%s>\\n"\n' % (
				name, xml.sax.saxutils.escape(self._content.strip()), name)
		self._currentPath.pop()
		try : getattr(self, "end_"+name)()
		except AttributeError : pass

	def characters(self, content) :
		strippedContent = content.strip()
		if not strippedContent: return
		if self._currentPath[-1] == "in" :
			self._connectionIn = strippedContent 
		if self._currentPath[-1] == "out" :
			self._connectionOut = strippedContent
		if self._processingName :
			self._content+=content

	def getCode(self, name="NetworkFiller") :
		return ("""\
#include <CLAM/Network.hxx>
#include <sstream>
#include <CLAM/XMLStorage.hxx>

namespace CLAM {

class %(name)s
{
public:
	%(name)s(){}

	void loadConfig(ProcessingConfig & config, const std::string & xmlContent)
	{
		std::istringstream os(xmlContent);
		CLAM::XMLStorage::Restore(config, os);
	}

	void setUp(Network &network)
	{
""" + 
	self._result + 
"""\
	}
};
}""")%dict(name=name)

	def setupCode(self) :
		return self._result


if __name__=="__main__" and "--test" in sys.argv :
	sys.argv.remove("--test")
	import unittest
	class TestClamNetwork2Code(unittest.TestCase) :
		def parse(self, xmlContent, handler) :
			saxparser = xml.sax.make_parser()
			saxparser.setContentHandler(handler)
			saxparser.parse(cStringIO.StringIO(xmlContent))
			
		def test_formatId1(self) :
			n = NetworkHandler()
			mangled = n._formatId("Audio_Processing")
			self.assertEquals("Audio_Processing", mangled)

		def test_formatId2(self) :
			n = NetworkHandler()
			mangled = n._formatId("Audio(Processing.01)")
			self.assertEquals("Audio_Processing_01_", mangled)

		def test_emptyNetwork(self) :
			n = NetworkHandler()
			xml = """\
<?xml version="1.0" encoding="UTF-8" standalone="no" ?>
<network clamVersion="1.4.0" id="Unnamed">
</network>
"""
			self.parse(xml,n)
			self.assertEquals(u"""\
		network.SetName("Unnamed");
""",n.setupCode())
		def test_portConnection(self) :
			n = NetworkHandler()
			xml = """\
<?xml version="1.0" encoding="UTF-8" standalone="no" ?>
<network clamVersion="1.4.0" id="Unnamed">
  <port_connection>
    <out>proc1.outport</out>
    <in>proc2.inport</in>
  </port_connection>

</network>
"""
			self.parse(xml,n)
			self.assertEquals(u"""\
		network.SetName("Unnamed");
		network.ConnectPorts("proc1.outport", "proc2.inport");
""",n.setupCode())

		def test_controlConnection(self) :
			n = NetworkHandler()
			xml = """\
<?xml version="1.0" encoding="UTF-8" standalone="no" ?>
<network clamVersion="1.4.0" id="Unnamed">
  <control_connection>
    <out>proc1.outport</out>
    <in>proc2.inport</in>
  </control_connection>

</network>
"""
			self.parse(xml,n)
			self.assertEquals(u"""\
		network.SetName("Unnamed");
		network.ConnectControls("proc1.outport", "proc2.inport");
""",n.setupCode())

		def test_processing(self) :
			n = NetworkHandler()
			xml = """\
<?xml version="1.0" encoding="UTF-8" standalone="no" ?>
<network clamVersion="1.4.0" id="Unnamed">
  <processing id="myid" position="10,15" size="20,30" type="MyType">
  </processing>
</network>
"""
			self.parse(xml,n)
			self.assertEquals("""\
		network.SetName("Unnamed");
		Processing & _myid = network.AddProcessing("myid", "MyType");
		ProcessingConfig * _myid_config = dynamic_cast<ProcessingConfig *>( _myid.GetConfig().DeepCopy()) ;
		loadConfig(*_myid_config,
			"<Configuration>\\n"
			"</Configuration>\\n");
		_myid.Configure(*_myid_config);
		delete _myid_config;

""",n.setupCode())


		def test_processing_withConfigParameters(self) :
			n = NetworkHandler()
			xml = """\
<?xml version="1.0" encoding="UTF-8" standalone="no" ?>
<network clamVersion="1.4.0" id="Unnamed">
  <processing id="myid" position="10,15" size="20,30" type="MyType">
    <parameter>a value</parameter>
  </processing>
</network>
"""
			self.parse(xml,n)
			self.assertEquals("""\
		network.SetName("Unnamed");
		Processing & _myid = network.AddProcessing("myid", "MyType");
		ProcessingConfig * _myid_config = dynamic_cast<ProcessingConfig *>( _myid.GetConfig().DeepCopy()) ;
		loadConfig(*_myid_config,
			"<Configuration>\\n"
				"<parameter>a value</parameter>\\n"
			"</Configuration>\\n");
		_myid.Configure(*_myid_config);
		delete _myid_config;

""",n.setupCode())
			
		def test_processingConfig_withEscaping(self) :
			n = NetworkHandler()
			xml = """\
<?xml version="1.0" encoding="UTF-8" standalone="no" ?>
<network clamVersion="1.4.0" id="Unnamed">
  <processing id="myid" position="10,15" size="20,30" type="MyType">
    <parameter>&gt;&lt;&amp;</parameter>
  </processing>
</network>
"""
			self.parse(xml,n)
			self.assertEquals("""\
		network.SetName("Unnamed");
		Processing & _myid = network.AddProcessing("myid", "MyType");
		ProcessingConfig * _myid_config = dynamic_cast<ProcessingConfig *>( _myid.GetConfig().DeepCopy()) ;
		loadConfig(*_myid_config,
			"<Configuration>\\n"
				"<parameter>&gt;&lt;&amp;</parameter>\\n"
			"</Configuration>\\n");
		_myid.Configure(*_myid_config);
		delete _myid_config;

""",n.setupCode())
    
		def test_classGenerated(self) :
			network = NetworkHandler()
			saxparser = xml.sax.make_parser()
			saxparser.setContentHandler(network)

			exampleNetwork = """\
<?xml version="1.0" encoding="UTF-8" standalone="no" ?>
<network clamVersion="1.4.0" id="Unnamed">
</network>
"""
			datasource = cStringIO.StringIO(exampleNetwork)
			saxparser.parse(datasource)

			result= network.getCode()
			n = NetworkHandler()
			
			self.assertEquals("""\
#include <CLAM/Network.hxx>
#include <sstream>
#include <CLAM/XMLStorage.hxx>

namespace CLAM {

class NetworkFiller
{
public:
	NetworkFiller(){}

	void loadConfig(ProcessingConfig & config, const std::string & xmlContent)
	{
		std::istringstream os(xmlContent);
		CLAM::XMLStorage::Restore(config, os);
	}

	void setUp(Network &network)
	{
		network.SetName("Unnamed");
	}
};
}""", result)


		def test_classGeneratedUsingSpecificName(self) :
			network = NetworkHandler()
			saxparser = xml.sax.make_parser()
			saxparser.setContentHandler(network)

			exampleNetwork = """\
<?xml version="1.0" encoding="UTF-8" standalone="no" ?>
<network clamVersion="1.4.0" id="Unnamed">
</network>
"""
			datasource = cStringIO.StringIO(exampleNetwork)
			saxparser.parse(datasource)

			result= network.getCode("MyClassName")
			n = NetworkHandler()
			
			self.assertEquals("""\
#include <CLAM/Network.hxx>
#include <sstream>
#include <CLAM/XMLStorage.hxx>

namespace CLAM {

class MyClassName
{
public:
	MyClassName(){}

	void loadConfig(ProcessingConfig & config, const std::string & xmlContent)
	{
		std::istringstream os(xmlContent);
		CLAM::XMLStorage::Restore(config, os);
	}

	void setUp(Network &network)
	{
		network.SetName("Unnamed");
	}
};
}""", result)
		# End of TestClamNetwork2Code class


	sys.exit(unittest.main())

if __name__ == "__main__" :
	if "--test" in sys.argv :
		_test()

	from optparse import OptionParser
	parser = OptionParser(
		usage= u"usage: %prog networkFile [classNameSuffix]",
		version=u"%prog 1.4\n"+copyright,
		description=description,
		epilog=epilog
		)
	parser.add_option("--test", dest='test', action="store_true",
			help="Runs script tests")
	options, args = parser.parse_args()

	if len(args) not in (1,2) :
		parser.print_help()
		sys.exit(-1)

	networkfile = args[0]

	print "// File generated by %s from CLAM network '%s' "%(sys.argv[0], networkfile)

	exampleNetwork = file(networkfile).read()
	network = NetworkHandler()
	saxparser = xml.sax.make_parser()
	saxparser.setContentHandler(network)

	datasource = cStringIO.StringIO(exampleNetwork)
	saxparser.parse(datasource)

	if len(args)>1 :
		nameClass = "NetworkDefinition_"+args[1]
	else :
		nameClass = "NetworkDefinition_"+os.path.splitext(os.path.basename(networkfile))[0]
	print network.getCode(nameClass)
	open("original.clamnetwork","w").write(exampleNetwork)



