#ifndef _TDWindowing_
#define _TDWindowing_



#include "Array.hxx"
#include "Audio.hxx"
#include "DataTypes.hxx"
#include "Defines.hxx"
#include "DynamicType.hxx"
#include "EWindowingMethod.hxx"
#include "ProcessingObject.hxx"
#include "Storage.hxx"
#include "Window.hxx"

#include "config.h"



namespace MTG {

  class TDWindowing;

  class TDWindowingConfig: public MTG::ProcessingObjectConfig
	{
	public:
    REG_NUM_ATTR_USING_INTERFACE (TDWindowingConfig, 3, MTG::ProcessingObjectConfig);
		REGISTER (0, Name, std::string);
		REGISTER (1, WindowingMethod, MTG::EWindowingMethod);
		REGISTER (2, OverlapSize, TInt32);
    
		void Init();
	};

 
  class TDWindowing: public ProcessingObject 
  {
	protected:	
		
    TDWindowingConfig mConfig;


  private:
		const char *GetClassName() const {return "TDWindowing";}

		/** Configuration change method
		 * @throw
		 * bad_cast exception when the argument is not an FFTConfig
		 * object.  
		 */
    bool ConcreteConfigure(const ProcessingObjectConfig&) throw(std::bad_cast);

	public:

		/* configuration methods */

		TDWindowing(void);

		TDWindowing(const TDWindowingConfig &c);

	  ~TDWindowing();

		const ProcessingObjectConfig &GetConfig() const { return mConfig;}

		/* do methods */

		bool Do(void);

		/** Do method
			* Applies the window function buffer to an input signal buffer and stores the
			* the result in an output signal buffer. Processing in two different modes is 
			* possible, normal and zero-phase.
			*
		  *		in:     signal input buffer
			*		out:    signal output buffer
			*   window: window function buffer
			*/

    bool Do(DataArray &in, DataArray &out, DataArray &window);

		bool Do(DataArray &in, DataArray &out, Window &window);

		bool Do(Audio &in, Audio &out, DataArray &window);

		bool Do(Audio &in, Audio &out, Window &window);

		bool Do(TData *pin, TInt32 inSize, TData *pout, TInt32 outSize, TData *pwin, TInt32 winSize);

		/* prototype methods */

    bool SetPrototypes();

		bool UnsetPrototypes();

		bool MayDisableExecution() const {return true;}

		/* storage */

    //void StoreOn(Storage &s) {};

	private:
	};
} //end of namespace MTG


#endif // _TDWindowing_
