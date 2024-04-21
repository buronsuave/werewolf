New-Item -ItemType Directory -Force -Path .build
g++ -o .build/client_test.exe client/test.cpp client/net/Client.cpp -lws2_32
