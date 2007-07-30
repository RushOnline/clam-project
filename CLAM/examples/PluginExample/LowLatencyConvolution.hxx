#ifndef LowLatencyConvolution_hxx
#define LowLatencyConvolution_hxx

#include <CLAM/InPort.hxx>
#include <CLAM/OutPort.hxx>
#include <CLAM/Processing.hxx>
#include <CLAM/MonoAudioFileReader.hxx>
#include <CLAM/AudioWindowing.hxx>
#include "MyFFT.hxx"
#include "ComplexSpectrum.hxx"
#include "ComplexSpectrumProduct.hxx"
#include "ComplexSpectrumSum.hxx"
#include "ComplexSpectrumMixer.hxx"
#include "DummyGlobalData.hxx"
#include <algorithm>
#include <cmath>

namespace CLAM
{

class LowLatencyConvolution : public Processing
{ 
public:
	class Config : public ProcessingConfig
	{
	    DYNAMIC_TYPE_USING_INTERFACE( Config, 2, ProcessingConfig );
	    DYN_ATTRIBUTE( 0, public, int, FrameSize);
	    DYN_ATTRIBUTE( 1, public, AudioInFilename, ImpulseResponseAudioFile );

	protected:
	    void DefaultInit()
	    {
		  AddAll();
		  UpdateData();
		  SetFrameSize(512);
	    };
	};

private:
	InPort<ComplexSpectrum> _input;
	OutPort<ComplexSpectrum> _output;

	Config _config;
	std::vector<ComplexSpectrum> _responseSpectrums;
	std::vector<ComplexSpectrum> _delayedSpectrums;
	unsigned _current;
	ComplexSpectrumProduct _product;
	ComplexSpectrumSum _sum;
	std::vector<ComplexSpectrum> & ResponseSpectrums()
	{
		return DummyGlobalData::GetStaticResponseSpectrums();
//		return _responseSpectrums;
	}
	//TODO extract to new class
	double _lastPosition;
	std::vector<ComplexSpectrum> _finalResponseSpectrums;


public:
	const char* GetClassName() const { return "LowLatencyConvolution"; }
	LowLatencyConvolution(const Config& config = Config()) 
		: _input("Input", this)
		, _output("Output", this)
	{
		Configure( config );
	}
	bool ConcreteConfigure(const ProcessingConfig & config)
	{
		CopyAsConcreteConfig(_config, config);

		if (!ComputeResponseSpectrums("", _responseSpectrums )) return false;

		// Fill the _delayedSpectrums the same size of response spectrums, with zeros
		const unsigned nBlocks = ResponseSpectrums().size();
		std::cout << "LowLatencyConvolution: N blocks " << nBlocks << std::endl; 
		_delayedSpectrums.resize(nBlocks);
		for (unsigned i=0; i<nBlocks; i++)
		{
			ComplexSpectrum & spectrum = _delayedSpectrums[i];
			spectrum.spectralRange=ResponseSpectrums()[0].spectralRange;
			spectrum.bins.assign(ResponseSpectrums()[0].bins.size(),std::complex<CLAM::TData>());
		}
		_current=0;

		return true;
	}
	const ProcessingConfig & GetConfig() const { return _config; }


	bool Do()
	{

		const ComplexSpectrum & input = _input.GetData();
		ComplexSpectrum & output = _output.GetData();

		unsigned nBlocks = std::min(ResponseSpectrums().size(),1000u);
		std::cout << "Do nBlocks: " <<nBlocks <<std::endl;

		output.spectralRange = input.spectralRange;
		const unsigned nBins = input.bins.size(); 
		output.bins.resize( nBins );
		for (unsigned i=0; i<output.bins.size(); i++)
			output.bins[i]=0;
		_delayedSpectrums[_current].bins=input.bins;
		unsigned delayIndex=_current+1;
		ComplexSpectrum productSpectrum;
		for (unsigned i=0; i<nBlocks; i++)
		{
			if (delayIndex>=nBlocks) delayIndex=0;
			_product.Do(ResponseSpectrums()[nBlocks-i-1], _delayedSpectrums[delayIndex++], productSpectrum);
			_sum.Do(productSpectrum, output, output);
		}
		_current++;
		if (_current>=nBlocks) 
		{
			_current=0;
		}
		// Tell the ports this is done
		_input.Consume();
		_output.Produce();
		return true;
	}
private:
	bool ComputeResponseSpectrums(const std::string & wavfile, std::vector<ComplexSpectrum> & responseSpectrums)
	{
		if (wavfile == "") return true;

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

#endif // LowLatencyConvolution_hxx
