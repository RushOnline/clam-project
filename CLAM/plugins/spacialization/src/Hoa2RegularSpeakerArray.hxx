#ifndef Hoa2RegularSpeakerArray_hxx
#define Hoa2RegularSpeakerArray_hxx
#include <CLAM/AudioInPort.hxx>
#include <CLAM/AudioOutPort.hxx>
#include <CLAM/Processing.hxx>
#include <CLAM/Audio.hxx>
#include <CLAM/InControl.hxx>
#include <CLAM/Filename.hxx>
#include "Orientation.hxx"
#include "SpeakerLayout.hxx"
#include <cmath>

/**
 Decodes a Nth order signal for a regular array of speakers.
 Regular arrays can be decoded with a single constant for all the components 
 of the same degree, given an order of ambisonics.
 @param Order [Config] The order of ambisonics that it will be fed with.
 @param SpeakerLayout [Config] A file containing the target speaker layout.
 @param[out] XX [Port] Ambisonics component were XX is one of W,X,Y,Z,R,S,T,U,V,K,L,M,N,O,P,Q.
 @param[out] AAA [Ports] Audio signals to be emitted by the speaker AAA. Here AAA is the label in the layout file.
 @see SpeakerLayout for a description of the file format.
 @todo Make the order configurable
 @todo Make the decoding configurable
 @ingroup SpatialAudio
 @see AmbisonicsConventions
*/
class Hoa2RegularSpeakerArray : public CLAM::Processing
{ 
public:
	class Config : public CLAM::ProcessingConfig
	{
		DYNAMIC_TYPE_USING_INTERFACE( Config, 2, ProcessingConfig );
		DYN_ATTRIBUTE( 0, public, unsigned, Order);
		DYN_ATTRIBUTE( 1, public, CLAM::InFilename, SpeakerLayout);
	protected:
		void DefaultInit()
		{
			AddAll();
			UpdateData();
			SetOrder(1);
		};
	};
private:
	SpeakerLayout _layout;
	typedef std::vector<CLAM::AudioOutPort*> OutPorts;
	typedef std::vector<CLAM::AudioInPort*> InPorts;
	OutPorts _outputs;
	InPorts _inputs;
	Config _config;
	double _decoding[4];

public:
	const char* GetClassName() const { return "Hoa2RegularSpeakerArray"; }
	Hoa2RegularSpeakerArray(const Config& config = Config()) 
	{
		Configure( config );
	}
	const CLAM::ProcessingConfig & GetConfig() const
	{
		return _config;
	}

