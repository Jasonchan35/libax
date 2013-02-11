axTYPE_LIST( uint8_t )
axTYPE_LIST( uint16_t )
axTYPE_LIST( uint32_t )
axTYPE_LIST( uint64_t )

#if axTypeHas_long 
	axTYPE_LIST( unsigned long )
#endif

#if axTypeHas_long_long
	axTYPE_LIST( unsigned long long )
#endif
