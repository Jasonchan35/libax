#ifndef __axMatrix4x4_h__
#define __axMatrix4x4_h__

#include "ax_math_angle.h"
#include "axVec3.h"
#include "axVec4.h"
#include "axEulerRotation3.h"
#include "axQuaternion.h"

//! \addtogroup math
//@{

template<class T> class axMatrix4x4;
typedef axMatrix4x4<float>	axMatrix4x4f;
typedef axMatrix4x4<double>	axMatrix4x4d;

//! 4x4 matrix template, matrix is column-major
template<class T>
class axMatrix4x4 {
public:
	typedef	T	TYPE;
	
	axVec4<T> cx,cy,cz,cw;

	axMatrix4x4() {}
	axMatrix4x4( axVec4<T> cxx, axVec4<T> cyy, axVec4<T> czz, axVec4<T> cww ) :cx(cxx),cy(cyy),cz(czz),cw(cww) {}

	axMatrix4x4 ( T v00, T v01, T v02, T v03,
			  T v10, T v11, T v12, T v13,
			  T v20, T v21, T v22, T v23,
			  T v30, T v31, T v32, T v33 );

	void set ( T v00, T v01, T v02, T v03,
			   T v10, T v11, T v12, T v13,
		 	   T v20, T v21, T v22, T v23,
			   T v30, T v31, T v32, T v33 );

		void		setAll			( T v )						{ cx.setAll(v); cy.setAll(v); cz.setAll(v); cw.setAll(v); }

			  T&	e( axSize col, axSize row )					{ return (&cx)[col][row]; }
		const T&	e( axSize col, axSize row ) const			{ return (&cx)[col][row]; }

			  T*	asPointer()										{ return &cx.x; }
		const T*	asPointer() const								{ return &cx.x; }

			  axVec4<T>&	col( axSize i )							{ return (&cx)[i]; }
		const axVec4<T>&	col( axSize i ) const					{ return (&cx)[i]; }

			  axVec4<T>		row( axSize i ) const					{ return axVec4<T>( cx[i], cy[i], cz[i], cw[i] ); }

			  axVec4<T>&	operator[]( axSize col )				{ return (&cx)[col]; }
		const axVec4<T>&	operator[]( axSize col ) const			{ return (&cx)[col]; }

		void		operator+=		( const axMatrix4x4 &v ) 		{ cx += v.cx; cy += v.cy; cz += v.cz; cw += v.cw; }
		void		operator-=		( const axMatrix4x4 &v ) 		{ cx -= v.cx; cy -= v.cy; cz -= v.cz; cw -= v.cw; }

		void		operator+=		( T v ) 					{ cx += v; cy += v; cz += v; cw += v; }
		void		operator-=		( T v ) 					{ cx -= v; cy -= v; cz -= v; cw -= v; }
		void		operator*=		( T v ) 					{ cx *= v; cy *= v; cz *= v; cw *= v; }
		void		operator*=		( const axMatrix4x4 &v );

		bool		operator==		( const axMatrix4x4 &v ) const	{ return memcmp( &v, this, sizeof(v) ) == 0; }
		bool		operator!=		( const axMatrix4x4 &v ) const	{ return memcmp( &v, this, sizeof(v) ) != 0; }

		axMatrix4x4		operator+		( const axMatrix4x4 &v ) const	{ return axMatrix4x4( cx+v.cx, cy+v.cy, cz+v.cz, cw+v.cw ); }
		axMatrix4x4		operator-		( const axMatrix4x4 &v ) const	{ return axMatrix4x4( cx-v.cx, cy-v.cy, cz-v.cz, cw-v.cw ); }
		axMatrix4x4		operator*		( const axMatrix4x4 &v ) const;

		axMatrix4x4		operator+		( T v ) const				{ return axMatrix4x4<T>( cx+v, cy+v, cz+v, cw+v ); }
		axMatrix4x4		operator-		( T v ) const				{ return axMatrix4x4<T>( cx-v, cy-v, cz-v, cw-v ); }
		axMatrix4x4		operator*		( T v ) const				{ return axMatrix4x4<T>( cx*v, cy*v, cz*v, cw*v ); }
		axMatrix4x4		operator/		( T v ) const				{ T inv = (T)1.0/v; return *this * inv; }

		axMatrix4x4		transpose		() const;
		axMatrix4x4		inverse			() const;

			void		inverseIt		() { *this = inverse(); }

