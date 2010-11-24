#!/usr/bin/python

import clam_lv2_generator
import sys, os
import unittest
sys.path.append('../../../scripts/')
sys.path.append('../../../../../clam-test-data/b2b/lv2_plugin/')
from audiob2b import runBack2BackProgram
from audiob2b import run

if __name__ == '__main__':

	data_path="../../../../../clam-test-data/b2b/lv2_plugin/"


	back2BackTests = [
		("simple_ttl",
		"./clam_lv2_generator.py --ttl -u %(uri)s %(network)s > %(target)s" % dict(uri="http://clam-project.org/examples/lv2", network="othercable.clamnetwork", target="output.ttl")
		, [
			"output.ttl",
		]),
	]

	runBack2BackProgram(data_path, sys.argv, back2BackTests)
