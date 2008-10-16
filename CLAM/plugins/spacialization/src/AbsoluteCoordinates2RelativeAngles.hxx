#ifndef AbsoluteCoordinates2RelativeAngles_hxx 
#define AbsoluteCoordinates2RelativeAngles_hxx

#include <CLAM/Processing.hxx>
#include <CLAM/InControl.hxx>
#include <CLAM/OutControl.hxx>
#include <cmath>
#define PI 3.14159265

namespace CLAM
{

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
 @param[out] "target azimuth" [Control] Relative azimuth of the target
 @param[out] "target elevation" [Control] Relative elevation of the target
 @param[out] "target roll" [Control] Relative roll of the target
 @ingroup SpatialAudio
 @see AmbisonicsConventions
*/
class AbsoluteCoordinates2RelativeAngles : public CLAM::Processing
{ 

	CLAM::InControl _targetX;
	CLAM::InControl _targetY;
	CLAM::InControl _targetZ;
	CLAM::InControl _targetRoll;
	CLAM::InControl _targetElevation;
	CLAM::InControl _targetAzimuth;

	CLAM::InControl _sourceX;
	CLAM::InControl _sourceY;
	CLAM::InControl _sourceZ;
	CLAM::OutControl _sourceRoll;
	CLAM::OutControl _sourceElevation;
	CLAM::OutControl _sourceAzimuth;
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
		, _sourceRoll("source roll", this)
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
//		double cosRoll = std::cos(targetRoll);
//		double sinRoll = std::sin(targetRoll);

		double rotatedX = + cosAzimuth*sinZenith * dx + sinAzimuth * sinZenith * dy+ cosZenith * dz;
		double rotatedY = - sinAzimuth * dx + cosAzimuth * dy - 0 * dz;
		double rotatedZ = - cosZenith * cosAzimuth *dx + -cosZenith*sinAzimuth*dy 	+ sinZenith  * dz;

		// TODO: Test that with target elevation and azimuth
		double dazimuth = 180./M_PI*std::atan2(rotatedY,rotatedX);
		double delevation = 180./M_PI*std::asin(rotatedZ/std::sqrt(rotatedX*rotatedX+rotatedY*rotatedY+rotatedZ*rotatedZ));

		//TODO calculate the roll relative between the listener and the source
		_sourceRoll.SendControl( targetRoll );
		_sourceElevation.SendControl( delevation );
		_sourceAzimuth.SendControl( dazimuth );
		return true;
	}

};
} // namespace
#endif // AbsoluteCoordinates2RelativeAngles_hxx