			bool		isAll			( T v ) const						{ return ( cx.isAll(v) && cy.isAll(v) && cz.isAll(v) && cw.isAll(v) ); }
			bool		isAny			( T v ) const						{ return ( cx.isAny(v) || cy.isAny(v) || cz.isAny(v) || cw.isAny(v) ); }

			void		setIdentity		()									{ cx.set(1,0,0,0); cy.set(0,1,0,0); cz.set(0,0,1,0); cw.set(0,0,0,1); }
			
			void		setTRS			( const axVec3<T> & translate, const axVec3<T> & rotate, const axVec3<T> & scale );
			
			void		setTranslate	( const axVec3<T> &v );
			void		setTranslate	( T x, T y, T z ) 					{ setTranslate( axVec3<T>(x,y,z) ); }
			
			void		translate		( const axVec3<T> &v );
			void		translate		( const axVec2<T> &v )				{ translate( axVec3<T>(v.x,v.y,0) ); }
			void		translate		( T x, T y, T z )					{ translate( axVec3<T>(x,y,z) ); }

			void		setRotate		( const axVec3<T> & eulerAngle	  );
			void		setRotateDeg	( const axVec3<T> & eulerAngleDeg )	{ setRotate( ax_deg_to_rad( eulerAngleDeg ) ); }
			
			void		rotate			( const axVec3<T> & eulerAngle    )	{ axMatrix4x4<T> t; t.setRotate(eulerAngle); operator*=(t); }
			void		rotateDeg		( const axVec3<T> & eulerAngleDeg )	{ rotate( ax_deg_to_rad( eulerAngleDeg ) ); }

			void		rotate			( const axEulerRotation3<T> & er );
			void		rotate			( const axQuaternion<T>     & qu );

			void		setRotateX		( T rad );
			void		setRotateY		( T rad );
			void		setRotateZ		( T rad );

			void		rotateX         ( T rad )							{ axMatrix4x4<T> t; t.setRotateX( rad ); operator*=(t); }
			void		rotateY         ( T rad )							{ axMatrix4x4<T> t; t.setRotateY( rad ); operator*=(t); }
			void		rotateZ         ( T rad )							{ axMatrix4x4<T> t; t.setRotateZ( rad ); operator*=(t); }

			void		setRotateDegX	( T deg )							{ return setRotateX( ax_deg_to_rad(deg) ); }
			void		setRotateDegY	( T deg )							{ return setRotateY( ax_deg_to_rad(deg) ); }
			void		setRotateDegZ	( T deg )							{ return setRotateZ( ax_deg_to_rad(deg) ); }
				
			void		rotateDegX		( T deg )							{ return rotateX( ax_deg_to_rad( deg ) ); }
			void		rotateDegY		( T deg )							{ return rotateY( ax_deg_to_rad( deg ) ); }
			void		rotateDegZ		( T deg )							{ return rotateZ( ax_deg_to_rad( deg ) ); }

			//rotate along axis
			void		setRotateAxis	( T rad, const axVec3<T> &axis );
			void		setRotateAxis	( T rad, T x, T y, T z )			{ setRotateAxis( rad, axVec3<T>(x,y,z) ); }
			
			void		setRotateDegAxis( T deg, const axVec3<T> &axis )	{ setRotateAxis( ax_deg_to_rad(deg), axis ); }
			void		setRotateDegAxis( T deg, T x, T y, T z )			{ setRotateAxis( ax_deg_to_rad(deg), x,y,z ); }

			void		rotateAxis		( T rad, const axVec3<T> &axis )	{ axMatrix4x4<T> t; t.setRotateAxis( rad, axis ); operator*=(t); }
			void		rotateDegAxis	( T deg, const axVec3<T> &axis )	{ rotateAxis( ax_deg_to_rad(deg), axis ); }

			void		rotateAxis		( T rad, T x, T y, T z )			{ rotateAxis( rad, 					axVec3<T>(x,y,z) ); }
			void		rotateDegAxis	( T deg, T x, T y, T z )			{ rotateAxis( ax_deg_to_rad(deg), 	axVec3<T>(x,y,z) ); }
			
			void		setScale		( const axVec3<T> &v );
			void		scale			( const axVec3<T> &v )				{ axMatrix4x4<T> t; t.setScale( v ); operator*=(t); }
			void		scale			( const axVec2<T> &v )				{ scale( axVec3<T>(v.x,v.y,1) ); }
			void		scale			( T x, T y, T z )					{ scale( axVec3<T>(x,y,z) ); }

