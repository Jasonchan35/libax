cd `dirname "$0"`

# http://www.oracle.com/technetwork/topics/intel-macsoft-096467.html
# Download instantclient-sdk-macos.x64-11.2.0.3.0.zip and instantclient-basic-macos.x64-11.2.0.3.0.zip

rm -Rf oracle

mkdir oracle/
mkdir oracle/tmp/
mkdir oracle/tmp/base
mkdir oracle/tmp/sdk

echo uncompress ...

unzip -o -qq instantclient-basic-macos.x64-11.2.0.3.0.zip -d oracle/tmp/base
unzip -o -qq instantclient-sdk-macos.x64-11.2.0.3.0       -d oracle/tmp/sdk

echo copy header ... 

mkdir oracle/OSX
mv -f oracle/tmp/sdk/instantclient_11_2/sdk/include/* oracle/OSX/

echo copy lib ... 

rm -Rf ../lib/OSX/oracle_dylib
mkdir ../lib/OSX/oracle_dylib
mv -f oracle/tmp/base/instantclient_11_2/lib* ../lib/OSX/oracle_dylib/
ln -s libclntsh.dylib.11.1 ../lib/OSX/oracle_dylib/libclntsh.dylib

rm -Rf oracle/tmp/



