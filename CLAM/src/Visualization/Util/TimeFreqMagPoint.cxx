#include "TimeFreqMagPoint.hxx"

namespace CLAMVM
{

	TimeFreqMagPoint::TimeFreqMagPoint()
		: mAttribute( 0 ), mTimeIndex( -1 )
	{
	}
		
	TimeFreqMagPoint::TimeFreqMagPoint( const TimeFreqMagPoint& p )
	{
		mMag = p.mMag;
		mFreq = p.mFreq;
		mTimeIndex = p.mTimeIndex;
	}
		
	const TimeFreqMagPoint& TimeFreqMagPoint::operator=( const TimeFreqMagPoint& p )
	{
				
		return *this;
	}

	TimeFreqMagPoint::~TimeFreqMagPoint()
	{
	}

}
