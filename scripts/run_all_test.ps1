# Execute server and then client
Start powershell {Write-Host "Executing Server..." -ForegroundColor Blue; ./.build/server_test.exe; sleep;}
Start-Sleep -Seconds 2
Start powershell {Write-Host "Executing Client 1..." -ForegroundColor Green; ./.build/client_test.exe; sleep;}
Start-Sleep -Seconds 1
Start powershell {Write-Host "Executing Client 2..." -ForegroundColor Green; ./.build/client_test.exe; sleep;}
Start-Sleep -Seconds 1
Start powershell {Write-Host "Executing Client 3..." -ForegroundColor Green; ./.build/client_test.exe; sleep;}