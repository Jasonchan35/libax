//
//  axTuple.h
//  ax_core
//
//  Created by Jason on 2012-11-14.
//
//

#ifndef ax_core_axTuple_h
#define ax_core_axTuple_h

template<class A=void, class B=void, class C=void, class D=void> class axTuple;

template<class A, class B, class C, class D>
class axTuple {
public:
	A	a;
	B	b;
	C	c;
	D	d;
};

template<class A, class B, class C>
class axTuple <A,B,C,void> {
public:
	A	a;
	B	b;
	C	c;
};

template<class A, class B>
class axTuple <A,B,void,void> {
public:
	A	a;
	B	b;
};

template<class A>
class axTuple <A,void,void,void> {
public:
	A	a;
};

#endif
