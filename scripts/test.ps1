Write-Host "######### Compiling Server #########" -ForegroundColor Blue;
& "$PSScriptRoot/build_server_test.ps1"
Write-Host "######### Compiling Client #########" -ForegroundColor Green;
& "$PSScriptRoot/build_client_test.ps1"
Write-Host "######### Starting Executions [4 windows] #########" -ForegroundColor Yellow;
& "$PSScriptRoot/run_all_test.ps1"
