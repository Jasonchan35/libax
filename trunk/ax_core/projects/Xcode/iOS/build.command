cd `dirname "$0"`
xcodebuild -project ax_core.xcodeproj -sdk iphoneos5.1 -configuration "Release" 
xcodebuild -project ax_core.xcodeproj -sdk iphoneos5.1 -configuration "Debug" 
xcodebuild -project ax_core.xcodeproj -sdk iphonesimulator5.1 -configuration "Release" 
xcodebuild -project ax_core.xcodeproj -sdk iphonesimulator5.1 -configuration "Debug" 


