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

///////////////////////////////////////////////////////////////////////////////
// Class SpectralPeak: Processing Data class to store spectral peak data
///////////////////////////////////////////////////////////////////////////////
// Description: 
//  This class holds a basic data structure used for spectral processing: the
//  the Spectral Peak. A Spectral Peak holds information about a particular
//  spectral sample extracted from a Peak Detection Algorithm. Some information
//  such as Frequency and Amplitude is mandatory, other as Phase and BinWidth
//  may be needed for some particular applications and BinPos may be redundant
//  with Frequency but is kept for backwards compatibility.
///////////////////////////////////////////////////////////////////////////////


#ifndef _SpectralPeak_
#define _SpectralPeak_

#include "DynamicType.hxx"
#include "Array.hxx"
#include "DataTypes.hxx"
#include "OSDefines.hxx"
#include "Err.hxx"
#include "ProcessingData.hxx"
#include "GlobalEnums.hxx"

namespace CLAM{



/**
* Class <b>SpectralPeak<b> is a basic spectral domain Processing Data class. 
* Contains frequency, magnitude, phase, bin position and bin width of a given
* spectral peak. An array of Spectral Peaks is created by means of the 
* SpectralPeakArray class
* @see ProcessingData
* @see SpectralPeakArray
* @author Xavier Amatriain
*/
class SpectralPeak: public ProcessingData
{
public:
	DYNAMIC_TYPE_USING_INTERFACE (SpectralPeak, 6, ProcessingData);
	DYN_ATTRIBUTE (0, public, EScale, Scale);
	DYN_ATTRIBUTE (1, public, TData, Freq);
	DYN_ATTRIBUTE (2, public, TData, Mag);
	DYN_ATTRIBUTE (3, public, TData, BinPos);
	DYN_ATTRIBUTE (4, public, TData, Phase);
	DYN_ATTRIBUTE (5, public, int, BinWidth);


protected:
/** Configuration constructor
*/
	void DefaultInit();

public:
//operators

/** Operator for computing distance between spectral peaks only taking into
* account their frequency
* @return the frequency distance in Hz (TData)
*/
	TData operator|(const SpectralPeak& a) const
	{
		return (Abs(GetFreq()-a.GetFreq()));
	}
/** Operator for multiplying a spectral peak by a given factor, just influences
* its magnitude
* TODO: does not take into account if the scale is logarithmic or not, should it?
* @param A reference to the peak
* @param the multiplying factor
*/
	friend  SpectralPeak operator * (const SpectralPeak& a,int factor)
	{
		SpectralPeak ret;
		ret.SetFreq(a.GetFreq());
		ret.SetMag(a.GetMag()*factor);
		ret.SetPhase(a.GetPhase());
		ret.SetBinPos(a.GetBinPos());
		ret.SetBinWidth(a.GetBinWidth());
		ret.SetScale(a.GetScale());
		return ret;
	}


};

/** Function for changing the scale of a peak to linear, conserving data consistency
* @param inPeak: incoming peak
* @param factor to apply in the logarithmic conversion (default 20)
*/
inline SpectralPeak Lin(SpectralPeak &inPeak,int scalingFactor=20)
{
	if (inPeak.GetScale()!=EScale::eLinear){
		TData currMag = inPeak.GetMag();
		inPeak.SetMag(pow(10,(currMag/scalingFactor)));
		inPeak.SetScale(EScale::eLinear);
	}
	return inPeak;
}

/** Function for changing the scale of a peak to log, conserving data consistency
* @param inPeak: incoming peak
* @param factor to apply in the logarithmic conversion (default 20)
*/
inline SpectralPeak DB(SpectralPeak& inPeak,int scalingFactor=20)
{
	if (inPeak.GetScale()!=EScale::eLog){
		TData currMag = inPeak.GetMag();
		inPeak.SetMag(scalingFactor*log10(currMag));
		inPeak.SetScale(EScale::eLog);
	}
	return inPeak;
}

};//namespace

#endif
