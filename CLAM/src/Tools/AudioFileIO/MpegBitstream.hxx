#ifndef __MpegBitstream__
#define __MpegBitstream__

#if USE_MAD != 1
#error USE_MAD was not set to 1 in your settings.cfg file, but you are including files that require this. Please fix your settings.cfg
#endif

#include <mad.h>
#include <cstdio>
#include "DataTypes.hxx"

namespace CLAM
{

namespace AudioCodecs
{
	class MpegBitstream
	{
		struct mad_stream  mBitstream;
		struct mad_frame   mCurrentFrame;
		struct mad_synth   mMpegSynth;
		mad_timer_t        mStreamTimer;
		
		unsigned char*     mInputBuffer;
		static const int   mInputBufferSize;

		int                mStatus;
		bool               mFatalError;
		int                mDecodedFrames;
		FILE*              mpFile;

	public:
		MpegBitstream( FILE* bitstream );
		MpegBitstream();
		~MpegBitstream();

		void   Init();
		void   Init(FILE*);
		TTime  Finish();
		bool   EOS();
		bool   FatalError();
		bool   NextFrame();
		bool   SynthesizeCurrent();
		struct mad_frame& CurrentFrame();
		struct mad_synth&  CurrentSynthesis();
		struct mad_stream& StreamState();
	};

	inline struct mad_stream& MpegBitstream::StreamState()
	{
		return mBitstream;
	}
}
		

}


#endif // MpegBitstream.hxx
