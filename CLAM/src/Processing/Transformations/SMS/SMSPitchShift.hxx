/*
 * Copyright (c) 2001-2004 MUSIC TECHNOLOGY GROUP (MTG)
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


#ifndef _SMSPitchShift_
#define _SMSPitchShift_

#include "InPort.hxx"
#include "OutPort.hxx"
#include "InControl.hxx"
#include "FrameTransformation.hxx"
#include "SpectralEnvelopeExtract.hxx"
#include "SpectralEnvelopeApply.hxx"
#include "FDCombFilter.hxx"
#include "SpectralPeakArray.hxx"
#include "Frame.hxx"

#include "FrameTransformationConfig.hxx"
#include "SMSTransformationConfig.hxx"


namespace CLAM{


	/** 
	 *	Pitch shift with timbre preservation using the SMS model. In order to preserve timbre, 
	 *	the original spectral shape is extracted and then applied back.
	 */
	class SMSPitchShift: public FrameTransformation
	{
		const char *GetClassName() const {return "SMSPitchShift";}

		InPort<SpectralPeakArray> mInPeaks;
		OutPort<SpectralPeakArray> mOutPeaks;
		InPort<Fundamental> mInFundamental;
		OutPort<Fundamental> mOutFundamental;
		InPort<Spectrum> mInSpectrum;
		OutPort<Spectrum> mOutSpectrum;

		InControl mIsHarmonic;
		InControl mShiftAmount;

		FrameTransformationConfig mConfig;

	public:
		SMSPitchShift()
			: 
			mInPeaks("In SpectralPeaks", this), 
			mOutPeaks("Out SpectralPeaks", this), 
			mInFundamental("In Fundamental", this),
			mOutFundamental("Out Fundamental", this),
			mInSpectrum("In Spectrum", this), 
			mOutSpectrum("Out Spectrum", this), 
			mIsHarmonic("Harmonic",this),
			mShiftAmount("Shift Amount", this)
		{
			Configure( SegmentTransformationConfig() );
		}

 		~SMSPitchShift() {}

		const ProcessingConfig& GetConfig() const { return mConfig; }

	    bool ConcreteConfigure(const ProcessingConfig& c) { return true; }

		bool Do(const SpectralPeakArray& inPeaks,
		        const Fundamental& inFund,
				const Spectrum& inRes,
				SpectralPeakArray& outPeaks,
				Fundamental& outFund,
				Spectrum& outRes);

		bool Do(const Frame& in, Frame& out);

		bool Do()
		{
			bool result = Do(mInPeaks.GetData(), 
							mInFundamental.GetData(), 
							mInSpectrum.GetData(), 
							mOutPeaks.GetData(), 
							mOutFundamental.GetData(), 
							mOutSpectrum.GetData());

			mInPeaks.Consume();
			mOutPeaks.Produce();
			mInFundamental.Consume();
			mOutFundamental.Produce();
			mInSpectrum.Consume();
			mOutSpectrum.Produce();

			return result;
		}

	private:
		SpectralEnvelopeExtract mSpectralEnvelopeExtract;
		SpectralEnvelopeApply mSpectralEnvelopeApply;
		FDCombFilter mFDCombFilter;
		Spectrum mSpectralEnvelope;
	};		
}	//namespace CLAM

#endif // _SMSPitchShift_

