
# Get all processes with names starting with 'UE4'
Get-Process -Name UE4*
$ue4Processes = Get-Process -Name UE4*

# Iterate through each process and stop it
foreach ($process in $ue4Processes) {
    Write-Host "Stopping process: $($process.Name) (PID: $($process.Id))"
    Stop-Process -InputObject $process -Force
}
