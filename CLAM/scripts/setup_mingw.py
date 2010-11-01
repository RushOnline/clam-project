#!/usr/bin/python
import subprocess
import sys
import os
import glob

# TODO:
# libid3 lacks a pkg-config file
# libsndfile lacks dependencies: sqlite flac ogg vorbis


sandbox = os.path.expanduser("~/CajitasDeArena/mingw2")
target = "i586-mingw32msvc"
sfmirror = "kent.dl.sourceforge.net"
prefix = os.path.join(sandbox,"local")

def die(message) :
	print >> sys.stderr, "\033[31m%s\033[0m"%message
	sys.exit(-1)
def warning(message) :
	print >> sys.stderr, "\033[33m%s\033[0m"%message

def run(command, message=None) :
	if not message : message = "Running: " + command
	print "\033[32m== %s\033[0m"%(message)
	subprocess.call(command, shell=True) and die("Failed")

def output(command) :
	return subprocess.Popen(command, shell=True, stdout=subprocess.PIPE).communicate()[0] 

def applyPatch(directory, patch, level=0) :
	fullpatchpath = scriptRelative(patch)
	run("patch -d '%s' -p%i < '%s' "%(directory, level, fullpatchpath),
		"Applying patch %s"%os.path.basename(patch)
		)

def download(uri, filename=None) :
	if filename:
		outputOption = " -O '%s'"% os.path.join(sandbox,"downloads",filename)
	else :
		outputOption = " -P '%s'"% os.path.join(sandbox,"downloads")
	run("wget -nv --progress=dot -c %s '%s'" % (outputOption, uri),
		"Downloading %s"%uri
	)

def extractSource(tarball) :
	def extractCommand(tarball) :
		for extension, command in [
			('.tar.gz',  "tar xvfz %(sandbox)s/downloads/%(tarball)s -C %(sandbox)s/src"),
			('.tgz',     "tar xvfz %(sandbox)s/downloads/%(tarball)s -C %(sandbox)s/src"),
			('.tar.bz2', "tar xvfj %(sandbox)s/downloads/%(tarball)s -C %(sandbox)s/src"),
			] :
			if tarball.endswith(extension) : return command
		die("Unsuported compressed file extension: %s" % tarball)
	run( extractCommand(tarball) % dict(sandbox=sandbox,tarball=tarball), "Uncompressing %s"%tarball)

def ensureDir(adir) :
	try: os.makedirs(adir)
	except OSError :  pass

def scriptRelative(path) :
	return os.path.join(os.path.dirname(os.path.abspath(__file__)), path)

def sfcheck(project, package) :
	return (
		""" wget -q -O- 'http://sourceforge.net/projects/%s/files/%s/' | """
		""" grep 'title="/' | """
		""" sed -n 's,.*title="\(/[^:]*\).*released on \([0-9-]*\)",\\2 \\1,p' | """
		""" sort | """
		""" sed 's,^[^ ]* ,,' | """
		)%(project, package)

def buildPackage(name, uri, checkVersion, downloadUri, tarballName, buildCommand,
		srcdir=None,
		deps="",
		pinnedVersion = None,
	) :
	print "Module:", name
	subst = dict(
		sandbox = sandbox,
		prefix = prefix,
		target = target,
		sfmirror = sfmirror,
		name = name,
		pinned = pinnedVersion if pinnedVersion else "None",
	)
	availableVersion = output(checkVersion).strip()
	availableVersion or die("No online version found for the package\n Command used:\n%s" % checkVersion)
	subst.update (
		version = pinnedVersion if pinnedVersion else availableVersion
	)
	print "Found version: %s" % availableVersion
	if (pinnedVersion and pinnedVersion != availableVersion) :
		warning("Package: Pinning to version %s, although version %s is available" % (pinnedVersion, availableVersion))

	subst.update(
		tarball = tarballName % subst,
		srcdir = ("%(sandbox)s/src/" + (srcdir or "%(name)s-%(version)s/")) % subst,
	)
	print "srcdir:", subst['srcdir']
	download(downloadUri % subst)
	extractSource(subst['tarball'])
	patches = glob.glob(scriptRelative("mingw-"+name+"*"))
	print patches
	for patch in patches :
		applyPatch(subst['srcdir'], patch, level=1)
	run(buildCommand % subst)


