#ifndef NSurround_hxx
#define NSurround_hxx
#include <CLAM/AudioInPort.hxx>
#include <CLAM/AudioOutPort.hxx>
#include <CLAM/Processing.hxx>
#include <CLAM/Audio.hxx>
#include <CLAM/InControl.hxx>
#include <cmath>

/**
 Converts pressure and velocity input into N equiangular surround
 channels with the first one at the front and going to the left.
 @param NChannels [Config] The number of channels that are to be generated. That configuration parameter changes number of output ports the processing will have and the separation among the speakers.
 @param[in] beta [Control] Angle in degrees for a simulated rotation of the receiver in the xy plane to the right.
 @param[in] p [Port] Pressure to be reproduced at the sweet-spot
 @param[in] vx [Port] X component of the velocity to be reproduced at the sweet-spot
 @param[in] vy [Port] Y component of the velocity to be reproduced at the sweet-spot
 @param[out] "out AAA" [Ports] Audio signals to be emitted by the speaker at AAA. Here AAA is the angle in degrees for each one of the speakers.
 @bug It should take W,X,Y,Z instead pressure and velocity according to conventions
 @todo Review the rotation direction with the conventions.
 @ingroup SpatialAudio
*/
class NSurround : public CLAM::Processing
{ 
protected:
	class Config : public CLAM::ProcessingConfig
	{
		DYNAMIC_TYPE_USING_INTERFACE( Config, 1, ProcessingConfig );
		DYN_ATTRIBUTE( 0, public, int, NChannels);
	protected:
		void DefaultInit()
		{
			AddAll();
			UpdateData();
			SetNChannels(0);
		};
	};

	CLAM::AudioInPort _p;
	CLAM::AudioInPort _vx;
	CLAM::AudioInPort _vy;
	CLAM::InControl _beta;
	std::vector<double> _sinAlphas;
	std::vector<double> _cosAlphas;
	typedef std::vector<CLAM::AudioOutPort*> OutPorts;
	OutPorts _outputs;
	unsigned _nChannels;
	Config _config;

public:
	const char* GetClassName() const { return "NSurround"; }
	NSurround(const Config& config = Config()) 
		: _p("p", this)
		, _vx("vx", this)
		, _vy("vy", this)
		, _beta("beta", this) // angle in degrees
		, _nChannels(15)
	{
		Configure( config );
		_beta.SetBounds(-360, 360); //a complete spin on each slider direction
		_beta.DoControl(0.);

	}
	const CLAM::ProcessingConfig & GetConfig() const
	{
		return _config;
	}

	bool ConcreteConfigure(const CLAM::ProcessingConfig& config)
	{
		CopyAsConcreteConfig(_config, config);
		RemovePortsAndControls();
		_sinAlphas.clear();
		_cosAlphas.clear();

		_nChannels=_config.GetNChannels();
		unsigned buffersize = BackendBufferSize();
		for (unsigned i=0; i<_nChannels; i++)
		{
			double angle = 360.*i/_nChannels;
			double rad = M_PI*angle/180;
			_sinAlphas.push_back( std::sin(rad) );
			_cosAlphas.push_back( std::cos(rad) );
	//		std::cout << "sin "<< angle << " "<< std::sin(rad) << std::endl;

			std::ostringstream nameStream;
			nameStream << "out " << angle;
			CLAM::AudioOutPort * port = new CLAM::AudioOutPort( nameStream.str(), this);
			port->SetSize( buffersize );
			port->SetHop( buffersize );
			_outputs.push_back( port );
		}
		
		_p.SetSize(buffersize);
		_p.SetHop(buffersize);
		_vx.SetSize(buffersize);
		_vx.SetHop(buffersize);
		_vy.SetSize(buffersize);
		_vy.SetHop(buffersize);
		return true;
	}
 
	bool Do()
	{
		const double beta=_beta.GetLastValue()*M_PI/180; //conversion. beta is in radians
		const double cosBeta=std::cos(beta);
		const double sinBeta=std::sin(beta);
		const CLAM::DataArray& p =_p.GetAudio().GetBuffer();
		const CLAM::DataArray& vx =_vx.GetAudio().GetBuffer();
		const CLAM::DataArray& vy =_vy.GetAudio().GetBuffer();
		CLAM::TData* channels[_nChannels];
	
		for (unsigned channel=0; channel<_nChannels; channel++)
			channels[channel] = &_outputs[channel]->GetAudio().GetBuffer()[0];


		for (int i=0; i<p.Size(); i++)
		{
			double ux = vx[i] * cosBeta - vy[i] * sinBeta;
			double uy = vx[i] * sinBeta + vy[i] * cosBeta;
			for (unsigned channel=0; channel<_nChannels; channel++)
			{
				channels[channel][i] = 0.5*( p[i] - ux * _cosAlphas[channel] - uy * _sinAlphas[channel] );
			}
		}

		_p.Consume();
		_vx.Consume();
		_vy.Consume();
		// Tell the ports this is done
		for (unsigned channel=0; channel<_nChannels; channel++)
			_outputs[channel]->Produce();
		
		return true;
	}
	void RemovePortsAndControls()
	{
		OutPorts::iterator it;
		for(it=_outputs.begin(); it!=_outputs.end(); it++)
			delete *it;
		_outputs.clear();
		// Delete ports from Processing (base class) register
		GetOutPorts().Clear();
	}
	~NSurround()
	{
		RemovePortsAndControls();
	}



};
#endif

