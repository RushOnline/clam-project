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

#ifndef _WAVE_GENERATOR_H
#define _WAVE_GENERATOR_H

#include "Audio.hxx"
#include "Processing.hxx"
#include "Port.hxx"

namespace CLAM {

	class EWaveType : public Enum {
	public:
		
		static tEnumValue sEnumValues[];
		static tValue sDefault;
		EWaveType() : Enum(sEnumValues, sDefault) {}
		EWaveType(tValue v) : Enum(sEnumValues, v) {};
		EWaveType(std::string s) : Enum(sEnumValues, s) {};
		
		typedef enum {
			eSine
		};
		
		virtual Component* Species() const
		{
			return (Component*) new EWaveType(eSine);
		};
	};


	template<typename EWaveType::tValue V> 
	class WaveFunctor {
	public:
		TData operator()(TTime x,TData amplitude);
	};

	class WaveGeneratorConfig: public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (WaveGeneratorConfig, 7, ProcessingConfig);
		DYN_ATTRIBUTE (0, public, std::string, Name);
		DYN_ATTRIBUTE (1, public, EWaveType,   WaveType);
		DYN_ATTRIBUTE (2, public, TData,       Frequency);
		DYN_ATTRIBUTE (3, public, TData,       Amplitude);
		DYN_ATTRIBUTE (4, public, TData,       Phase);
		DYN_ATTRIBUTE (5, public, TData,       SampleRate);
		DYN_ATTRIBUTE (6, public, int,         FrameSize);
	protected:
		void DefaultInit(void);
	};

	class WaveGenerator: public Processing
	{
	protected:
		WaveGeneratorConfig mConfig;
	private:

		TData mAmplitude;

		TData mXPos;    // Radians
		TData mXDelta;  // Radians

		EWaveType::tValue mType;

		const char *GetClassName() {return "WaveGenerator";}

		/** Configuration change method
		 * @throw
		 * bad_cast exception when the argument is not an FFTConfig
		 * object.
		 */
		bool ConcreteConfigure(const ProcessingConfig&) throw(std::bad_cast);

		inline TData Sine(TTime pos);

	public:

		OutPortTmpl<Audio> Output;

		WaveGenerator();

		WaveGenerator(const WaveGeneratorConfig &c);

		virtual ~WaveGenerator();

		/** Configuration access:
		 */
		const ProcessingConfig &GetConfig() const { return mConfig;}

		void Attach(Audio& out) {Output.Attach(out);}

		/** Supervised-mode Do function.
		 */
		bool Do(void);

		/** Standard Do function, with storage class references as
		 * arguments. This method implements the old conversor routines.
		 */
		bool Do(Audio& in);

		/** @todo Not yet implemented. */
		void StoreOn(Storage &s) {};

	private:

		template<typename EWaveType::tValue V> 
		void FillBuffer(Array<TData> &buffer)
		{
			int i;
			WaveFunctor<V> func;
			int size = buffer.Size();
			for (i=0; i<size; i++) {
				buffer[i] = func(mXPos,mAmplitude);
				mXPos += mXDelta;
			}
			if (mXPos > 2.0 * M_PI)
				mXPos = fmod(mXPos,2.0 * M_PI);
		}
 	
	};
}//namespace CLAM

#endif // _WaveGenerator_
