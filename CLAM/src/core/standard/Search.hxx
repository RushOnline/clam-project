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

#ifndef _Search_
#define _Search_

#include "OSDefines.hxx"
#include "DataTypes.hxx"

namespace CLAM {

template <class U, class T> class Search
{
/* Based on locate() and hunt(), Numerical Recipes,second Edition, 117 */
private:
	const U* mpData;
public:
	Search()
	{
		mpData = NULL;
	}
	Search(const U& array)
	{
		Set(array);
	}
	~Search()
	{

	}
	void Set(const U& array)
	{
		mpData = &array;
	}
	TIndex Find(const T& value,TIndex prevIndex=0) const
	{
		return Hunt(value,prevIndex);
	}
	TIndex Hunt(const T& value,TIndex prevIndex=0) const;
	
	TIndex Bisection(const T& value) const
	{
		return Bisection(value,0,mpData->Size());
	}
	TIndex Bisection(const T& value,TIndex lowerLimit) const
	{
		return Bisection(value,lowerLimit,mpData->Size());
	}
	TIndex Bisection(const T& value,TIndex lowerLimit,TIndex upperLimit) const;

};

template <class U, class T> 
	inline TIndex Search<U,T>::Hunt(
		const T& value,TIndex guessIndex) const
{
	TIndex result;
	TIndex n = mpData->Size();
	if (guessIndex<0 || guessIndex>=n)
	{
		return Bisection(value);
	}
	bool ascending = (*mpData)[n-1] >= (*mpData)[0];
	TIndex inc = 1;
	TIndex upperLimit;
	if ( (value >= (*mpData)[guessIndex]) == ascending)
	{
		if (guessIndex == n-1)
			return -1;//X.A. changed to -1 for consistency
		upperLimit = guessIndex+1;
		while (	(value >= (*mpData)[upperLimit]) == ascending)
		{
			guessIndex = upperLimit;
			inc += inc;
			upperLimit += inc;
			if (upperLimit >= n) {
				result=Bisection(value,guessIndex);
				if(result==n-1) return -1;
				else return result;
			}
		}
	} else {
		if (guessIndex==0) return -1;
		upperLimit = guessIndex--;
		while (	(value < (*mpData)[guessIndex]) == ascending)
		{
			upperLimit = guessIndex;
			inc += inc;
			if (inc >= upperLimit) {
				result = Bisection(value,0,upperLimit);
				if(result==0) return -1;
				else return result;
			}
			guessIndex = upperLimit-inc;
		}
	}
	return Bisection(value,guessIndex,upperLimit);
}

template <class U, class T> 
	inline TIndex Search<U,T>::Bisection(
		const T& value,TIndex lowerLimit,TIndex upperLimit) const
{
	lowerLimit--;
	bool ascending = (*mpData)[mpData->Size()-1] >= (*mpData)[0];
	while (upperLimit-lowerLimit > 1)
	{
		TIndex midPoint = (upperLimit+lowerLimit)>>1;
		if ( (value >= (*mpData)[midPoint]) == ascending)
			lowerLimit = midPoint;
		else
			upperLimit = midPoint;
	}
	return lowerLimit;
}

}

#endif

