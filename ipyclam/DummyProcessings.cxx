#include "DummyProcessings.hxx"
#include <CLAM/ProcessingFactory.hxx>

#define IPYCLAMTEST_REGISTER_PROCESSING(NUMBER,NAME) \
static const char * metadata_##NUMBER[] = { \
	"key", #NAME, \
	"category", "ipyclam tests", \
	0 \
	}; \
static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, NAME> registrator_##NUMBER(metadata_##NUMBER);

IPYCLAMTEST_REGISTER_PROCESSING( 1, DummyProcessingWithInAndOutPorts);
IPYCLAMTEST_REGISTER_PROCESSING( 2, DummyProcessingWithInAndOutControls);
IPYCLAMTEST_REGISTER_PROCESSING( 3, Dummy2IOPortsControls);
IPYCLAMTEST_REGISTER_PROCESSING( 4, DummyProcessingWithStringConfiguration);
IPYCLAMTEST_REGISTER_PROCESSING( 5, DummyProcessingWithIntConfiguration);
IPYCLAMTEST_REGISTER_PROCESSING( 6, DummyProcessingWithCompleteConfiguration);
IPYCLAMTEST_REGISTER_PROCESSING( 7, DummyProcessingWithOptionalAttributes);
IPYCLAMTEST_REGISTER_PROCESSING( 8, DummyTypeProcessing1);
IPYCLAMTEST_REGISTER_PROCESSING( 9, DummyTypeProcessing2);
IPYCLAMTEST_REGISTER_PROCESSING(10, ProcessingWithPortsAndControls);
IPYCLAMTEST_REGISTER_PROCESSING(11, DummyPortSink);
IPYCLAMTEST_REGISTER_PROCESSING(12, DummyPortSource);
IPYCLAMTEST_REGISTER_PROCESSING(13, DummyControlSink);
IPYCLAMTEST_REGISTER_PROCESSING(14, DummyControlSource);
IPYCLAMTEST_REGISTER_PROCESSING(15, ProcessingWithNameSpacedPorts);
IPYCLAMTEST_REGISTER_PROCESSING(16, ProcessingWithNameSpacedControls);
IPYCLAMTEST_REGISTER_PROCESSING(17, ProcessingWithNumericPorts);
IPYCLAMTEST_REGISTER_PROCESSING(18, Dummy6IOPorts);
IPYCLAMTEST_REGISTER_PROCESSING(19, Dummy6IOControls);
IPYCLAMTEST_REGISTER_PROCESSING(20, OtherControlSink);


#include <boost/python.hpp>

BOOST_PYTHON_MODULE(DummyProcessings)
{}

