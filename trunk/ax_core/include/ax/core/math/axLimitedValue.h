//
//  axLimitedValue.h
//  ax_core
//
//  Created by Jason on 2013-02-24.
//
//

#ifndef ax_core_axLimitedValue_h
#define ax_core_axLimitedValue_h

template<class T>
class axLimitedValue { //copyable 
public:
	axLimitedValue( T value=0, T max=0 ) : value_(value), max_(max) {}

	void	setValue	( T v )		{ v = ax_min( v, max_ ); }
	void	setMax		( T v )		{ max_ = v; setValue(value_); }

	operator	T		() const	{ return value_; }

	T		value		() const	{ return value_; }
	T		max			() const	{ return max_; }
	
	void	operator=	( T v ) 	{ setValue(v); }
	void	operator+=	( T v )		{ setValue( value_ + v ); }
	void	operator-=	( T v )		{ setValue( value_ - v ); }
	void	operator*=	( T v )		{ setValue( value_ * v ); }
	void	operator/=	( T v )		{ setValue( value_ / v ); }

	template<class S>
	axStatus	serialize_io( S &s ) {
		axStatus st;
		ax_io_named( value_, "value" );
		ax_io_named( max_,   "max"   );
		return 0;
	}

	axStatus	toStringFormat( axStringFormat & f ) const {
		return f.format("({?} / {?})", value_, max_ );
	}

private:
	T	value_;
	T	max_;
};

typedef	axLimitedValue<int>			axLimitedInt;
typedef	axLimitedValue<float>		axLimitedFloat;
typedef	axLimitedValue<double>		axLimitedDouble;


#endif
