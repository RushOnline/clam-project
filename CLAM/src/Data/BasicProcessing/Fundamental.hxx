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

#ifndef _Fundamental_
#define _Fundamental_

#include "DynamicType.hxx"
#include "Array.hxx"
#include "DataTypes.hxx"
#include "OSDefines.hxx"
#include "Err.hxx"
#include "ProcessingData.hxx"


namespace CLAM{



////////////////////////////////////////////////////////////////////////////////////////////////
// Class Fundamental:
//

/**class Fundamental, used to manage the bins candidates to be fundamental frequencies, and their errors.
There are getters and setters,but for efficiency work directly on the buffer
@see Frame
*/
class Fundamental: public ProcessingData
{
public:
	DYNAMIC_TYPE_USING_INTERFACE (Fundamental, 4, ProcessingData);
	/** Maximum number of candidates wanted */
	DYN_ATTRIBUTE (0, public, int, nMaxCandidates);
	/** Current(initial) number of candidates */
	DYN_ATTRIBUTE (1, public, int, nCandidates);
	/** Array with the candidate frequencies*/
	DYN_ATTRIBUTE (2, public, DataArray, CandidatesFreq);
	/** Array with the error of candidate frequencies*/
	DYN_ATTRIBUTE (3, public, DataArray, CandidatesErr);
protected:
	
	void DefaultInit();
public:


  //Accessory getters and setters, for efficiency work directly on the buffer

	
	/** Returns the Frequency of a given position in the candidate array
	 *  @param pos Position we want to get frequency, 0 by default
	 *  @return The frequency in the position indicated
	 */

	TData GetFreq(TIndex pos=0);

	/** Returns the Error of a given position in the candidate array
	 *  @param pos Position we want to get frequency, 0 by default
	 *  @return The error estimated in this position
	 */
	TData GetErr(TIndex pos=0);


	/** Changes the frequency of a candidate at a given index
	 *  @param pos Position to change the frequency
	 *  @param newFreq New value of this position
	 */
	void SetFreq(TIndex pos,TData newFreq);

	/** Changes the error of a candidate at a given index
	 *  @param pos Position to change the error
	 *  @param newFreq New value of this position
	 */
	void SetErr(TIndex pos,TData newErr);
	
	// Other functionality
	
       /** Adds a candidate, in frequencies array and errors array
	*  @param freq The frequency value of new candidate, 0 by default
	*  @param err The estimated error of new candidate, 0 by default
	*/
	void AddElem (double freq=0, double err=0);

       /** Adds a candidate, in frequencies array and errors array, and uses parameter pos to insert it in the arrays.
	*  @param pos Position where we want to insert the new element
	*  @param freq The frequency value of new candidate, 0 by default
	*  @param err The estimated error of new candidate, 0 by default
	*/
	void InsertElem (TIndex pos, double freq=0, double err=0);

	/** Delete an element of both arrays (frequency and error) given its position
	 *  @param Position that we wanto to eliminate
	 */
	void DeleteElem (TIndex pos);

  
	/** Exchange two array elements
	 *  @param pos1 Element to be changed
	 *  @param pos2 The other element of the exchange
	 */
	void Exchange(TIndex pos1, TIndex pos2); // Exchange two array elements
  
	// Ordering functions

	/** Sort the arrays by frequency*/
	void SortByFrequency();

	/** Sort the arrays by estimated error*/
	void SortByError();

	/** Initializes data */
	void Init()
	{
		GetCandidatesFreq().SetSize(0);
		GetCandidatesErr().SetSize(0);
		SetnCandidates(0);
	}

};

}; // namespace CLAM

#endif
