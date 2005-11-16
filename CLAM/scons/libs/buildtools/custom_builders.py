import os
import sys

def generate_so_name( target, source, env ) :
	source_dir = os.path.dirname( str(source[0]) )
	source_file = os.path.basename( str(source[0]) )
	cwd = os.getcwd()
	os.chdir( source_dir )
	if sys.platform == 'linux2' :
		os.system( "ldconfig -n ." )
	os.chdir(cwd)

def generate_so_name_message( target, source, env ) :
	return "generating %s with ldconfig -n"%target[0]

def generate_linker_name( target, source, env ) :
	source_dir = os.path.dirname( str(source[0]) )
	source_file = os.path.basename( str(source[0]) )
	target_file = os.path.basename( str(target[0]) )
	target_dir = os.path.dirname( str(target[0]) )
	cwd = os.getcwd()
	os.chdir( target_dir )
	os.system( "ln -sf %s %s"%(source_file,target_file) )
	os.chdir(cwd)

def generate_linker_name_message( target, source, env ) :
	return "creating soft link to %s with linker name %s"%(source[0], target[0])

