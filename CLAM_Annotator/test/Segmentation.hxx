#ifndef Segmentation_hxx
#define Segmentation_hxx

#include <vector>
#include <algorithm>
#include <iterator>
#include <cmath>

namespace CLAM_Annotator
{
	class Segmentation
	{
	public:
		class InsertedOutOfBounds : public std::exception
		{
			public:
			const char * what() const throw () { return "Segmentation point inserted out of limits";}
		};
		typedef std::vector<double> Bounds;
	public:
		Segmentation(double maxLength)
			: _maxLength(maxLength)
		{
			_selection.push_back(false);
			_endBounds.push_back(maxLength);

		}
		/**
		 * Inserts a new bound at timePosition.
		 */
		unsigned insert(double timePosition)
		{
			if (timePosition<=0.0) throw InsertedOutOfBounds();
			Bounds::iterator insertPoint = 
				std::lower_bound(_endBounds.begin(), _endBounds.end(), timePosition);
			if (insertPoint == _endBounds.end()) throw InsertedOutOfBounds();
			// 'position' must be computed before the insertion to not invalidate iterators.
			int position = insertPoint - _endBounds.begin() +1;
			_endBounds.insert(insertPoint, timePosition);
			_selection.insert(_selection.begin()+position, false);
			return position;
		}
		/**
		 * Returns the index of the segment which is nearest to the
		 * given time position, and within the tolerance.
		 * If no end of segment within the tolerance range an invalid
		 * segment is returned (> nSegments)
		 */
		unsigned pickEndBound(double timePosition, double tolerance) const
		{
			Bounds::const_iterator lowerBound = 
				std::lower_bound(_endBounds.begin(), _endBounds.end(), timePosition-tolerance);
			Bounds::const_iterator upperBound = 
				std::upper_bound(lowerBound, _endBounds.end(), timePosition+tolerance);

			if (lowerBound==upperBound) return _endBounds.size(); // None found
	
			// Pick the closest in range
			unsigned lowerSegment = lowerBound - _endBounds.begin();
			unsigned upperSegment = upperBound - _endBounds.begin();
			double lastDifference = std::fabs(timePosition-_endBounds[lowerSegment]);
			for (unsigned i=lowerSegment; i<upperSegment; i++)
			{
				double newDifference = std::fabs(timePosition-_endBounds[i]);
				if (newDifference>lastDifference) break;
				lastDifference = newDifference;
				lowerSegment = i;
			}
			return lowerSegment;
		}
		/**
		 * Returns the index of the segment which body is on timePosition.
		 */
		unsigned pickSegmentBody(double timePosition) const
		{
			if (timePosition<0) return _endBounds.size();
			Bounds::const_iterator lowerBound =
				std::lower_bound(_endBounds.begin(), _endBounds.end(), timePosition);
			return lowerBound - _endBounds.begin();
		}
		void dragEndBound(unsigned segment, double newTimePosition)
		{
			if (segment==_endBounds.size()) return;
			if (newTimePosition>_endBounds[segment+1])
				newTimePosition = _endBounds[segment+1];
			double leftBound;
			if (segment==0) leftBound = 0;
			else leftBound = _endBounds[segment-1];
			if (newTimePosition<leftBound)
				newTimePosition = leftBound;
			_endBounds[segment]=newTimePosition;
		}
		void removeEndBound(unsigned segment)
		{
			if (segment==_endBounds.size()-1) return;
			_endBounds.erase(_endBounds.begin()+segment);
			_selection.erase(_selection.begin()+segment);
		}
		void select(unsigned segment)
		{
			_selection[segment]=true;
		}
		void clearSelection()
		{
			for (unsigned i=0; i<_selection.size(); i++)
				_selection[i]=false;
		}
		std::string boundsAsString() const
		{
			std::ostringstream os;
			double lastOffset = 0;
			for (unsigned i=0; i<_endBounds.size(); i++)
			{
				if (_selection[i]) os << "+";
				os << "(" << lastOffset << "," << _endBounds[i] << ") ";
				lastOffset = _endBounds[i];
			}
			return os.str();
		}
	private:
		Bounds _endBounds;
		double _maxLength;
		std::vector<bool> _selection;
	};

}

// Init single Segment
// Split segment
// Join partitions
// Drag Point
// Current




#endif//Segmentation_hxx