ensureDir(os.path.join(sandbox, "src"))
ensureDir(os.path.join(sandbox, "downloads"))

os.environ.update(
	PKG_CONFIG_PATH = os.path.join(prefix, 'lib', 'pkgconfig'),
	)


if 0 : buildPackage( "pthread",
	uri = "http://sourceware.org/pthreads-win32/",
	checkVersion = ""
		""" wget -q -O- 'ftp://sourceware.org/pub/pthreads-win32/Release_notes' | """
		""" sed -n 's,^RELEASE \([0-9][^[:space:]]*\).*,\\1,p' | """
		""" tr '.' '-' | """
		""" head -1 """,
	tarballName = "pthreads-w32-%(version)s-release.tar.gz",
	downloadUri = "ftp://sourceware.org/pub/pthreads-win32/%(tarball)s",
	srcdir = "pthreads-w32-%(version)s-release",
	buildCommand =
		""" cd %(srcdir)s && """
		""" make CROSS=i586-mingw32msvc- GC-inlined && """
		""" mkdir -p %(prefix)s/{include,lib,bin} && """
		""" cp libpthreadGC2.a %(prefix)s/lib && """
		""" cp *.h %(prefix)s/include && """
		""" cp pthreadGC2.dll %(prefix)s/bin && """
		""" ln -s libpthreadGC2.a %(prefix)s/lib/libpthread.a """
	)


if 0 : buildPackage( "fftw",
	uri = "http://www.fftw.org",
	deps = "pthread",
	checkVersion =
		""" wget -q -O- 'http://www.fftw.org/download.html' | """
		""" sed -n 's,.*fftw-\([0-9][^>]*\)\.tar.*,\\1,p' | """
		""" grep -v alpha | """
		""" grep -v beta | """
		""" head -1 """,
	tarballName = "fftw-%(version)s.tar.gz",
	downloadUri = "http://www.fftw.org/%(tarball)s",
	buildCommand =
		"cd %(srcdir)s && "
		"autoconf && "
		"./configure --host='%(target)s' --prefix='%(prefix)s' && "
		"make install "
	)


if 0 : buildPackage( 'libmad',
	uri = "??",
	checkVersion =
		sfcheck("mad","libmad") +
		""" sed -n 's,.*libmad-\([0-9][^>]*\)\.tar.*,\\1,p' | """
		""" tail -1 """,
	tarballName = "libmad-%(version)s.tar.gz",
	downloadUri = "http://%(sfmirror)s/sourceforge/mad/%(tarball)s",
	buildCommand =
		""" cd %(srcdir)s && """
		""" sed -i '/-fforce-mem/d' configure.ac && """
		""" touch NEWS AUTHORS ChangeLog && """
		""" autoreconf --force --install && """
		""" ./configure --host='%(target)s' --prefix='%(prefix)s' && """
		""" make install """
	)


if 0 : buildPackage( "id3lib",
	uri = "http://id3lib.sourceforge.net/",
	checkVersion =
		sfcheck("id3lib", "id3lib") +
		""" sed -n 's,.*id3lib-\([0-9][^>]*\)\.tar.*,\\1,p' | """
		""" tail -1 """,
	downloadUri = "http://%(sfmirror)s/sourceforge/id3lib/%(tarball)s",
	tarballName = "id3lib-%(version)s.tar.gz",
	buildCommand =
		""" cd %(sandbox)s/src/id3lib-%(version)s && """
		""" autoconf && """
		""" ./configure --host='%(target)s' --prefix='%(prefix)s' && """
		""" make && """
		""" make install """
	)


if 0 : buildPackage( "libogg",
	uri = 'http://www.xiph.org/ogg/',
	checkVersion =
		""" wget -q -O- 'http://www.xiph.org/downloads/' | """
		""" sed -n 's,.*libogg-\([0-9][^>]*\)\.tar.*,\\1,p' | """
		""" head -1 """,
	tarballName = "libogg-%(version)s.tar.gz",
	downloadUri = "http://downloads.xiph.org/releases/ogg/%(tarball)s",
	buildCommand =
		""" cd %(srcdir)s && """
		""" sed -i 's,cross_compiling=no,cross_compiling=yes,' 'configure' && """
		""" ./configure --host='%(target)s' --prefix='%(prefix)s' && """
		""" make && """
		""" make install """
	)


