#include <ax/core/math/axFrustum3.h>

template<class T>
void axFrustum3<T>::_ctor() {
	valid = false;
	plane_dirty = true;
}

template<class T>
axFrustum3<T>::axFrustum3() {
	_ctor();
}

template<class T>
axFrustum3<T>::axFrustum3( const axMatrix4<T>& projection ) {
	_ctor();
	set( projection );
}

template<class T>
const axPlane3<T>*	axFrustum3<T>::planes() const {
    axFrustum3<T> *c = (axFrustum3<T>*)this; // cahnge "const" this to not "const" this
	if( c->plane_dirty ) {
		c->_plane[0].n = ax_tri_normal( pt[0], pt[2], pt[1] );	//near
		c->_plane[0].p = pt[0];

		c->_plane[1].n = ax_tri_normal( pt[4], pt[5], pt[6] );	//far
		c->_plane[1].p = pt[4];

		c->_plane[2].n = ax_tri_normal( pt[2], pt[3], pt[6] );	//top
		c->_plane[2].p = pt[2];

		c->_plane[3].n = ax_tri_normal( pt[0], pt[1], pt[4] );	//bottom
		c->_plane[3].p = pt[0];

		c->_plane[4].n = ax_tri_normal( pt[0], pt[4], pt[3] );	//left
		c->_plane[4].p = pt[0];

		c->_plane[5].n = ax_tri_normal( pt[1], pt[2], pt[5] );	//right
		c->_plane[5].p = pt[1];

		c->plane_dirty = false;
	}
	return _plane;
}

template<class T>
void axFrustum3<T>::set( const axMatrix4<T>&	projection ) {
	axMatrix4<T>	im = projection.inverse();
	//near
	pt[0] = ( axVec4<T>( -1,-1, 0, 1 ) * im ).to_Vec3();
	pt[1] = ( axVec4<T>(  1,-1, 0, 1 ) * im ).to_Vec3();
	pt[2] = ( axVec4<T>(  1, 1, 0, 1 ) * im ).to_Vec3();
	pt[3] = ( axVec4<T>( -1, 1, 0, 1 ) * im ).to_Vec3();

	//far
	pt[4] = ( axVec4<T>( -1,-1, 1, 1 ) * im ).to_Vec3();
	pt[5] = ( axVec4<T>(  1,-1, 1, 1 ) * im ).to_Vec3();
	pt[6] = ( axVec4<T>(  1, 1, 1, 1 ) * im ).to_Vec3();
	pt[7] = ( axVec4<T>( -1, 1, 1, 1 ) * im ).to_Vec3();

	valid = true;
	plane_dirty = true;
}

template<class T>
axFrustum3<T> axFrustum3<T>::operator*( const axMatrix4<T>& m ) {
	axFrustum3<T>	fr;
	if( !valid ) return fr;

	fr.pt[0] = pt[0].mul4x4(m);
	fr.pt[1] = pt[1].mul4x4(m);
	fr.pt[2] = pt[2].mul4x4(m);
	fr.pt[3] = pt[3].mul4x4(m);
	fr.pt[4] = pt[4].mul4x4(m);
	fr.pt[5] = pt[5].mul4x4(m);
	fr.pt[6] = pt[6].mul4x4(m);
	fr.pt[7] = pt[7].mul4x4(m);
	fr.valid = true;

	return fr;
}


template<class T>
void axFrustum3<T>::operator*=( const axMatrix4<T>& m ) {
	if( !valid ) return;
	pt[0].mul4x4_it(m);
	pt[1].mul4x4_it(m);
	pt[2].mul4x4_it(m);
	pt[3].mul4x4_it(m);
	pt[4].mul4x4_it(m);
	pt[5].mul4x4_it(m);
	pt[6].mul4x4_it(m);
	pt[7].mul4x4_it(m);
}


//---- The explicit instantiation ----
template class axFrustum3<float>;
template class axFrustum3<double>;

