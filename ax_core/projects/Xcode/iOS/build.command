cd `dirname "$0"`
xcodebuild -project ax_core.xcodeproj -sdk iphoneos6.0 -configuration "Release" 
xcodebuild -project ax_core.xcodeproj -sdk iphoneos6.0 -configuration "Debug" 
xcodebuild -project ax_core.xcodeproj -sdk iphonesimulator6.0 -configuration "Release" 
xcodebuild -project ax_core.xcodeproj -sdk iphonesimulator6.0 -configuration "Debug" 


