#ifndef ChordCorrelator_hxx
#define ChordCorrelator_hxx
#include <list>
#include <vector>
#include <cmath>
#include <sstream>

namespace Simac
{

class ChordCorrelator
{
public:
	static std::string PitchNames(unsigned index)
	{
		static const char * PitchNames[] = {"G","G#","A","A#","B","C","C#","D","D#","E","F","F#"};
		return PitchNames[index];
	}
	enum Pitch {G,Ab,A,Bb,B,C,Db,D,Eb,E,F,Gb};
	typedef unsigned ChordMode;
	typedef std::vector<double> PCP;
	typedef std::vector<double> ChordCorrelation;

	struct ChordModePatterns
	{
		double pattern[12];
		const char * name;
		unsigned nRoots;
	};
	
	static ChordModePatterns * chordModePatterns()
	{
		static ChordModePatterns patterns[] =
		{
			{{1,1,1,1,1,1,1,1,1,1,1,1}, "None",        1},
			{{1,0,0,0,1,0,0,1,0,0,0,0}, "Major",      12},
			{{1,0,0,1,0,0,0,1,0,0,0,0}, "Minor",      12},
			{{1,0,0,0,1,0,0,1,0,0,0,1}, "Major7",     12}, // Cmaj7
			{{1,0,0,0,1,0,0,1,0,0,1,0}, "Dominant7",  12}, // C7
			{{1,0,0,1,0,0,0,1,0,0,0,1}, "MinorMajor7",12}, // Cm/maj7
			{{1,0,0,1,0,0,0,1,0,0,1,0}, "Minor7",     12}, // Cm7
//			{{1,0,1,0,0,0,0,1,0,0,0,0}, "Suspended2", 12}, // Csus2
//			{{1,0,0,0,0,1,0,1,0,0,0,0}, "Suspended4", 12}, // Csus4
//			{{1,0,0,0,1,0,0,1,0,1,0,0}, "Major6",     12}, // C6
//			{{1,0,0,1,0,0,0,1,0,1,0,0}, "Minor6",     12}, // Cm6
//			{{1,0,1,0,1,0,0,1,0,1,0,0}, "6/9",        12}, // C6/9
//			{{1,0,0,0,1,0,0,0,1,0,0,0}, "Augmented",   4}, // Caug
//			{{1,0,0,1,0,0,1,0,0,1,0,0}, "Diminished",  3}, // Cdim
//			{{1,0,0,1,0,0,1,0,0,0,1,0}, "Diminished7",12}, // Cdim7
			{{0,0,0,0,0,0,0,0,0,0,0,0}, 0, 0}
		};
		return patterns;
	}

	struct ChordPattern {
		double pattern[12];
		Pitch root;
		ChordMode mode;
		double normalization;
		
	};
public:
	ChordCorrelator(ChordModePatterns * modePatterns = chordModePatterns())
	{
		for (unsigned mode=0; modePatterns[mode].name; mode++)
		{
			_chordModeNames.push_back(modePatterns[mode].name);
			double * basePattern = modePatterns[mode].pattern;
			double normalizationFactor = 0.0;
			for (unsigned i = 0; i<12; i++) 
				normalizationFactor += basePattern[i];

			for (unsigned root=0; root<modePatterns[mode].nRoots; root++)
			{
				ChordPattern pattern;
				pattern.root = Pitch(root);
				pattern.mode = mode;
				pattern.normalization = normalizationFactor;
				for (unsigned i=root; i<12; i++)
					pattern.pattern[i] = basePattern[i-root];
				for (unsigned i=0; i<root; i++)
					pattern.pattern[i] = basePattern[12+i-root];
				_chordPatterns.push_back(pattern);
			}
		}
		_output.resize(_chordPatterns.size());
	}
	void DumpChordPatterns(std::ostream & os)
	{
		for (unsigned pattern=0; pattern<_chordPatterns.size(); pattern++)
		{
			for (unsigned i=0; i<12; i++)
				os << _chordPatterns[pattern].pattern[i] << " ";
			os << _chordPatterns[pattern].normalization << " ";
			os << PitchNames(_chordPatterns[pattern].root) << " ";
			os << _chordModeNames[_chordPatterns[pattern].mode] << "\n";
		}
	}
	std::string root(unsigned chordIndex) const
	{
		return PitchNames(_chordPatterns[chordIndex].root);
	}
	std::string mode(unsigned chordIndex) const
	{
		return _chordModeNames[_chordPatterns[chordIndex].mode];
	}
	std::string chordRepresentation(unsigned chordIndex) const
	{
		std::ostringstream os;
		os << PitchNames(_chordPatterns[chordIndex].root) << "_";
		os << _chordModeNames[_chordPatterns[chordIndex].mode];
		return os.str();
	}
	~ChordCorrelator()
	{
	}
	void doIt(const PCP & pcp)
	{
		for (unsigned int i=0; i<_chordPatterns.size(); i++)
		{
			_output[i]=0.0;
			double * chordPattern = _chordPatterns[i].pattern;
			for (unsigned pitch=0; pitch<12; pitch++)
			{
				_output[i]+= chordPattern[pitch] * pcp[pitch];
			}
			_output[i]/=_chordPatterns[i].normalization;
		}
	}
	const std::vector<double> & output() const
	{
		return _output;
	}
private:
	std::vector<ChordPattern> _chordPatterns;
	std::vector<std::string> _chordModeNames;
	ChordCorrelation _output;
};

} // namespace Simac

#endif// ChordCorrelator_hxx