if 0 : buildPackage( "libvorbis",
	uri = "http://www.vorbis.com/",
	deps = "libogg",
	checkVersion =
		""" wget -q -O- 'http://www.xiph.org/downloads/' | """
		""" sed -n 's,.*libvorbis-\([0-9][^>]*\)\.tar.*,\\1,p' |  """
		""" head -1 """,
	downloadUri = "http://downloads.xiph.org/releases/vorbis/libvorbis-%(version)s.tar.gz",
	tarballName = "libvorbis-%(version)s.tar.gz",
	buildCommand =
		""" cd %(srcdir)s && """
		""" ./configure --host='%(target)s' --prefix='%(prefix)s' && """ 
		""" make && """
		""" make install """
	)
	
# TODO: flac dll has missing symbols when linking it against sndfile
if 0 : buildPackage( "flac",
	uri = "http://flac.sourceforge.net/",
	deps = "libiconv libogg pthread", # TODO What about libiconv?
	checkVersion =
		""" wget -q -O- 'http://flac.cvs.sourceforge.net/viewvc/flac/flac/' | """
		""" grep '<option>FLAC_RELEASE_' | """
		""" sed -n 's,.*FLAC_RELEASE_\([0-9][0-9_]*\)__.*,\\1,p' | """
		""" sed 's,_,.,g' | """
		""" head -1 """,
	tarballName = "flac-%(version)s.tar.gz",
	downloadUri = "http://downloads.xiph.org/releases/flac/%(tarball)s",
	buildCommand =
		""" cd %(srcdir)s && """
		""" ./autogen.sh --host='%(target)s' --prefix='%(prefix)s' PKG_CONFIG_PATH=$PKG_CONFIG_PATH --disable-xmms-plugin --enable-ogg --disable-oggtest && """
#		""" ./configure --host='%(target)s' --prefix='%(prefix)s' --disable-xmms-plugin --enable-ogg --disable-oggtest && """
		""" make && """
		""" make install """
	)

if 0 : buildPackage( "libsndfile",
	uri = "http://www.mega-nerd.com/libsndfile/",
	deps = "flac ogg vorbis",
	pinnedVersion = "1.0.21",
	checkVersion =
		""" wget -q -O- 'http://www.mega-nerd.com/libsndfile/' | """
		""" grep '<META NAME="Version"' | """
		""" sed -n 's,.*CONTENT="libsndfile-\([0-9][^"]*\)">.*,\\1,p' | """
		""" head -1 """,
	tarballName = "libsndfile-%(version)s.tar.gz",
	downloadUri = "http://www.mega-nerd.com/libsndfile/files/libsndfile-%(version)s.tar.gz",
	buildCommand = 
		""" cd %(srcdir)s && """
#		""" autoconf && """
		""" ./configure mingw32 --host=%(target)s --prefix=%(prefix)s --enable-external-libs --disable-flac --disable-sqlite --disable-octave --disable-alsa && """
		""" make install """
	)

if 0 : buildPackage( "speex",
	uri = "http://www.speex.org/",
	deps = "libogg",
	checkVersion = 
		""" wget -q -O- 'http://git.xiph.org/?p=speex.git;a=tags' | """
		""" grep '<a class="list name"' | """
		""" sed -n 's,.*<a[^>]*>Speex-\([0-9][^<]*\)<.*,\\1,p' | """
		""" head -1 """,
	tarballName = "speex-%(version)s.tar.gz",
	downloadUri = "http://downloads.xiph.org/releases/speex/%(tarball)s",
	buildCommand =
		""" cd %(srcdir)s && """
		"""  ./configure  --host=%(target)s --prefix=%(prefix)s --with-ogg-dir=%(prefix)s && """ # TODO: --enable-sse not supported by arch
		""" make && """
		""" make install """
	)

