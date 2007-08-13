#include "ComplexSpectrumProduct.hxx"
#include <CLAM/ProcessingFactory.hxx>
namespace CLAM
{
namespace Hidden
{
	static const char* metadata[] = {
		"key", "ComplexSpectrumProduct",
		"category", "CLAM",
		"description", "ComplexSpectrumProduct",
		0
	};
	static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, ComplexSpectrumProduct>
	reg = metadata;
}

}
