#ifndef ImpulseResponseLoaderAndInterpolator_hxx
#define ImpulseResponseLoaderAndInterpolator_hxx

#include <CLAM/InPort.hxx>
#include <CLAM/OutPort.hxx>
#include <CLAM/Processing.hxx>
#include <CLAM/MonoAudioFileReader.hxx>
#include <CLAM/AudioWindowing.hxx>
#include "MyFFT.hxx"
#include "ComplexSpectrum.hxx"
#include "ComplexSpectrumMixer.hxx"
#include "DummyGlobalData.hxx"
#include <algorithm>
#include <cmath>

namespace CLAM
{

class ImpulseResponseLoaderAndInterpolator : public Processing
{ 
public:
	class Config : public ProcessingConfig
	{
	    DYNAMIC_TYPE_USING_INTERFACE( Config, 1, ProcessingConfig );
	    DYN_ATTRIBUTE( 0, public, int, FrameSize);

	protected:
	    void DefaultInit()
	    {
		  AddAll();
		  UpdateData();
		  SetFrameSize(512);
	    };
	};

private:
	Config _config;
	std::vector<ComplexSpectrum> _responseSpectrums;
	std::vector<ComplexSpectrum> _delayedSpectrums;
	unsigned _current;

	//TODO extract to new class
	InControl _position;
	double _lastPosition;
	std::vector<ComplexSpectrum> _originResponseSpectrums;
	std::vector<ComplexSpectrum> _finalResponseSpectrums;


public:
	const char* GetClassName() const { return "ImpulseResponseLoaderAndInterpolator"; }
	ImpulseResponseLoaderAndInterpolator(const Config& config = Config()) 
		: _position("Position", this)
	{
		Configure( config );
		_position.SetBounds(0,1);
	}
	bool ConcreteConfigure(const ProcessingConfig & config)
	{
		CopyAsConcreteConfig(_config, config);

		if (!ComputeResponseSpectrums("room1/p_emissor_4-1-1_receptor_4-1-1.wav", _originResponseSpectrums )) return false;
		if (!ComputeResponseSpectrums("room1/p_emissor_4-1-1_receptor_4-10-1.wav", _finalResponseSpectrums )) return false;

		ConfigureInterpolatedResponseSpectrums();
		_position.DoControl(_position.DefaultValue());
		InterpolateResponseSpectrums();

		const unsigned nBlocks = _responseSpectrums.size();
		std::cout << "ImpulseResponseLoaderAndInterpolator: N blocks " << nBlocks << std::endl; 
		_delayedSpectrums.resize(nBlocks);
		for (unsigned i=0; i<nBlocks; i++)
		{
			ComplexSpectrum & spectrum = _delayedSpectrums[i];
			spectrum.spectralRange=_responseSpectrums[0].spectralRange;
			spectrum.bins.assign(_responseSpectrums[0].bins.size(),std::complex<CLAM::TData>());
		}
		_current=0;


		return true;
	}
	const ProcessingConfig & GetConfig() const { return _config; }

	void ConfigureInterpolatedResponseSpectrums()
	{
		const unsigned nBlocks = std::min(_originResponseSpectrums.size(), _finalResponseSpectrums.size());
		_responseSpectrums.resize(nBlocks);
		for (unsigned i=0; i<nBlocks; i++)
		{
			ComplexSpectrum & spectrum = _responseSpectrums[i];
			spectrum.spectralRange=_finalResponseSpectrums[0].spectralRange;
			spectrum.bins.assign(_finalResponseSpectrums[0].bins.size(),std::complex<CLAM::TData>());
		}
		DummyGlobalData::SetStaticResponseSpectrums( &_responseSpectrums );
		std::cout << "Dummy global data set"<<std::endl;

	}
	void InterpolateResponseSpectrums()
	{
		if (std::abs( _position.GetLastValue() - _lastPosition) < 0.001 )
			return;
		std::cout << "ImpulseResponseLoaderAndInterpolator: position changed"<<std::endl;
		_lastPosition = _position.GetLastValue();
		ComplexSpectrumMixer mixer;
		mixer.GetInControl("Gain1").DoControl(_lastPosition);
		mixer.GetInControl("Gain2").DoControl(1-_lastPosition);
		unsigned nBlocks = _responseSpectrums.size();
		for (unsigned i=0; i<nBlocks; i++) //TODO paramter to limit the nblocks to interpolate
		{
			mixer.Do( _originResponseSpectrums[i], _finalResponseSpectrums[i], _responseSpectrums[i]);
		}
	}

	bool Do()
	{

		InterpolateResponseSpectrums();
		return true;
	}
private:
	bool ComputeResponseSpectrums(const char* wavfile, std::vector<ComplexSpectrum> & responseSpectrums)
	{
		MonoAudioFileReaderConfig readerConfig;
		readerConfig.SetSourceFile(wavfile);
		MonoAudioFileReader reader(readerConfig);
		if (!reader.IsConfigured())
		{
			AddConfigErrorMessage("Configuring the inner AudioFileReader:\n");
			AddConfigErrorMessage(reader.GetConfigErrorMessage());
			return false;
		}
		const unsigned nSamples = reader.GetHeader().GetSamples();
		std::cout << "NSamples: " << nSamples << std::endl;

		AudioWindowingConfig windowerConfig;
		windowerConfig.SetSamplingRate(44100); // TODO: Take it from the file
		windowerConfig.SetHopSize(_config.GetFrameSize());
		windowerConfig.SetWindowSize(_config.GetFrameSize()+1);
		windowerConfig.SetFFTSize(_config.GetFrameSize()*2);
		windowerConfig.SetDoHalfWindowShift(false);
		windowerConfig.SetWindowType(EWindowType::eNone);
		AudioWindowing windower(windowerConfig);
		if (!windower.IsConfigured())
		{
			AddConfigErrorMessage("Configuring the inner AudioWindowing:\n");
			AddConfigErrorMessage(windower.GetConfigErrorMessage());
			return false;
		}
		FFTConfig fftConfig; 
		fftConfig.SetAudioSize(_config.GetFrameSize()*2);
		MyFFT fft(fftConfig);

		ConnectPorts(reader,0,windower,0);
		ConnectPorts(windower,0,fft,0);
		InPort<ComplexSpectrum> fetcher;
		fft.GetOutPorts().GetByNumber(0).ConnectToIn(fetcher);

		responseSpectrums.clear();
		reader.Start();
		windower.Start();
		fft.Start();
		
		for (bool samplesAvailable=true; samplesAvailable; )
		{
			samplesAvailable = reader.Do();
			if (!windower.CanConsumeAndProduce()) continue;
			windower.Do();
			fft.Do();
			responseSpectrums.push_back(fetcher.GetData());
//			fetcher.GetData().dump(std::cout);
			fetcher.Consume();
		}

		reader.Stop();
		windower.Stop();
		fft.Stop();
		return true;
	}

};

} // namespace CLAM

#endif // ImpulseResponseLoaderAndInterpolator_hxx

