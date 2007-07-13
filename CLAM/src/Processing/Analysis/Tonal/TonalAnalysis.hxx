
#ifndef TonalAnalysis_hxx
#define TonalAnalysis_hxx

#include <CLAM/DataTypes.hxx>
#include <CLAM/OutPort.hxx>
#include <CLAM/AudioInPort.hxx>
#include <CLAM/Processing.hxx>
#include <CLAM/Audio.hxx>
#include <CLAM/ProcessingConfig.hxx>
#include <string>

namespace Simac { class ChordExtractor; }

namespace CLAM
{

class TonalAnalysisConfig : public ProcessingConfig
{
public:
	DYNAMIC_TYPE_USING_INTERFACE (TonalAnalysisConfig, 4, ProcessingConfig);
	DYN_ATTRIBUTE (0, public, double, FilterInertia);
	DYN_ATTRIBUTE (1, public, bool, TunningEnabled);
	DYN_ATTRIBUTE (2, public, bool, PeakWindowingEnabled);
	DYN_ATTRIBUTE (3, public, unsigned, HopRatio);
protected:
	void DefaultInit(void);
};

class TonalAnalysis : public Processing
{
private:
	
	TonalAnalysisConfig _config;
	AudioInPort _input;
	OutPort<std::vector<CLAM::TData> > _pcp;
	OutPort<std::vector<CLAM::TData> > _chordCorrelation;
	OutPort<std::vector<std::pair<CLAM::TData, CLAM::TData> > > _chromaPeaks;
	OutPort<std::pair<CLAM::TData, CLAM::TData> > _tunning;

public:
	TonalAnalysis( const TonalAnalysisConfig & config = TonalAnalysisConfig() );
#ifdef _MSC_VER
	TonalAnalysis( bool callThisAsFakeConstructorToBeCalledToAvoidWindowsInstantiationBug );
#endif

	bool Do();
	virtual ~TonalAnalysis();
	const char * GetClassName() const {return "TonalAnalysis";}
	
	inline const ProcessingConfig &GetConfig() const { return _config;}
	bool ConcreteConfigure(const ProcessingConfig& c);
private:
	Simac::ChordExtractor * _implementation;
	std::vector<float> _floatBuffer;

};

} //namespace CLAM

#endif



