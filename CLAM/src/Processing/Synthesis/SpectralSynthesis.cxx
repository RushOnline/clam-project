/*
 * Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include "SpectralSynthesis.hxx"

using namespace CLAM;


Flags<7>::tFlagValue SpectralSynthesisSnapshotsFlags::sFlagValues[] = {
 	{SpectralSynthesisSnapshotsFlags::eInputFrameSpectrum,"InputFrameSpectrum"},
	{SpectralSynthesisSnapshotsFlags::eIFFTOutput,"IFFTOutput"},
	{SpectralSynthesisSnapshotsFlags::eAudioFrameNoCircularShift,"AudioFrameNoCircularShift"},
	{SpectralSynthesisSnapshotsFlags::eAudioFrameNoZeroPadding,"AudioFrameNoZeroPadding"},
	{SpectralSynthesisSnapshotsFlags::eWindowedAudioFrame,"InverseWindowedAudioFrame"},
	{SpectralSynthesisSnapshotsFlags::eOutputAudioFrame,"OutputAudioFrame"},
	{SpectralSynthesisSnapshotsFlags::eSynthesisWindow,"SynthesisWindow"},
	{0,NULL}
}; 


/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/*					SpectralSYNTHESIS CONFIGURATION						*/
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////


void SpectralSynthesisConfig::DefaultInit()
{
	AddAll();
	UpdateData();
	DefaultValues();	
}

void SpectralSynthesisConfig::DefaultValues()
{
/** FFTSize will be next power of two of the window size*/
	SetprZeroPadding(0);
	SetSamplingRate(44100);

	/** Default analysis window size corresponds to 512*/
	GetAnalWindowGenerator().SetName("AnalysisWG");
	SetAnalWindowSize(513);
	SetAnalWindowType(EWindowType::eBlackmanHarris92);
	GetAnalWindowGenerator().SetInvert(true);

	/** WindowSize/2*/
	SetHopSize((GetAnalWindowSize()-1)/2);

	/** Synthesis Window Type is set to triangular*/
	GetSynthWindowGenerator().SetName("SynthesisWG");
	GetSynthWindowGenerator().SetType(EWindowType::eTriangular);
	GetSynthWindowGenerator().SetNormalize(EWindowNormalize::eNone);
	GetSynthWindowGenerator().SetSize(GetHopSize()*2+1);
	/**/

	SetFrameSize(256);

	SpectralSynthesisSnapshotsFlags tmpfl;
	tmpfl.bInputFrameSpectrum=tmpfl.bIFFTOutput=tmpfl.bAudioFrameNoCircularShift=tmpfl.bAudioFrameNoZeroPadding=
		tmpfl.bWindowedAudioFrame=tmpfl.bOutputAudioFrame=tmpfl.bSynthesisWindow=false;
	SetDisplayFlags(tmpfl);
	

}

/** Synthesis Window size in miliseconds. In num. of samples WindowSize/SR is forced to be odd*/	
void SpectralSynthesisConfig::SetAnalWindowSize(TSize w)
{
	CLAM_ASSERT(w%2==1,"Window size must be odd");
	GetAnalWindowGenerator().SetSize(w);
	TData audioSize=TData(PowerOfTwo((w-1)*int(pow(TData(2.0),TData(GetZeroPadding())))));
	GetIFFT().SetAudioSize(int(audioSize));
	GetCircularShift().SetAmount(TData(w/2)); 

/*	TODO:This condition should be checked!!
	if(w<2*GetHopSize()+1)
		SetHopSize((w-1)/2);*/
}

TSize SpectralSynthesisConfig::GetAnalWindowSize() const
{
	return GetAnalWindowGenerator().GetSize();
}

/** Analysis Window type*/
void SpectralSynthesisConfig::SetAnalWindowType(const EWindowType& t)
{
	GetAnalWindowGenerator().SetType(t);
}

const EWindowType& SpectralSynthesisConfig::GetAnalWindowType() const
{
	return GetAnalWindowGenerator().GetType();
}


void SpectralSynthesisConfig::SetSynthWindowSize(TSize w)
{
	CLAM_ASSERT(w%2==1,"Window size must be odd");
	GetSynthWindowGenerator().SetSize(w);
}

TSize SpectralSynthesisConfig::GetSynthWindowSize() const
{
	return GetSynthWindowGenerator().GetSize();
}

/** Zero padding factor*/
void SpectralSynthesisConfig::SetZeroPadding(int z)
{
	SetprZeroPadding(z);
	TData audioSize=TData(PowerOfTwo((GetAnalWindowSize()-1)*int(pow(TData(2),TData(GetZeroPadding())))));
	GetIFFT().SetAudioSize(int(audioSize));
}