			void		setShear		( const axVec3<T> &v );
			void		shear			( const axVec3<T> &v )				{ axMatrix4x4<T> t; t.setShear( v ); operator*=(t); }

			void		setAimZ_UpY		( const axVec3<T> &eye, const axVec3<T> &aim, const axVec3<T> &up, bool neg_z = false );
			void		setLookAt		( const axVec3<T> &eye, const axVec3<T> &aim, const axVec3<T> &up = axVec3f(0,1,0) );
			void		setOrtho		( T left,T right, T bottom, T top,T zNear,T zFar );

			//! the fov using rad instead deg ( OpenGL )
			void		setPerspective	( T fovy_rad, T aspect, T zNear, T zFar );

	axVec4<T>		position		() const;
	axVec3<T>		dirX			() const; //!< side
	axVec3<T>		dirY			() const; //!< up
	axVec3<T>		dirZ			() const; //!< front

	axVec3<T>		dirX_transpose	() const;
	axVec3<T>		dirY_transpose	() const;
	axVec3<T>		dirZ_transpose	() const;

	const	static 		axMatrix4x4	kIdentity;

	template<class S>	axStatus	serialize_io	( S &s );

						axStatus	toStringFormat	( axStringFormat &f ) const;
						axStatus	onTake			( axMatrix4x4<T> &b ) { *this = b; return 0; }

};

template< class T > inline axMatrix4x4f to_axMatrix4x4f( const axMatrix4x4<T>& v ) { 
	return axMatrix4x4f( to_axVec4f(v.cx), to_axVec4f(v.cy), to_axVec4f(v.cz), to_axVec4f(v.cw) ); 
}

template< class T > inline axMatrix4x4d to_axMatrix4x4d( const axMatrix4x4<T>& v ) { 
	return axMatrix4x4d( to_axVec4d(v.cx), to_axVec4d(v.cy), to_axVec4d(v.cz), to_axVec4d(v.cw) ); 
}


template<class T> inline axVec4<T> axMatrix4x4<T>::position() const    { return cw; }

template<class T> inline axVec3<T> axMatrix4x4<T>::dirX() const       { return axVec3<T>( cx.x, cy.x, cz.x ).normalize(); }
template<class T> inline axVec3<T> axMatrix4x4<T>::dirY() const       { return axVec3<T>( cx.y, cy.y, cz.y ).normalize(); }
template<class T> inline axVec3<T> axMatrix4x4<T>::dirZ() const       { return axVec3<T>( -cx.z, -cy.z, -cz.z ).normalize(); }

template<class T> inline axVec3<T> axMatrix4x4<T>::dirX_transpose() const   { return axVec3<T>( cx.x, cx.y, cx.z ).normalize(); }
template<class T> inline axVec3<T> axMatrix4x4<T>::dirY_transpose() const   { return axVec3<T>( cy.x, cy.y, cy.z ).normalize(); }
template<class T> inline axVec3<T> axMatrix4x4<T>::dirZ_transpose() const   { return axVec3<T>( -cz.x, -cz.y, -cz.z ).normalize(); }



template<class T>
template<class S> inline
axStatus axMatrix4x4<T>::serialize_io( S &s ) {
#if axBYTE_ORDER == axSERIALIZE_BYTE_ORDER
	return s.io_raw( this, sizeof(T)*16 );
#else
	axStatus st;
	st = s.io( cx );	if( !st ) return st;
	st = s.io( cy );	if( !st ) return st;
	st = s.io( cw );	if( !st ) return st;
	st = s.io( cz );	if( !st ) return st;
	return 0;
#endif
}

template< class T > inline axStatus ax_json_serialize_value( axJsonWriter &s, axMatrix4x4<T> &v ) { return ax_json_serialize_value_array( s, v.asPointer(), 16 ); }
template< class T > inline axStatus ax_json_serialize_value( axJsonParser &s, axMatrix4x4<T> &v ) { return ax_json_serialize_value_array( s, v.asPointer(), 16 ); }


//@}

#define axTYPE_LIST(T) \
	template<> class axTypeTrait<T> { \
	public: \
		static const bool isPOD = true; \
	};\
//----
    axTYPE_LIST( axMatrix4x4f )
    axTYPE_LIST( axMatrix4x4d )
#undef axTYPE_LIST


#endif //__axMatrix4x4_h__
    
