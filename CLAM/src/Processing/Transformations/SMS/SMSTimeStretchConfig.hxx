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
#ifndef __SMSTIMESTRETCHCONFIG__
#define __SMSTIMESTRETCHCONFIG__

#include "Processing.hxx"
#include "BPF.hxx"


namespace CLAM
{
	class SMSTimeStretchConfig: public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (SMSTimeStretchConfig, 6,ProcessingConfig);
		/** Name of the SMSTransformation object*/
		DYN_ATTRIBUTE (0, public, std::string, Name);
		/** Type of transformation, for the time being just a string, should become
		* an enumeration of known transformation types??*/
		DYN_ATTRIBUTE (1, public, std::string, Type);
		/** Single Value Parameter */
		DYN_ATTRIBUTE (2, public, TData, Amount);
		/** BPF (envelope-like) Parameter */
		DYN_ATTRIBUTE (3, public, BPF, BPFAmount);
		DYN_ATTRIBUTE (4, public, TData, SamplingRate);
		DYN_ATTRIBUTE (5, public, TData, HopSize);
		
	protected:
		void DefaultInit();

		void DefaultValues();
	public:
		~SMSTimeStretchConfig();
	};
	
}

#endif // SMSTimeStretchConfig.hxx