int SpectralSynthesisConfig::GetZeroPadding() const
{
	return GetprZeroPadding();
}

/** Synthesis Hop size in miliseconds. Must be < (WindowSize-(1/SR))/2*/	
void SpectralSynthesisConfig::SetHopSize(TSize h)
{

	//CLAM_ASSERT(GetSynthWindowSize()>=2*h, "SpectralSynthesisConfig::SetHopSize: Hop Size is too large compared to window size");
	GetSynthWindowGenerator().SetSize(2*h+1);
	GetOverlapAdd().SetHopSize(h);
	GetOverlapAdd().SetBufferSize(GetFrameSize()+h);
}

void SpectralSynthesisConfig::SetFrameSize(TSize f)
{
	GetOverlapAdd().SetFrameSize(f);
	GetOverlapAdd().SetBufferSize(f+GetHopSize());
}

TSize SpectralSynthesisConfig::GetFrameSize()
{
	return GetOverlapAdd().GetFrameSize();
}

TSize SpectralSynthesisConfig::GetHopSize() const
{
	return GetOverlapAdd().GetHopSize();
}

/** Sampling rate of the input audio*/
void SpectralSynthesisConfig::SetSamplingRate(TData sr)
{
	SetprSamplingRate(int(sr));

	/** for sychronizing existing config*/
	SetAnalWindowSize(GetAnalWindowSize());
}

TData SpectralSynthesisConfig::GetSamplingRate() const
{
	return TData(GetprSamplingRate());
}


TInt32 SpectralSynthesisConfig::PowerOfTwo(TInt32 size)
{
	int tmp = size;
	int outputSize = 1;
	while (tmp) 
	{
	 	outputSize=outputSize << 1;
	 	tmp=tmp >> 1;
	}
	if(outputSize == size << 1)
		outputSize = outputSize >> 1;
	return outputSize;
}

//methods for PO

void SpectralSynthesis::AttachChildren()
{
	mPO_AnalWindowGen.SetParent(this);
	mPO_SynthWindowGen.SetParent(this);
	mPO_AudioProduct.SetParent(this);
	mPO_CircularShift.SetParent(this);
	mPO_IFFT.SetParent(this);
	mPO_OverlapAdd.SetParent(this);
}

SpectralSynthesis::SpectralSynthesis() 
{
	Configure(SpectralSynthesisConfig());
	AttachChildren();
}

SpectralSynthesis::SpectralSynthesis(const SpectralSynthesisConfig& cfg)
{
	Configure(cfg);
	AttachChildren();
}

SpectralSynthesis::~SpectralSynthesis()
{
	
}

bool SpectralSynthesis::ConfigureChildren()
{
	
	//instantiate analysis window gen
	if(!mPO_AnalWindowGen.Configure(mConfig.GetAnalWindowGenerator()))
		return false;

	/*instantiate synthesis window generator*/
	if(!mPO_SynthWindowGen.Configure(mConfig.GetSynthWindowGenerator()))
		return false;
	
	//Instantiate Circular Shift
	if(!mPO_CircularShift.Configure(mConfig.GetCircularShift()))
		return false;		

	//Overlap and add PO
	if(!mPO_OverlapAdd.Configure(mConfig.GetOverlapAdd()))
		return false;

	//instantiate IFFT
	IFFTConfig IFFTCFG;
	IFFTCFG.SetName("ifft_name");
	IFFTCFG.SetAudioSize(mConfig.GetIFFT().GetAudioSize());
	if(!mPO_IFFT.Configure(IFFTCFG))
		return false;		
		
	return true;
}

