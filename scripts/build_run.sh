# Create .build folder
echo "Cretaing .build folder";
mkdir .build

# Create .build/logs folder
echo "Cretaing .build/logs folder";
mkdir .build/logs

# Create logs folder
echo "Cretaing logs folder";
mkdir logs

# Compile client
echo "Compiling client_app.cpp file";
i686-w64-mingw32-gcc -o .build/client_app.exe app/client_app.cpp game/*.cpp net/*.cpp -lws2_32 -static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic;

# Compile server
echo "Compiling server_app.cpp file";
i686-w64-mingw32-gcc -o .build/server_app.exe app/server_app.cpp game/*.cpp net/*.cpp -lws2_32 -static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic;

# Execute server
echo "Launching execution"
gnome-terminal -- bash -c "wine .build/server_app.exe; exec bash"
sleep 2

# Launch 9 clients in sequence
for i in {1..9}
do
    echo "Launching client $i..."
    gnome-terminal -- bash -c "echo 'Executing Client $i...'; wine .build/client_app.exe; exec bash"
    sleep 1
done
