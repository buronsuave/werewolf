New-Item -ItemType Directory -Force -Path .build
g++ -o .build/server_test.exe server/test.cpp server/net/*.cpp -lws2_32