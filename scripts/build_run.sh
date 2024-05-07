# Create .build folder
echo "Cretaing .build folder";
mkdir .build

# Compile client
echo "Compiling client_app.cpp file";
i686-w64-mingw32-gcc -o .build/client_app.exe app/client_app.cpp game/*.cpp net/*.cpp -lws2_32 -static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic;

# Compile server
echo "Compiling server_app.cpp file";
i686-w64-mingw32-gcc -o .build/server_app.exe app/server_app.cpp game/*.cpp net/*.cpp -lws2_32 -static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic;

# Execute server and then three clients
echo "Launching execution"
gnome-terminal -- bash -c "wine .build/server_app.exe; exec bash"
sleep 2
gnome-terminal -- bash -c "wine .build/client_app.exe; exec bash"
sleep 1
gnome-terminal -- bash -c "wine .build/client_app.exe; exec bash"
sleep 1
gnome-terminal -- bash -c "wine .build/client_app.exe; exec bash"