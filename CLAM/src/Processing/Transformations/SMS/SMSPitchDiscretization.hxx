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


#ifndef _SMSPitchDiscretization_
#define _SMSPitchDiscretization_

#include "FrameTransformation.hxx"
#include "SMSPitchShift.hxx"
#include "SpectrumAdder2.hxx"


namespace CLAM{


	class SMSPitchDiscretization: public FrameTransformation
	{

		SMSPitchShift mPitchShift;

	public:

		SMSPitchDiscretization() {}

		const ProcessingConfig& GetConfig() const { throw 0; }

		bool ConcreteConfigure(const ProcessingConfig& c) { return true; }

 		~SMSPitchDiscretization() {}

		bool Do()
		{
			CLAM_ASSERT(false, "Do with ports not implemented");
			return false;
		}
		bool Do(const Frame& in, Frame& out);

	private:

		const char *GetClassName() const {return "SMSPitchDiscretization";}
		
	};		
} //namespace CLAM

#endif // _SMSPitchDiscretization_

