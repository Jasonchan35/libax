
#include <ax/ax_core.h>



int main(int argc, char *argv[]) {

    ax_log( "1234" );
    
    
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    int retVal = UIApplicationMain(argc, argv, nil, nil);
    [pool release];
    return retVal;
}
