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

#ifndef _CIRCULAR_SHIFT_H_
#define _CIRCULAR_SHIFT_H_

#include "Processing.hxx"
#include "DynamicType.hxx"
#include "Spectrum.hxx"
#include "Audio.hxx"




namespace CLAM {

	
	class CircularShiftConfig: public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (CircularShiftConfig, 2,ProcessingConfig);
		DYN_ATTRIBUTE (0, public, std::string, Name);
		/* Right now you can change the amount any time but it will only be taken into
		account if Type is set to eCustom */
		DYN_ATTRIBUTE (1, public, TData, Amount);

		~CircularShiftConfig(){};
	protected:
		void DefaultInit();


	};

	class CircularShift: public Processing {
		CircularShiftConfig mConfig;

		const char *GetClassName() {return "CircularShift";}

		/** Config change method
		 * @throw
		 * bad_cast exception when the argument is not an SpecAdderConfig
		 * object.
		 */
		bool ConcreteConfigure(const ProcessingConfig&) throw(std::bad_cast);

	public:
		CircularShift();

		CircularShift(const CircularShiftConfig &c);

		~CircularShift();

		const ProcessingConfig &GetConfig() const { return mConfig;}

		bool Do(void);

		bool Do(DataArray& input, DataArray& out);
		bool Do(Spectrum& input, Spectrum& out);
		bool Do(Audio& input, Audio& out);

		// Port interfaces.

		/** Change the internal type state.
		 * Apart from prototype configuration, the Size, Scale and
		 * SpectralRange attributes of each Spectrum are also
		 * checked.
		 */

		bool SetPrototypes(Spectrum& inputs,const Spectrum& out);

		bool SetPrototypes();

		bool UnsetPrototypes();

		bool MayDisableExecution() const {return true;}

		void StoreOn(Storage &s) {};
		void SetAmount(TSize amount) { mAmount.DoControl(TControlData(amount)); }


	private:
		InControl    mAmount;
	};

}

#endif // _CIRCULAR_SHIFT_H_
