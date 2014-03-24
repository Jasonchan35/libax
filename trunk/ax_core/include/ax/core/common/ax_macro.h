#ifndef __ax_macro_h__
#define __ax_macro_h__

/*
#define	axForArray( T, V, ARR  )	for( T *(V)=(ARR).ptr(), *axEachEnd=(V)+(ARR).size(); (V)<axEachEnd; (V)++ )
#define axForDList( T, V, LIST )	for( T *(V)=(LIST).head(); (V); (V)=(V)->next() )
#define axForDListRev( T, V, LIST )	for( T *(V)=(LIST).tail(); (V); (V)=(V)->prev() )

#define	axForArrayRev( T, V, ARR )\
	for( T *(V)=(ARR).size()?&(ARR).last():NULL, *axEachEnd=(ARR).size()?(ARR).ptr():(ARR).ptr()+1; (V)>=axEachEnd; (V)-- )
//-----
*/

#define axL(_String) L ## _String

#define axJOIN_WORD(w0,w1)		w0##w1
#define axJOIN_WORD3(w0,w1,w2)	w0 ## w1 ## w2

//TODO
//#define ax_release_assert(_Expression) { if( !(_Expression) ) _ax_assert(#_Expression, __FILE__, __LINE__ ); }


#endif //__ax_macro_h__
