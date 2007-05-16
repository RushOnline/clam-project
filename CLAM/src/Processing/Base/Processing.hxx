/*
 * Copyright (c) 2001-2005 MUSIC TECHNOLOGY GROUP (MTG)
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


#ifndef _Processing_hxx_
#define _Processing_hxx_


#include "ErrProcessingObj.hxx"
#include "InControlRegistry.hxx"
#include "OutControlRegistry.hxx"
#include "InPortRegistry.hxx"
#include "OutPortRegistry.hxx"
#include "ProcessingConfig.hxx"
#include "NullProcessingConfig.hxx"

#include <list>
#include <typeinfo>
#include <string>

namespace CLAM {

	class Processing;
	class InPortBase;
	class OutPortBase;
	class InControl;
	class OutControl;
	class ProcessingComposite;

	/**
	 * Connects two ports of two processings selecting them by the port name.
	 * Short hand for sender.GetOutPort(outPortName).ConnectToIn(receiver.GetOutPort(inPortName))
	 */
	void ConnectPorts(
			Processing & sender, const std::string & outPortName, 
			Processing & receiver, const std::string & inPortName );
	/**
	 * Connects two ports of two processings selecting them by the port number.
	 * Short hand for sender.GetOutPort(outPortName).ConnectToIn(receiver.GetOutPort(inPortName))
	 */
	void ConnectPorts(
			Processing & sender, unsigned outPortNumber, 
			Processing & receiver, unsigned inPortNumber );
	/**
	 * Free function that connects two controls.
	 * Short hand for sender.GetOutControls().GetByNumber(outControlNumber).AddLink( receiver.GetOutControls().GetByNumber(inControlNumber))
	 */
	void ConnectControls(
			Processing & sender, unsigned outControlNumber, 
			Processing & receiver, unsigned inControlNumber );
	/**
	 * Connects two controls of two processings selecting them by the control name.
	 * Short hand for sender.GetOutControls().GetByName(outControlName).AddLink( receiver.GetOutControls().GetByName(inControlName))
	 */
	void ConnectControls(
			Processing & sender, const std::string & outControlName, 
			Processing & receiver, const std::string & inControlName );
		
	/**
	 * Connects a free port to one belonging to a processing selecting it by the port number.
	 * Short hand for sender.ConnectToIn(receiver.GetOutPort(inPortName))
	 */
	void ConnectPorts(
			OutPortBase & sender, 
			Processing & receiver, unsigned inPortNumber );
	/**
	 * Connects a processing port, selected by number, to a free in port.
	 * Short hand for sender.GetOutPort(outPortName).ConnectToIn(receiver)
	 */
	void ConnectPorts(
			Processing & sender, unsigned outPortNumber, 
			InPortBase & receiver );
	/**
	 * The base class for all the CLAM processing object classes.
	 *
	 * Processing is the base class for all the CLAM processing object classes.
	 * It defines their shared interface and contains common infrastructure.
	 * Processings are the building blocks for any CLAM system,
	 * they are an abstraction of an audio/music processing step.
	 * 
	 * Whenever the Do method is called, the processing consumes
	 * data tokens from its input ports and produces data tokens 
	 * for its output ports performing a single processing step.
	 * Each port is related to a given C++ data type and the number
	 * of tokens that are seen/consumed/produced at every Do step
	 * is flexible, even at running mode.
	 *
	 * A processing can also receive an event from another one in an
	 * asyncronous way in order to change the way the processing is
	 * done. 
	 * Asyncronous comunication is done using the Control abstraction.
	 * 
	 * @see InPortBase, OutPortBase, InControl, OutControl
	 *
	 * An initial setup is needed before using any processing.
	 * You can specify this initial setup by providing the
	 * procesing with a configuration object (an instance of the
	 * proper ProcessingConfig subclass) containing all the
	 * parameters.
	 *
	 * @see ProcessingConfig
	 *
	 * @dot
	 * digraph example
	 * {
	 * 	bgcolor="#ffffaa";
	 * 	rankdir=LR;
	 * 	node [shape=ellipse, style=filled, fillcolor="#ccffcc", color="#558855", fontname=Arial, fontsize=10];
	 * 	edge [color=red ];
	 * 	Ready        -> Running [ label="Start"     URL="\ref Start" ];
	 * 	Unconfigured -> Ready   [ label="Configure" URL="\ref Configure" ];
	 * 	Running      -> Ready   [ label="Stop"      URL="\ref Stop" ];
	 * 	Running      -> Running [ label="Do"        URL="\ref Do" ];
	 * }
	 * @enddot
	 */
	class Processing {
	public:
		/** Processing Object possible execution states. */
		typedef enum {
			Unconfigured=0,
			Ready, 
			Running
		} ExecState;

		typedef NullProcessingConfig Config;

// Basic usage interface:

	public:

		/** Configuration change method.
		 *  This is the method to be used in order to update the
		 *  configuration of a processing object. Note that the object
		 *  must NOT be running when calling this method.
		 *  This method performs some execution state checkings and
		 *  updates, and calls the ConcreteConfigure method of the
		 *  concrete class to perform the actual configuration.
		 *  @param config Reference to the configuration object.
		 *  @throw ErrProcessingObj if the processing object is in
		 *  running or disabled state, or if the argument is not
		 *  an object of the configuration class matching the concrete
		 *  processing class of the processing object.
		 */
		bool Configure(const ProcessingConfig& config);

		/** Method to turn the object into running state.
		 * This method must be called before any call to Do() methods.
		 * @pre the processing object is in ready state
		 */
		void Start(void);

		/**
		 * Supervised mode execution method (using ports)
		 * @return A boolean telling whether it has been output
		 */
		virtual bool Do(void)=0;

		/** Method to put the object out of running state When in
		 * execution mode, this method must be called before any
		 * further call to Configure() methods
		 * @pre The processing object is in runnig state (or disabled).
		 */
		void Stop(void);

		Processing();
		virtual ~Processing();

// Overridable interface:
	public:

		/** Override it in every subclass and retur the name of that class */
		virtual const char * GetClassName() const = 0;
		
		/** Override this method if your processing cannot process inplace*/
		virtual bool CanProcessInplace() { return true; }

	protected:
		/** Configuration method interface.
		 * The Processing base class forces all the concrete
		 * classes derived from it to implement this method, which
		 * must actually perform the specific configuration
		 * tasks.
		 * <p> Note that the user can not call this method
		 * directly. He will use Configure instead. The argument is
		 * expected to be an object of the necesary concrete configuration
		 * class.
		 * @param Reference to the configuration object.
		 * @return <b>false</b> if the object is not yet fully configured.
		 * (For example, if some important configuration attribute
		 * such as number of ports was set to zero in the config
		 * object)
		 * <p> <b> true </b> if the processing object is left in a
		 * consistent state, and can be executed.
		 * @throw This method must throw a bad_cast exception if the
		 * argument is not an object of the expected configuration class.
		 * */
		virtual bool ConcreteConfigure(const ProcessingConfig&) = 0; 
		
		/**
		 * Processing objects have to redefine this method when starting
		 * them implies some internal changes. ie: adquiring resources.
		 * @returns Whether start changes have been successful
		 */
		virtual bool ConcreteStart() {return true;};

		/**
		 * Processing objects have to redefine this method when stoping
		 * them implies some internal changes. ie: releasing resources.
		 * @returns Whether stop changes have been successful
		 */
		virtual bool ConcreteStop() {return true;};

// Public interface:	
	public:
		/** Check that Supervised Do() can be safely called */
		bool CanConsumeAndProduce();

		/** Configuration getter.
		 * Gets the configuration parameters used to create the object.
		 * @return Const reference to the configuration object. The
		 * usual way to perform a configuration change in the
		 * processing object is to take a copy of this reference, to
		 * change it and to send it with the Configure method.
		 * If not overriden, it returns a NullProcessingConfig.
		 */
		virtual const ProcessingConfig &GetConfig() const;

		/** State getter.
		 * Returns the object execution state. This may be used for
		 * debugging purposes, and should also be used inside the
		 * execution (Do) methods to confirm that the object is either
		 * in running or in disabled state.
		 */
		ExecState GetExecState() const {return mExecState;}
		
		std::string GetExecStateString() const;

		void PublishOutPort(OutPortBase* out);
		void PublishInPort(InPortBase* in);
		void PublishOutControl(OutControl* out);
		void PublishInControl(InControl* in);

		void SetParent(Processing *p);

		/**
		 * This method is used to determine if a given processing can change its interface of ports/controls after
		 * its construction (i.e. changing the name of ports in ConcreteConfigure). If a concrete processing
		 * can do this, it should reimplement the method returning true, in order to notify networks, graphical
		 * interfaces, etc.
		 */
		virtual bool ModifiesPortsAndControlsAtConfiguration()	{ return false;	}
	
		bool HasInPort( const std::string & name )
		{
			return mInPortRegistry.Has(name);
		}


		bool HasOutPort( const std::string & name )
		{
			return mOutPortRegistry.Has(name);
		}

		bool HasInControl( const std::string & name )
		{
			return mInControlRegistry.Has(name);
		}

		bool HasOutControl( const std::string & name )
		{
			return mOutControlRegistry.Has(name);
		}

		InPortBase & GetInPort( const std::string & name )
		{
			return mInPortRegistry.Get(name);
		}
		OutPortBase & GetOutPort( const std::string & name )
		{
			return mOutPortRegistry.Get(name);
		}
		InControl & GetInControl( const std::string & name )
		{
			return mInControlRegistry.Get(name);
		}		
		OutControl & GetOutControl( const std::string & name )
		{
			return mOutControlRegistry.Get(name);
		}

		/** Accessor to published Controls manager */
		InControlRegistry& GetInControls() { return mInControlRegistry; }
		
		/** Accessor to published Controls manager */
		OutControlRegistry& GetOutControls() { return mOutControlRegistry; }
		
		/** Accessor to published Ports manager */
		InPortRegistry& GetInPorts() { return mInPortRegistry; }
		
		/** Accessor to published Portss manager */
		OutPortRegistry& GetOutPorts() { return mOutPortRegistry; }

		/** Returns a string describing configuration errors if any */
		const std::string& GetConfigErrorMessage() const { return mConfigErrorMessage; }

		/** Wether the processing is a sync source such as audio i/o device,
		 * or an audio callback hook (i.e. Externalizer) */
		virtual bool IsSyncSource() const { return false; }

// Helpers only for subclasses
	protected:
		
		/** Method to prepend a message to mConfigErrorMessage
		* CLAM_ASSERT */
		void AddConfigErrorMessage( const std::string& msg );
	
		/** In debug-mode checks that the processing is configured and started. 
		 * And always checks it's not disabled */
		bool AbleToExecute(void) const;

		/**
		 * Helper template to convert a reference to a ProcessingConfig to the concrete
		 * ProcessingConfig specified on the first parameter.
		 * @param concrete The copy destination (it forces the runtime type for abstract)
		 * @param abstract A reference to the configuration to be copied
		 * @pre The object runtime type must be exactly the type required by the first parameter
		 */
		template <typename ConcreteConfig>
		void CopyAsConcreteConfig(ConcreteConfig & concrete, const ProcessingConfig & abstract) const;

// Attributes:
	protected:
		/** Pointer to the parent (composite) processing object, or 0 */
		ProcessingComposite *mpParent;

		/** Processing object execution state */
		ExecState mExecState;

		/** Status description, for debugging */
		std::string mConfigErrorMessage;


	private:
		InControlRegistry mInControlRegistry;
		OutControlRegistry mOutControlRegistry;
		InPortRegistry mInPortRegistry;
		OutPortRegistry mOutPortRegistry;
	};

	

// -----------------------------------------------------------------------------------------------	
// Inline implementations

inline bool Processing::AbleToExecute(void) const
{
	CLAM_BEGIN_DEBUG_CHECK
		if (GetExecState() != Running)
		{
			std::string err(GetClassName());
			err += ": Do(): Not in execution mode - did you call Start on this "
					"object, the composite it is in, or the ToplevelProcessing singleton?";
				CLAM_DEBUG_ASSERT( false, err.c_str() );
		}
	CLAM_END_DEBUG_CHECK
	return GetExecState() == Running;
}

template <typename ConcreteConfig>
inline void Processing::CopyAsConcreteConfig(ConcreteConfig & concrete, const ProcessingConfig & abstract) const 
{
	CLAM_ASSERT(typeid(ConcreteConfig)==typeid(abstract), 
		"Configuring a Processing with a configuration not being the proper type.");
	concrete = static_cast<const ConcreteConfig &>(abstract);
}

};//namespace CLAM

#endif

