#ifndef AbsoluteCoordinates2RelativeAngles_hxx 
#define AbsoluteCoordinates2RelativeAngles_hxx

#include <CLAM/Processing.hxx>
#include <CLAM/InControl.hxx>
#include <CLAM/OutControl.hxx>
#include <cmath>
#include <iomanip>
#define PI 3.14159265

namespace CLAM
{

/**
 Represents spherical orientation of a point
 from a given reference frame expressed as the
 azimuth and the elevation angles in degrees.

 @see AmbisonicsConventions
*/
struct Orientation
{
	double azimuth;
	double elevation;

	Orientation(double anAzimuth=0.0, double anElevation=0.0)
		: azimuth(anAzimuth)
		, elevation(anElevation)
	{}
	/**
	 Converts the orientation values for azimuth and elevation to be
	 within the ranges according AmbisonicsConventions.
	*/
	void normalize()
	{
		elevation += 90;
		elevation = _module(elevation, 360);
		if (elevation>180)
		{
			azimuth += 180;
			elevation = 360 - elevation;
		}
		azimuth = _module(azimuth, 360.);
		elevation -= 90;
	}
	bool operator!=(const Orientation & other) const
	{
		if (fabs(azimuth-other.azimuth)>1e-20) return true;
		if (fabs(elevation-other.elevation)>1e-20) return true;
		return false;
	}
	operator std::string() const
	{
		std::ostringstream os;
		os << *this;
		return os.str();
	}
	friend std::ostream & operator<<(std::ostream & os, const Orientation & orientation)
	{
		return os
			<< std::setprecision(4)
			<< orientation.azimuth
			<< " "
			<< orientation.elevation
			;
	}

private:
	double _module(double input, double factor)
	{
		while (input<0) input+=factor;
		return fmod(input, factor);
	}
};

/**
 Computes the azimuth and elevation angles of
 a point (source) relative to the listener (target)
 position and orientation.
 Angles are expressed in degrees. 
 Coordinates systems and rotations follow @ref AmbisonicsConventions.
 @param[in] "target X" [Control] X coord of the listener in absolute coords
 @param[in] "target Y" [Control] Y coord of the listener in absolute coords
 @param[in] "target Z" [Control] Z coord of the listener in absolute coords
 @param[in] "target azimuth" [Control] Azimuth of the listener orientation
 @param[in] "target elevation" [Control] Elevation of the listener orientation
 @param[in] "target roll" [Control] Roll of the listener orientation
 @param[in] "source X" [Control] X coord of the source in absolute coords
 @param[in] "source Y" [Control] Y coord of the source in absolute coords
 @param[in] "source Z" [Control] Z coord of the source in absolute coords
 @param[out] "source azimuth" [Control] Relative azimuth of the source on target's frame
 @param[out] "source elevation" [Control] Relative elevation of the source on target's frame
 @todo Input roll is not considered at all
 @ingroup SpatialAudio
 @see AmbisonicsConventions
*/
class AbsoluteCoordinates2RelativeAngles : public CLAM::Processing
{ 

	CLAM::FloatInControl _targetX;
	CLAM::FloatInControl _targetY;
	CLAM::FloatInControl _targetZ;
	CLAM::FloatInControl _targetRoll;
	CLAM::FloatInControl _targetElevation;
	CLAM::FloatInControl _targetAzimuth;

	CLAM::FloatInControl _sourceX;
	CLAM::FloatInControl _sourceY;
	CLAM::FloatInControl _sourceZ;
	CLAM::FloatOutControl _sourceElevation;
	CLAM::FloatOutControl _sourceAzimuth;
public:
	const char* GetClassName() const { return "AbsoluteCoordinates2RelativeAngles"; }
	AbsoluteCoordinates2RelativeAngles(const Config& config = Config()) 
		: _targetX("target X", this)
		, _targetY("target Y", this)
		, _targetZ("target Z", this)		
		, _targetRoll("target roll", this)
		, _targetElevation("target elevation", this)
		, _targetAzimuth("target azimuth", this)
		, _sourceX("source X", this)
		, _sourceY("source Y", this)
		, _sourceZ("source Z", this)		
		, _sourceElevation("source elevation", this)
		, _sourceAzimuth("source azimuth", this)
	{
		Configure( config );
	}
 
	bool Do()
	{	

		float sourceX = _sourceX.GetLastValue();
		float sourceY = _sourceY.GetLastValue();
		float sourceZ = _sourceZ.GetLastValue();
		float targetX = _targetX.GetLastValue();
		float targetY = _targetY.GetLastValue();
		float targetZ = _targetZ.GetLastValue();
		double targetRoll = _targetRoll.GetLastValue();
		//We sum pi/2 because the matrix is for zenith and no elevation. TODO change zenith for elevation
		double targetZenith = _targetElevation.GetLastValue()+PI/2; 
		double targetAzimuth = _targetAzimuth.GetLastValue();
		double dx = (sourceX - targetX);
		double dy = (sourceY - targetY);
		double dz = (sourceZ - targetZ);
		double cosAzimuth = std::cos(targetAzimuth);
		double sinAzimuth = std::sin(targetAzimuth);
		double cosZenith = std::cos(targetZenith);
		double sinZenith = std::sin(targetZenith);

		double rotatedX = 
			+ dx * cosAzimuth * sinZenith
			+ dy * sinAzimuth * sinZenith 
			+ dz * cosZenith;
		double rotatedY = 
			- dx * sinAzimuth
			+ dy * cosAzimuth
			- dz * 0;
		double rotatedZ =
			- dx * cosZenith * cosAzimuth
			- dy * cosZenith * sinAzimuth
			+ dz * sinZenith;

		// TODO: Test that with target elevation and azimuth
		double dazimuth = 180./M_PI*std::atan2(rotatedY,rotatedX);
		double delevation = 180./M_PI*std::asin(rotatedZ/std::sqrt(rotatedX*rotatedX+rotatedY*rotatedY+rotatedZ*rotatedZ));

		//TODO calculate the roll relative between the listener and the source
		Orientation orientation(dazimuth, delevation);
		orientation.normalize();
		_sourceAzimuth.SendControl( orientation.azimuth );
		_sourceElevation.SendControl( orientation.elevation );
		return true;
	}

};
} // namespace
#endif // AbsoluteCoordinates2RelativeAngles_hxx
