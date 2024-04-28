# Create .build folder
Write-Host "Creating .build folder" -ForegroundColor Yellow;
New-Item -ItemType Directory -Force -Path .build

# Compile client
Write-Host "Compiling client_app.cpp file" -ForegroundColor Yellow;
g++ -o .build/client_app.exe app/client_app.cpp game/*.cpp net/*.cpp -lws2_32

# Compile server
Write-Host "Compiling server_app.cpp file" -ForegroundColor Yellow;
g++ -o .build/server_app.exe app/server_app.cpp game/*.cpp net/*.cpp -lws2_32

# Execute server and then client
Write-Host "Launching executions" -ForegroundColor Yellow;
Start powershell {Write-Host "Executing Server..." -ForegroundColor Blue; ./.build/server_app.exe;}
Start-Sleep -Seconds 2
Start powershell {Write-Host "Executing Client 1..." -ForegroundColor Green; ./.build/client_app.exe;}
Start-Sleep -Seconds 1
Start powershell {Write-Host "Executing Client 2..." -ForegroundColor Green; ./.build/client_app.exe;}
Start-Sleep -Seconds 1
Start powershell {Write-Host "Executing Client 3..." -ForegroundColor Green; ./.build/client_app.exe;}