void SpectralSynthesis::ConfigureData()
{
	///////////////////////////////////
	//INSTANIATE PROCESSING DATA
	///////////////////////////////////

	//intantiate audio
	mAudio1.SetSize(mConfig.GetIFFT().GetAudioSize());//audio used as output of the IFFT
	
	mAudio2.SetSize(mConfig.GetHopSize()*2);//audio used as input of the inverse + triangular windowing 
	
	mAudio3.SetSize(mConfig.GetHopSize()*2);//audio used as input of the overlap and add
	
	mSynthWindow.SetSize(mConfig.GetHopSize()*2+1);

	//INITIALIZATION OF WINDOW
	Audio tmpWindow,tmpWindow2;
	
	tmpWindow.SetSize(mConfig.GetAnalWindowSize());
	tmpWindow2.SetSize(mConfig.GetHopSize()*2+1);

	mPO_AnalWindowGen.Start();
	mPO_AnalWindowGen.Do(tmpWindow);
	mPO_AnalWindowGen.Stop();
//	CLAMGUI::showPDSnapshot(&tmpWindow,"InverseWindow");

	//We now take only the mSynthWindowSize central samples of the inverse window
	tmpWindow.GetAudioChunk((int)((TData)mConfig.GetAnalWindowSize()/2-(TData)mConfig.GetHopSize()),(int)((float)mConfig.GetAnalWindowSize()/2+(float)mConfig.GetHopSize()),mSynthWindow,false);

//	CLAMGUI::showPDSnapshot(&mSynthWindow,"Inverse Window, only central samples");

	/*Now we generate triangular synthesis window*/
	tmpWindow.SetSize(mConfig.GetHopSize()*2+1);

//	CLAMGUI::showPDSnapshot(&tmpWindow);
	mPO_SynthWindowGen.Start();
	mPO_SynthWindowGen.Do(tmpWindow);
	mPO_SynthWindowGen.Stop();
//	CLAMGUI::showPDSnapshot(&tmpWindow,"Triangular Window");
	
	/*Now we multiply both windows*/
	mPO_AudioProduct.Do(tmpWindow,mSynthWindow,mSynthWindow);

	/*now we set it to even size leaving last sample out*/
	mSynthWindow.SetSize(mConfig.GetHopSize()*2);

//	CLAMGUI::showPDSnapshot(&mSynthWindow,"Final inverse window");

}


bool SpectralSynthesis::ConcreteConfigure(const ProcessingConfig& c) throw(std::bad_cast)
{
	mConfig = dynamic_cast<const SpectralSynthesisConfig&>(c);

	//CONFIGURE CHILDREN AND DATA
	ConfigureChildren();

	ConfigureData();
	return true;
}

bool SpectralSynthesis::Do(void)
{
	return false;
}


bool SpectralSynthesis::Do(Spectrum& in, Audio& out)
{

	SpecTypeFlags tmpFlags;
	in.GetType(tmpFlags);
	if(mConfig.GetDisplayFlags().bInputFrameSpectrum)
		tmpFlags.bMagPhase=1;
	if(!tmpFlags.bComplex)
	{
		tmpFlags.bComplex=1;
	}
	in.SetTypeSynchronize(tmpFlags); //convert MagPhase data to ComplexData
//	if(mConfig.GetDisplayFlags().bInputFrameSpectrum)
//		CLAMGUI::showPDSnapshot(&in);
	
	mPO_IFFT.Do(in, mAudio1);

//	if(mConfig.GetDisplayFlags().bIFFTOutput)
//		CLAMGUI::showPDSnapshot(&mAudio1,"Output of the IFFT");
	
	mPO_CircularShift.Do(mAudio1,mAudio1);//Undoing Synthesis circular shift

//	if(mConfig.GetDisplayFlags().bAudioFrameNoCircularShift)
//		CLAMGUI::showPDSnapshot(&mAudio1,"Audio Frame without circular shift");

//Undoing zero padding by hand seems a bit ugly but...
	mAudio1.GetAudioChunk(0,mConfig.GetHopSize()*2,mAudio2,false);

//	if(mConfig.GetDisplayFlags().bAudioFrameNoZeroPadding)
//		CLAMGUI::showPDSnapshot(&mAudio2,"Audio Frame Without Zero Padding");
	
//	if(mConfig.GetDisplayFlags().bSynthesisWindow)
//		CLAMGUI::showPDSnapshot(&mSynthWindow, "Synthesis Window");

	mPO_AudioProduct.Do(mAudio2, mSynthWindow,mAudio3);//Aplying inverse window
	
//	if(mConfig.GetDisplayFlags().bWindowedAudioFrame)
//		CLAMGUI::showPDSnapshot(&mAudio3,"Audio frame with inverse windowing");

	mPO_OverlapAdd.Do(mAudio3, out);//overlapp and add
	
//	if(mConfig.GetDisplayFlags().bOutputAudioFrame)
//		CLAMGUI::showPDSnapshot(&out,"Output of the SpectralSynthesis");

	return true;
}

bool SpectralSynthesis::Do(Frame& in,bool residual)//this bool could be set in configuration
{
	if(!residual)
		return Do(in.GetSpectrum(),in.GetAudioFrame());
	else
		return Do(in.GetResidualSpec(),in.GetResidualAudioFrame());
}

bool SpectralSynthesis::Do(Segment& in,bool residual)
{
	return Do(in.GetFrame(in.mCurrentFrameIndex++),residual);
}

TInt32 SpectralSynthesis::CalculatePowerOfTwo(TInt32 size)
{
	int tmp = size;
	int outputSize = 1;
	while (tmp) 
	{
	 	outputSize=outputSize << 1;
	 	tmp=tmp >> 1;
	}
	if(outputSize == size << 1)
		outputSize = outputSize >> 1;
	return outputSize;
}
