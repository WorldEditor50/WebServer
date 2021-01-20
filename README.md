# CWS WebServer
Cream Web Server  
A web server frame by C++11  
## <1> What each directory for?
directories|for  
:--:|--  
commlib|some basic tools  
frame|utilities for server frame
src|classes of business  
test|functions and classes of test
## <2> How to build?
+ install third-party libs:
  + protobuf
  + mysqlclient
+ start build by the following steps:  
`mkdir build`  
`cd build`  
`cmake ..`  
`make`
## <3> How to start develope?
Start by the following steps:  
+ replace the *.proto in src/ by your own
+ cmake and get the stubs in build/stubs/
+ TODO...
## <4> Level of internal dependency
level|directories  
:--:|--  
basic|commlib  
frame|frame  
business|src  