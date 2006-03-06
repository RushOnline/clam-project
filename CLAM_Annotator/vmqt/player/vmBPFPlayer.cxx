#include "AudioIO.hxx"
#include "AudioManager.hxx"
#include "AudioOut.hxx"
#include "SimpleOscillator.hxx"
#include "vmBPFPlayer.hxx"

namespace CLAM
{
	namespace VM
	{
		const double min_ref = 8.1758;  // 8.1758 Hz 
		const double max_ref = 12545.9; // 12545.9 Hz 

		BPFPlayer::BPFPlayer(QObject* parent)
			: Player(parent)
			, mBPF(0)
			, mAudio0(0)
			, mAudio1(0)
			, mPitchBounds(min_ref,max_ref)
		{
			SetPlayingFlags(CLAM::VM::eUseOscillator);
		}

		BPFPlayer::~BPFPlayer()
		{
		}

		void BPFPlayer::SetData(const BPF& bpf)
		{
			mBPF = &bpf;
		}

		void BPFPlayer::SetAudioPtr(const Audio* audio, unsigned channelMask)
		{
			if (channelMask&1)
				mAudio0 = audio;
			if (channelMask&2)
				mAudio1 = audio;
		}

		void BPFPlayer::SetPitchBounds(double min, double max)
		{
			if(min >= max) return;
			mPitchBounds.min = min;
			mPitchBounds.max = max;
		}

		void BPFPlayer::run()
		{
			if(!mBPF) return;
			if(!mBPF->Size()) return;
			if(!mAudio0) SetPlayingFlags(CLAM::VM::eUseOscillator);
			if(mAudio0) mSamplingRate = mAudio0->GetSampleRate();
			else mSamplingRate=44100;

			unsigned frameSize = 1024;     
			AudioManager manager(mSamplingRate,(int)frameSize);  

			manager.Start();
			
			AudioOut channel0;   
			AudioIOConfig audioOutCfg0;     
			audioOutCfg0.SetChannelID(0);    
			channel0.Configure(audioOutCfg0); 

			AudioOut channel1;   
			AudioIOConfig audioOutCfg1;     
			audioOutCfg1.SetChannelID(1);    
			channel1.Configure(audioOutCfg1); 
			
			channel0.Start();
			channel1.Start();

			SimpleOscillatorConfig oscCfg;
			oscCfg.SetSamplingRate(mSamplingRate);
			oscCfg.SetAmplitude(0.6);
			SimpleOscillator osc(oscCfg);
			
			InControl& freqControl = osc.GetInControls().Get("Pitch");
    
			Audio samplesAudio0;
			Audio samplesAudio1;
			Audio samplesBpf;
			samplesAudio0.SetSize(frameSize);
			samplesAudio1.SetSize(frameSize);
			samplesBpf.SetSize(frameSize);

			Audio silence;
			silence.SetSize(frameSize);

		    unsigned firstIndex = FirstIndex();
			unsigned k = firstIndex;

			unsigned start = unsigned(mBeginTime*mSamplingRate);
			unsigned nSamples = unsigned(mTimeBounds.max*mSamplingRate);
	    
			unsigned leftIndex = start;        
			unsigned rightIndex = leftIndex+frameSize;

			osc.Start();

			for(unsigned i=start; i < nSamples; i+=frameSize)
			{
				if(!mBPF->Size()) mIsPlaying = false;
				if(!mIsPlaying) break;
				if(k < (unsigned)mBPF->Size()-1) if(double(i)/mSamplingRate >= double(mBPF->GetXValue(k+1))) k++;

				if(mAudio0 && rightIndex < unsigned(mAudio0->GetSize()))
					mAudio0->GetAudioChunk(int(leftIndex),int(rightIndex),samplesAudio0);
				if(mAudio1 && rightIndex < unsigned(mAudio1->GetSize()))
					mAudio1->GetAudioChunk(int(leftIndex),int(rightIndex),samplesAudio1);

				if(k < (unsigned)mBPF->Size()) freqControl.DoControl(GetPitch(k));
				osc.Do(samplesBpf);
				if (leftIndex/mSamplingRate < mBPF->GetXValue(k))
				{
					channel0.Do(silence);
					channel1.Do(silence);
				}
				else if ((mPlayingFlags & CLAM::VM::eAudio) && (mPlayingFlags & CLAM::VM::eUseOscillator))
				{
					channel0.Do(samplesAudio0);
					channel1.Do(samplesBpf);
				}
				else if (mPlayingFlags & CLAM::VM::eAudio && !(mPlayingFlags & CLAM::VM::eUseOscillator))
				{
					channel0.Do(samplesAudio0);
					channel1.Do(samplesAudio1);
				}
				else
				{
					channel0.Do(samplesBpf);
					channel1.Do(samplesBpf);
				}
				emit playingTime(double(leftIndex)/mSamplingRate);
				leftIndex += frameSize;
				rightIndex += frameSize;				
			}
			osc.Stop();
			channel0.Stop();
			channel1.Stop();

			(mIsPaused) ? mBeginTime = double(leftIndex)/mSamplingRate : mBeginTime = mTimeBounds.min;
			emit stopTime(double(leftIndex)/mSamplingRate,mIsPaused);
			mIsPlaying = false;
		}

		unsigned BPFPlayer::FirstIndex()
		{
			unsigned nPoints = mBPF->Size();
			if(nPoints <= 1) return 0;

			if(mBeginTime <= mBPF->GetXValue(0)) return 0;
			if(mBeginTime >= mBPF->GetXValue(nPoints-1)) return nPoints-1;

			unsigned index = 0;
			unsigned left_index = 0;
			unsigned right_index = nPoints-1;
			while(left_index <= right_index)
			{
				const unsigned currentIndex = (left_index+right_index)/2;
				if(currentIndex >= nPoints-1)
				{
					index = currentIndex;
					break;
				}
				if(mBeginTime >= double(mBPF->GetXValue(currentIndex)) && 
				   mBeginTime <= double(mBPF->GetXValue(currentIndex+1)))
				{
					index = currentIndex;
					break;
				}
				if(mBeginTime < double(mBPF->GetXValue(currentIndex)))
				{
					right_index = currentIndex-1;
				}
				else if(mBeginTime > double(mBPF->GetXValue(currentIndex)))
				{
					left_index = currentIndex+1;
				}
			}
			return index;
		}

		double BPFPlayer::GetPitch(unsigned index)
		{
			double value = mBPF->GetValueFromIndex(index);
//			if(value >= min_ref && value <= max_ref) return value;
			if(value < mPitchBounds.min) value = mPitchBounds.min;
			else if(value > mPitchBounds.max) value = mPitchBounds.max;
			return (value-mPitchBounds.min)*(max_ref-min_ref)/mPitchBounds.Span()+min_ref;
		}
	}
}

// END
