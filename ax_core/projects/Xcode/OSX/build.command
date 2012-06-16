cd `dirname "$0"`
xcodebuild -project ax_core.xcodeproj -sdk macosx10.7 -configuration "Release" 
xcodebuild -project ax_core.xcodeproj -sdk macosx10.7 -configuration "Debug" 

