/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef __TTIMEFREQMAGPOINT__
#define __TTIMEFREQMAGPOINT__

#include <CLAM/DataTypes.hxx>

namespace CLAM
{
	namespace VM
	{
		struct TimeFreqMagPoint
		{
			TData   mMag;
			TData   mFreq;
			TData   mAttribute;
			TIndex  mTimeIndex;
			TIndex  mColorIndex;

			TimeFreqMagPoint();

			TimeFreqMagPoint( const TimeFreqMagPoint& );

			const TimeFreqMagPoint& operator=( const TimeFreqMagPoint& );

			~TimeFreqMagPoint();

			bool isValid() const
				{
					return ( mTimeIndex >= 0 );
				}

			bool operator==( TIndex i ) 
				{
					return mTimeIndex==i;
				}

			bool operator==( TIndex i ) const
				{
					return mTimeIndex==i;
				}

			bool operator<( TIndex i )
				{
					return mTimeIndex<i;
				}
					
			bool operator<( TIndex i ) const
				{
					return mTimeIndex<i;
				}

			bool operator>( TIndex i )
				{
					return mTimeIndex>i;
				}
					
			bool operator>( TIndex i ) const
				{
					return mTimeIndex>i;
				}
					

		};
	}
}

#endif 