if 0 : buildPackage( "liblo",
	uri = "http://liblo.sourceforge.net/",
	deps = "pthreads",
	checkVersion =
		sfcheck("liblo", "liblo") +
		""" sed -n 's,.*liblo-\([0-9][^>]*\)\.tar.*,\\1,p' | """
		""" tail -1 """,
	tarballName = "%(name)s-%(version)s.tar.gz",
	downloadUri = "http://%(sfmirror)s/sourceforge/liblo/%(tarball)s",
	buildCommand =
		""" cd %(srcdir)s && """
		""" ./autogen.sh  --host=%(target)s --prefix=%(prefix)s LDFLAGS="-L%(prefix)s/lib/" CPPFLAGS="-I%(prefix)s/include/" && """
		""" make && """
		""" make install """
	)
if 0 : buildPackage( "cppunit",
	uri = "http://liblo.sourceforge.net/",
	checkVersion =
		sfcheck("cppunit", "cppunit") +
		""" sed -n 's,.*cppunit-\([0-9][^>]*\)\.tar.*,\\1,p' | """
		""" tail -1 """,
	tarballName = "%(name)s-%(version)s.tar.gz",
	downloadUri = "http://%(sfmirror)s/sourceforge/cppunit/%(tarball)s",
	buildCommand =
		""" cd %(srcdir)s && """
		""" ./configure --host=%(target)s --prefix=%(prefix)s  --enable-doxygen=no && """
		""" make && """
		""" make install """
#		""" cp src/cppunit/.libs/libcppunit-1-12-0.dll ../../dlls""" # TODO: Needed?
	)
	# TODO: From the wiki: To get some extra autoconf macros and libtool do: 
	# sudo apt-get install autoconf-archive libtool automake

if 0 : buildPackage( "dlfcn-win32",
	uri = "http://code.google.com/p/dlfcn-win32/",
	checkVersion =
		""" wget -q -O- 'http://code.google.com/p/dlfcn-win32/downloads/list' | """
		""" sed -n 's,.*dlfcn-win32-\(r[0-9][r.0-9]*\)\.tar.bz2.*,\\1,p' | """
		""" sort -g | tail -1 """,
	tarballName = "%(name)s-%(version)s.tar.bz2",
	downloadUri = "http://dlfcn-win32.googlecode.com/files/%(tarball)s",
	buildCommand = ""
		""" cd %(srcdir)s && """
		""" sed -i '$aecho Done\\n' configure &&  """
		""" ./configure --prefix='%(prefix)s' --cross-prefix='%(target)s-' --incdir='%(prefix)s/include' --libdir='%(prefix)s/lib' --enable-shared && """
		""" make && """
		""" make install """
	)

if 1 : buildPackage( "ladspa-sdk", # TODO: still not working for binaries
	uri = "http://www.ladspa.org/",
	deps = "dlfcn-win32",
	checkVersion =
		""" wget -q -O- 'http://www.ladspa.org/download/' | """
		""" sed -n 's,.*ladspa_sdk_\([0-9][.0-9]*\)\.tgz.*,\\1,p' | """
		""" sort -n | """
		""" tail -1 """,
	tarballName = "ladspa_sdk_%(version)s.tgz",
	srcdir = "ladspa_sdk",
	downloadUri = "http://www.ladspa.org/download/%(tarball)s",
	buildCommand = 
		""" cd %(srcdir)s/src && """
		""" sed -i 's,-fPIC,,' makefile && """
		""" sed -i 's,^\(LIBRARIES.*\),\\1 -L%(prefix)s\\/lib,' makefile && """
		""" sed -i 's,\(mkdirhier\),mkdir -p,' makefile && """
		""" make install """
			""" CC=%(target)s-gcc """
			""" CPP=%(target)s-g++ """
			""" LD=%(target)s-g++  """
			""" INCLUDES="-I. -I%(prefix)s/include" """
#			""" INSTALL_PLUGINS_DIR=%(prefix)s/lib/ladspa """
#			""" INSTALL_INCLUDE_DIR=%(prefix)s/include """
#			""" INSTALL_BINARY_DIR=%(prefix)s/bin """
			""" PREFIX=%(prefix)s """
			""" PROGRAMS="../bin/analyseplugin ../bin/listplugins" """ # TODO: applyplugin depends on endian.h, not in mingw
	)