	bool ConcreteConfigure(const CLAM::ProcessingConfig& config)
	{
		CopyAsConcreteConfig(_config, config);
		unsigned buffersize = 512; // BackendBufferSize();
		unsigned order = _config.GetOrder();
		bool errorHappened = false;
		if (order>3)
		{
			// If never configured just initialize a sane default to keep connections
			if (_inputs.size() == 0) ResizePortsToOrder(1, buffersize);
			errorHappened |= AddConfigErrorMessage("Ambisonics orders beyond 3rd are not supported");
			// Don't exit yet, we want to keep outports connections when loading from a network
		}
		else ResizePortsToOrder(order, buffersize);
		ComputeDecoding(order);

		std::string errorMessage;
		if (not _layout.load(_config.GetSpeakerLayout(), errorMessage))
			return AddConfigErrorMessage(errorMessage);
		else ResizePortsToLayout(buffersize);

		return not errorHappened;
	}
	bool Do()
	{
		// Ambisonics definition assures us one component for order 0 so we can do that:
		const unsigned nSamples = _inputs[0]->GetAudio().GetBuffer().Size();
		const unsigned nComponents = _inputs.size();
		const unsigned nSpeakers = _outputs.size();
		const CLAM::TData* components[nComponents];
		for (unsigned component=0; component<nComponents; component++)
			components[component] = &_inputs[component]->GetAudio().GetBuffer()[0];
		CLAM::TData* speakers[nSpeakers];
		for (unsigned speaker=0; speaker<nSpeakers; speaker++)
			speakers[speaker] = &_outputs[speaker]->GetAudio().GetBuffer()[0];

		double componentWeight[nComponents];
		CLAM::SphericalHarmonicsDefinition * sh = CLAM::Orientation::sphericalHarmonics();
		for (unsigned speaker=0; speaker<nSpeakers; speaker++)
		{
			for (unsigned hoaComponent=0; hoaComponent<nComponents; hoaComponent++)
				componentWeight[hoaComponent] = _decoding[sh[hoaComponent].zProjection] * _layout.orientation(speaker).sphericalHarmonic(sh[hoaComponent]);
			CLAM::TData * speakerBuffer = speakers[speaker];
			for (unsigned sample=0; sample<nSamples; sample++)
			{
				double sampleValue = 0;
				for (unsigned component=0; component<nComponents; component++)
					sampleValue += components[component][sample] * componentWeight[component];
				speakerBuffer[sample] = sampleValue;
			}
		}
		for (unsigned speaker=0; speaker<nSpeakers; speaker++)
			_outputs[speaker]->Produce();
		for (unsigned component=0; component<nComponents; component++)
			_inputs[component]->Consume();
		return true;
	}
	~Hoa2RegularSpeakerArray()
	{
		for (unsigned speaker=0; speaker<_outputs.size(); speaker++)
			delete _outputs[speaker];
		for (unsigned component=0; component<_inputs.size(); component++)
			delete _inputs[component];
	}
private:
	void ResizePortsToOrder(unsigned order, unsigned buffersize)
	{
		// Set up the inputs according to ambisonics order
		CLAM::SphericalHarmonicsDefinition *sh = CLAM::Orientation::sphericalHarmonics();
		unsigned i=0;
		for (;sh[i].name; i++)
		{
			if (sh[i].order > order) break;
			if (i<_inputs.size()) continue;
			CLAM::AudioInPort * port = new CLAM::AudioInPort( sh[i].name, this);
			port->SetSize( buffersize );
			port->SetHop( buffersize );
			_inputs.push_back( port );
		}
		unsigned actualSize=i;
		for (;i<_inputs.size(); i++)
			delete _inputs[i];
		_inputs.resize(actualSize);
	}
	void ResizePortsToLayout(unsigned buffersize)
	{
		// Set up the outputs according to the layout
		unsigned speakerToUpdate = firstDirtySpeaker();
		// delete existing speakers from the first one with different name
		for (unsigned oldSpeaker=speakerToUpdate ; oldSpeaker<_outputs.size(); oldSpeaker++)
			delete _outputs[oldSpeaker];
		_outputs.resize(speakerToUpdate);
		// adding new speakers
		for ( ; speakerToUpdate<_layout.size(); speakerToUpdate++)
		{
			CLAM::AudioOutPort * port = new CLAM::AudioOutPort( _layout.name(speakerToUpdate), this);
			port->SetSize( buffersize );
			port->SetHop( buffersize );
			_outputs.push_back( port );
		}
	}
	unsigned firstDirtySpeaker() const
	{
		for (unsigned speaker = 0; speaker<_layout.size(); speaker++)
		{
			if (speaker>=_outputs.size()) return speaker; 
			if (_outputs[speaker]->GetName() != _layout.name(speaker)) return speaker;
		}
		return _layout.size();
	}
	void ComputeDecoding(unsigned order)
	{
		for (unsigned i=0; i<=order; i++)
			_decoding[i] = inphaseDecoding(order, i);
		for (unsigned i=order+1; i<4; i++)
			_decoding[i] = 0;
	}
	double inphaseDecoding(unsigned maxOrder, unsigned order)
	{
		/*
			1..n * 1..n / (1..n-l) / (1..n+l) = n-l+1..n / n+1..n+l
		*/
		double g = 1;
		for (unsigned i=maxOrder-order+1; i<=maxOrder; i++)
			g *= i;
		for (unsigned i=maxOrder+1; i<=maxOrder+order; i++)
			g /= i;
		if (order) g *= 2;
		std::cout << "Inphase decoding " << maxOrder << "," << order << " " << g << std::endl;
		return g;
	}
 
};
#endif

