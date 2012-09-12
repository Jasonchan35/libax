#ifndef __ax_math_angle_h__
#define __ax_math_angle_h__

#include "../common/ax_common.h"

inline float  ax_rad_to_deg( int    x )	{ return x * (180.0f / (float)ax_math_PI); }
inline float  ax_rad_to_deg( float  x )	{ return x * (180.0f / (float)ax_math_PI); }
inline double ax_rad_to_deg( double x )	{ return x * (180.0  / ax_math_PI); }

inline float  ax_deg_to_rad( int    x )	{ return x * ((float) ax_math_PI / 180.0f); }
inline float  ax_deg_to_rad( float  x )	{ return x * ((float) ax_math_PI / 180.0f); }
inline double ax_deg_to_rad( double x )	{ return x * (ax_math_PI / 180.0);  }

float  ax_angle_normalize_180( float  angle );
double ax_angle_normalize_180( double angle );
float  ax_angle_normalize_360( float  angle );
double ax_angle_normalize_360( double angle );

float  ax_angle_delta( float  angle1, float  angle2 );
double ax_angle_delta( double angle1, double angle2 );



//------------ inline --------------

inline float ax_angle_normalize_360( float angle ) {
	if ( ( angle >= 360.0f ) || ( angle < 0.0f ) ) {
		angle -= ax_floor( angle / 360.0f ) * 360.0f;
	}
	return angle;
}

inline double ax_angle_normalize_360( double angle ) {
	if ( ( angle >= 360.0 ) || ( angle < 0.0 ) ) {
		angle -= ax_floor( angle / 360.0 ) * 360.0;
	}
	return angle;
}

inline float ax_angle_normalize_180( float angle ) {
	angle = ax_angle_normalize_360( angle );
	if ( angle > 180.0f ) angle -= 360.0f;
	return angle;
}

inline double ax_angle_normalize_180( double angle ) {
	angle = ax_angle_normalize_360( angle );
	if ( angle > 180.0 ) angle -= 360.0;
	return angle;
}

inline float  ax_angle_delta( float  angle1, float   angle2 ) { return ax_angle_normalize_180( angle1 - angle2 ); }
inline double ax_angle_delta( double angle1, double  angle2 ) { return ax_angle_normalize_180( angle1 - angle2 ); }



#endif //__ax_math_angle_h__
