#ifndef __ax_macro_h__
#define __ax_macro_h__

#define	axForArray( T, V, ARR  )	for( T *(V)=(ARR).ptr(), *axEachEnd=(V)+(ARR).size(); (V)<axEachEnd; (V)++ )
#define axForDList( T, V, LIST )	for( T *(V)=(LIST).head(); (V); (V)=(V)->next() )
#define axForDListRev( T, V, LIST )	for( T *(V)=(LIST).tail(); (V); (V)=(V)->prev() )

#define	axForArrayRev( T, V, ARR )\
	for( T *(V)=(ARR).size()?&(ARR).last():NULL, *axEachEnd=(ARR).size()?(ARR).ptr():(ARR).ptr()+1; (V)>=axEachEnd; (V)-- )
//-----

#define axMakeName2( Name0, Name1 )			Name0##Name1
#define axMakeName3( Name0, Name1, Name2 )	Name0##Name1##Name2


#endif //__ax_macro_h__
