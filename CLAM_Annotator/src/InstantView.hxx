#ifndef InstantView_hxx
#define InstantView_hxx

#include <CLAM/DynamicType.hxx>

#include <list>
#include <CLAM/Text.hxx>
#include <CLAM/Enum.hxx>

#include <CLAM/Assert.hxx>

#include "Enumerated.hxx"
#include "TypePlugin.hxx"

namespace CLAM_Annotator{
	
	class InstantView:public CLAM::DynamicType
	{
		DYNAMIC_TYPE(InstantView,3);
		DYN_ATTRIBUTE(0, public, std::string, Type); ///< The type name. Should be at the Type factory.
		DYN_ATTRIBUTE(1, public, std::string, AttributeName); ///< The attribute name, unique within the scope.
		DYN_ATTRIBUTE(2, public, std::string, AttributeScope); ///< The scope name at which the attribute sticks.
		
		void DefaultInit()
		{
			AddAll();
			UpdateData();
		}
	};
};

#endif
