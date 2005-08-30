#ifndef _Project_hxx_
#define _Project_hxx_

#include "DynamicType.hxx"
#include "SongFiles.hxx"

namespace CLAM_Annotator
{

class Project : public CLAM::DynamicType
{
	DYNAMIC_TYPE(Project,3);
	DYN_ATTRIBUTE(0, public, std::string, Songs);
	DYN_ATTRIBUTE(1, public, std::string, Schema);
	DYN_ATTRIBUTE(2, public, SongFiles, SongList);

	void DefaultInit()
	{
		AddAll();
		UpdateData();
	}
};

}

#endif//_Project_hxx_
