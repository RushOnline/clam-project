#ifndef __NETSPECGRAMRENDERER__
#define __NETSPECGRAMRENDERER__

#include <vector>
#include "Renderer.hxx"

namespace CLAM
{
    namespace VM
    {
	class NetSpecgramRenderer : public Renderer
	{
	public:
	    NetSpecgramRenderer();
	    virtual ~NetSpecgramRenderer();

	    void SetData(std::vector< std::vector<Color> >& data, int index);

	    void Render();

	private:
	    std::vector< std::vector<Color> > _data;
	    int _index;

	};
    }
}

#endif

