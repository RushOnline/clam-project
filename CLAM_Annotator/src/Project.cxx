#include "Project.hxx"
#include "Text.hxx"
#include "RestrictedString.hxx"
#include "IndexArray.hxx"
#include "XMLStorage.hxx"


namespace CLAM_Annotator
{

void Project::CreatePoolScheme(const CLAM_Annotator::Schema& schema, CLAM::DescriptionScheme& poolScheme)
{
	//First we start with HLD
	std::list<CLAM_Annotator::HLDSchemaElement>& hlds = schema.GetHLDSchema().GetHLDs();
	std::list<CLAM_Annotator::HLDSchemaElement>::iterator it2;
	for(it2 = hlds.begin(); it2 != hlds.end(); it2++)
	{
		if((*it2).GetType()=="Float")
		{
			poolScheme.AddAttribute <float>("Song",(*it2).GetName());
		}
		else if((*it2).GetType()=="Int")
		{
			poolScheme.AddAttribute <int>("Song",(*it2).GetName());
		}
		else if((*it2).GetType()=="RestrictedString")
		{
			poolScheme.AddAttribute <CLAM_Annotator::RestrictedString>("Song",(*it2).GetName());
		}
		else
		{
			poolScheme.AddAttribute <CLAM::Text>("Song",(*it2).GetName());
		}
	}
	//And now we go into LLD
	std::list<std::string>::iterator it;
	std::list<std::string>& descriptorsNames = schema.GetLLDSchema().GetLLDNames();
	for(it = descriptorsNames.begin(); it != descriptorsNames.end(); it++)
	{
		poolScheme.AddAttribute <CLAM::TData>("Frame", (*it));
	}
	//finally we add segmentation marks
	poolScheme.AddAttribute<CLAM::IndexArray>("Song","Segments");

}

bool Project::LoadScheme(const std::string & schemeFileName)
{
	CLAM_Annotator::Schema tempSchema;
	CLAM::XMLStorage::Restore(tempSchema,schemeFileName); // May throw an exception

	// Successfull file, just change it
	SetSchema(schemeFileName);
	GetAnnotatorSchema() = tempSchema;
	GetDescriptionScheme() = CLAM::DescriptionScheme();
	CreatePoolScheme(GetAnnotatorSchema(), GetDescriptionScheme());
	return true;
}

}

