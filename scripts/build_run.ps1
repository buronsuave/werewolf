# Create .build folder
Write-Host "Creating .build folder" -ForegroundColor Yellow;
New-Item -ItemType Directory -Force -Path .build

# Create .build/logs folder
Write-Host "Creating .build/logs folder" -ForegroundColor Yellow;
New-Item -ItemType Directory -Force -Path .build/logs

# Create logs folder
Write-Host "Creating /logs folder" -ForegroundColor Yellow;
New-Item -ItemType Directory -Force -Path logs

# Compile client
Write-Host "Compiling client_app.cpp file" -ForegroundColor Yellow;
g++ -o .build/client_app.exe app/client_app.cpp game/*.cpp net/*.cpp -lws2_32

# Compile server
Write-Host "Compiling server_app.cpp file" -ForegroundColor Yellow;
g++ -o .build/server_app.exe app/server_app.cpp game/*.cpp net/*.cpp -lws2_32

# Execute server
Write-Host "Launching executions" -ForegroundColor Yellow;
Start powershell {Write-Host "Executing Server..." -ForegroundColor Blue; ./.build/server_app.exe;}
Start-Sleep -Seconds 2

# Launch 9 clients in sequence
for ($i = 1; $i -le 9; $i++) {
    $clientScript = "Write-Host 'Executing Client $i...' -ForegroundColor Green; ./.build/client_app.exe"
    Start-Process powershell -ArgumentList $clientScript
    Start-Sleep -Seconds 1
}
