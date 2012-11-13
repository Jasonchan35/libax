//
//  axTuple.h
//  ax_core
//
//  Created by Jason on 2012-11-14.
//
//

#ifndef ax_core_axTuple_h
#define ax_core_axTuple_h

template<class T0, class T1, class T2, class T3>
class axTuple <T0,T1,T2,T3> {
public:
	T0	v0;
	T1	v1;
	T2	v2;
	T3	v3;
};

template<class T0, class T1, class T2>
class axTuple <T0,T1,T2,void> {
public:
	T0	v0;
	T1	v1;
	T2	v2;
};

template<class T0, class T1>
class axTuple <T0,T1,void,void> {
public:
	T0	v0;
	T1	v1;
};



#endif
