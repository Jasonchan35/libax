#include <ax/core/database/axDBO_Stmt.h>


axStatus axDBO_Stmt::exec() {
	if( !p_ ) return axStatus::not_initialized;
	return p_->exec();
}