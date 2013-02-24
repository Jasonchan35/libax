//
//  axClampedValue.h
//  ax_core
//
//  Created by Jason on 2013-02-24.
//
//

#ifndef ax_core_axClampedValue_h
#define ax_core_axClampedValue_h

template<class T>
class axClampedValue { //copyable 
public:
	axClampedValue( T value=0, T lower=0, T upper=0 ) : value_(value), lower_(lower), upper_(upper) {}

	void	setValue	( T v )		{ value_ = ax_clamp( v, lower_, upper_ ); }
	void	setLower	( T v )		{ lower_ = v; setValue(value_); }
	void	setUpper	( T v )		{ upper_ = v; setValue(value_); }

	operator	T		() const	{ return value_; }

	T		value		() const	{ return value_; }
	T		lower		() const	{ return lower_; }
	T		upper		() const	{ return upper_; }
	
	void	operator=	( T v ) 	{ setValue(v); }
	void	operator+=	( T v )		{ setValue( value_ + v ); }
	void	operator-=	( T v )		{ setValue( value_ - v ); }
	void	operator*=	( T v )		{ setValue( value_ * v ); }
	void	operator/=	( T v )		{ setValue( value_ / v ); }

	template<class S>
	axStatus	serialize_io( S &s ) {
		axStatus st;
		ax_io_named( value_, "value" );
		ax_io_named( lower_, "lower" );
		ax_io_named( upper_, "upper" );
		return 0;
	}

	axStatus	toStringFormat( axStringFormat & f ) const {
		return f.format("({?} in [{?}:{?}])", value_, lower_, upper_ );
	}

private:
	T	value_;
	T	lower_;
	T	upper_;
};

typedef	axClampedValue<int>			axClampedInt;
typedef	axClampedValue<float>		axClampedFloat;
typedef	axClampedValue<double>		axClampedDouble;


#endif
