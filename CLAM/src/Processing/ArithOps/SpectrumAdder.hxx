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

#ifndef _SPECTRUM_ADDER_
#define _SPECTRUM_ADDER_

#include "Processing.hxx"
#include "DynamicType.hxx"
#include "Port.hxx"
#include "Spectrum.hxx"

namespace CLAM {


	class SpecAdderConfig: public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (SpecAdderConfig, 2,ProcessingConfig);
		DYN_ATTRIBUTE (0, public, std::string, Name);
		DYN_ATTRIBUTE (1, public, int, NInputs);
	protected:
		void DefaultInit();

	};

/** This class performs the sum of N Spectrum processing data
 * objects.
 * <p>
 * It Allows any possible attribute configuration in its inputs and in
 * its output, but it performs faster when prototype configuration of
 * the data is specified using SetPrototypes(...), in certain
 * situations:
 * <ul>
 * <li> When all the inputs and the outputs have a common attirbute
 *      (not the BPF), and the same scale.
 * <li> In other cases, at least a vector conversion will be executed
 *      in one of the involved processing data objects. In some bad
 *      situations two conversions might be needed.
 * </ul><p>
 * @todo
 * <ul>
 * <li> Operation when any of the inputs has only a BPF attribute is
 * not implemented.
 * <li> Operation when not all the scales are the same is not implemented.
 * </ul>
 * Possible optimisations (which require more states):
 * <ul>
 * <li> Implement direct sum routines with inputs/outpust in
 *      different formats, and add the corresponding prototype states.
 * <li> Expand the state space to avoid checking if the attribute to be
 *      used in the computation is instantiated in each of the objects.
 *      Right now the same state is used when the three objects have a
 *      common attribute, and when a common attribute is to be used, but
 *      some of the objects lack it (and need format conversion).
 * </ul>
 **/
	class SpectrumAdder: public Processing {
		SpecAdderConfig mConfig;

		/** Size of the input/output vectors */
		int mSize;

		/** Number of inputs */
		int mNInputs;

		/** Input pointer vector */
		InPortTmpl<Spectrum> **mInputs;

		/** Aux. Structures */
		Complex **complex_bufs;
		Polar **polar_bufs;
		TData **mag_bufs;
		TData **phase_bufs;
		bool *remove;

		OutPortTmpl<Spectrum> mOut;

		/** Possible configuration/prototype states */
		typedef enum {
			// Type states in with the same attribute is used for all
			// of the inputs and the outputs (it may or may not be
			// present; in the second case it will be added at Do(...)
			// time.
			SMagPhase, SComplex, SPolar,

			// State when any of the inputs has only a BPF instantiated.
			ShasBPF,

			// State in which nothing is known about prototypes.
			SOther
		} PrototypeState;

		/** Possible scale combinations */
		typedef enum { Slin, Slog} ScaleState;

		/** Config/Prototype state */
		PrototypeState mProtoState;
		/** Scale combination state */
		ScaleState mScaleState;

		/** OBSOLETE */
		std::string NewUniqueName();

		const char *GetClassName() {return "SpectrumAdder";}

		/** Config change method
		 * @throw
		 * bad_cast exception when the argument is not an SpecAdderConfig
		 * object.
		 */
		bool ConcreteConfigure(const ProcessingConfig&) throw(std::bad_cast);

	public:
		SpectrumAdder();

		SpectrumAdder(const SpecAdderConfig &c);

		~SpectrumAdder();

		const ProcessingConfig &GetConfig() const { return mConfig;}

		bool Do(void);

		bool Do(Spectrum **inputs, Spectrum& out);

		// Port interfaces.

		/** Change the internal type state.  
		 * Apart from prototype configuration, the Size, Scale and
		 * SpectralRange attributes of each Spectrum are also
		 * checked.
		 */

		bool SetPrototypes(Spectrum **inputs,const Spectrum& out);

		bool SetPrototypes();

		bool UnsetPrototypes();

		bool MayDisableExecution() const {return true;}

		void StoreOn(Storage &s) {};

	private:

		/** Unoptimised internal multiplication method, when
		 * prototypes are not known (state SOther)
		 */
		inline void Add(Spectrum **inputs, Spectrum& out);

		// Adder methods for optimized configurations of the inputs/output

		// Direct sums
		inline void AddMagPhase(Spectrum **inputs, Spectrum& out);
		inline void AddMagPhaseLin(Spectrum **inputs, Spectrum& out);
		inline void AddMagPhaseLog(Spectrum **inputs, Spectrum& out);
		inline void AddComplex(Spectrum **inputs, Spectrum& out);
		inline void AddComplexLin(Spectrum **inputs, Spectrum& out);
		inline void AddComplexLog(Spectrum **inputs, Spectrum& out);
		inline void AddPolar(Spectrum **inputs, Spectrum& out);
		inline void AddPolarLin(Spectrum **inputs, Spectrum& out);
		inline void AddPolarLog(Spectrum **inputs, Spectrum& out);
	};

}

#endif // _SPECTRUM_ADDER_
