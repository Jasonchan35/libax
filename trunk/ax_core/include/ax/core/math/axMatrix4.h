#ifndef __axMatrix4_h__
#define __axMatrix4_h__

#include "ax_math_angle.h"
#include "axVec3.h"
#include "axVec4.h"
#include "axEulerRotation3.h"
#include "axQuaternion.h"

//! \addtogroup math
//@{

template<class T> class axMatrix4;
typedef axMatrix4<float>	axMatrix4f;
typedef axMatrix4<double>	axMatrix4d;

//! 4x4 matrix template, matrix is column-major
template<class T>
class axMatrix4 {
public:
	typedef	T	TYPE;
	
	axVec4<T> cx,cy,cz,cw;

	axMatrix4() {}
	axMatrix4( axVec4<T> cxx, axVec4<T> cyy, axVec4<T> czz, axVec4<T> cww ) :cx(cxx),cy(cyy),cz(czz),cw(cww) {}

	axMatrix4 ( T v00, T v01, T v02, T v03,
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

	void		operator+=		( const axMatrix4 &v ) 		{ cx += v.cx; cy += v.cy; cz += v.cz; cw += v.cw; }
	void		operator-=		( const axMatrix4 &v ) 		{ cx -= v.cx; cy -= v.cy; cz -= v.cz; cw -= v.cw; }

	void		operator+=		( T v ) 					{ cx += v; cy += v; cz += v; cw += v; }
	void		operator-=		( T v ) 					{ cx -= v; cy -= v; cz -= v; cw -= v; }
	void		operator*=		( T v ) 					{ cx *= v; cy *= v; cz *= v; cw *= v; }
	void		operator*=		( const axMatrix4 &v );

	bool		operator==		( const axMatrix4 &v ) const	{ return memcmp( &v, this, sizeof(v) ) == 0; }
	bool		operator!=		( const axMatrix4 &v ) const	{ return memcmp( &v, this, sizeof(v) ) != 0; }

	axMatrix4		operator+		( const axMatrix4 &v ) const	{ return axMatrix4( cx+v.cx, cy+v.cy, cz+v.cz, cw+v.cw ); }
	axMatrix4		operator-		( const axMatrix4 &v ) const	{ return axMatrix4( cx-v.cx, cy-v.cy, cz-v.cz, cw-v.cw ); }
	axMatrix4		operator*		( const axMatrix4 &v ) const;

	axMatrix4		operator+		( T v ) const				{ return axMatrix4<T>( cx+v, cy+v, cz+v, cw+v ); }
	axMatrix4		operator-		( T v ) const				{ return axMatrix4<T>( cx-v, cy-v, cz-v, cw-v ); }
	axMatrix4		operator*		( T v ) const				{ return axMatrix4<T>( cx*v, cy*v, cz*v, cw*v ); }
	axMatrix4		operator/		( T v ) const				{ T inv = (T)1.0/v; return *this * inv; }

	axMatrix4		transpose		() const;
	axMatrix4<T>	inverse			() const;

		void		inverseIt() { *this = inverse(); }

		bool		isAll				( T v ) const				{ return ( cx.isAll(v) && cy.isAll(v) && cz.isAll(v) && cw.isAll(v) ); }

		void		setIdentity	()									{ cx.set(1,0,0,0); cy.set(0,1,0,0); cz.set(0,0,1,0); cw.set(0,0,0,1); }
		void		setTranslate	( const axVec3<T> &v );
		void		translate		( const axVec3<T> &v );
		void		translate		( const axVec2<T> &v )			{ translate( axVec3<T>(v.x,v.y,0) ); }

		void		setRotate		( T rad, const axVec3<T> &axis );
		void		setRotateX		( T rad );
		void		setRotateY		( T rad );
		void		setRotateZ		( T rad );

		//! the angle using rad instead deg ( OpenGL )
		void		rotate			( T rad, const axVec3<T> &axis )	{ axMatrix4<T> t; t.setRotate( rad, axis ); operator*=(t); }
		void		rotate			( const axEulerRotation3<T>  &er );
		void		rotate			( const axQuaternion<T>     &qu );
		void		rotateX         ( T rad )							{ axMatrix4<T> t; t.setRotateX( rad );operator*=(t); }
		void		rotateY         ( T rad )							{ axMatrix4<T> t; t.setRotateY( rad );operator*=(t); }
		void		rotateZ         ( T rad )							{ axMatrix4<T> t; t.setRotateZ( rad );operator*=(t); }

		void		setScale		( const axVec3<T> &v );
		void		scale			( const axVec3<T> &v )				{ axMatrix4<T> t; t.setScale( v ); operator*=(t); }
		void		scale			( const axVec2<T> &v )				{ scale( axVec3<T>(v.x,v.y,1) ); }

		void		setShear		( const axVec3<T> &v );
		void		shear			( const axVec3<T> &v )				{ axMatrix4<T> t; t.setShear( v ); operator*=(t); }

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

	const	static axMatrix4	kIdentity;

	template<class S>	axStatus	serialize_io	( S &se );
						axStatus	toStringFormat	( axStringFormat &f ) const;
						axStatus	onTake	( axMatrix4<T> &b )				{ *this = b; return 0; }

};

template< class T > inline axMatrix4f to_axMatrix4f( const axMatrix4<T>& v ) { 
	return axMatrix4f( to_axVec4f(v.cx), to_axVec4f(v.cy), to_axVec4f(v.cz), to_axVec4f(v.cw) ); 
}

template< class T > inline axMatrix4d to_axMatrix4d( const axMatrix4<T>& v ) { 
	return axMatrix4d( to_axVec4d(v.cx), to_axVec4d(v.cy), to_axVec4d(v.cz), to_axVec4d(v.cw) ); 
}


template<class T>
template<class S> inline
axStatus axMatrix4<T>::serialize_io( S &s ) {
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


//@}

#define axTYPE_LIST(T) \
	template<> inline bool axTypeOf<T>::isPOD() { return true; }
//----
    axTYPE_LIST( axMatrix4f )
    axTYPE_LIST( axMatrix4d )
#undef axTYPE_LIST


#endif //__axMatrix4_h__
    
