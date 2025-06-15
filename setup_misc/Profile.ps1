Set-Alias v nvim



# Import the Chocolatey Profile that contains the necessary code to enable
# tab-completions to function for `choco`.
# Be aware that if you are missing these lines from your profile, tab completion
# for `choco` will not function.
# See https://ch0.co/tab-completion for details.
$ChocolateyProfile = "$env:ChocolateyInstall\helpers\chocolateyProfile.psm1"
if (Test-Path($ChocolateyProfile)) {
  Import-Module "$ChocolateyProfile"
}

#Import-Module 'F:\vcpkg\scripts\posh-vcpkg'

function Add-DirectoryToPath {
    param (
        [Parameter(Mandatory=$true)]
        [string]$DirectoryPath
    )

    $currentPath = [Environment]::GetEnvironmentVariable("Path", "Machine")
    
    if ($currentPath -notlike "*$DirectoryPath*") {
        $newPath = $currentPath + ";$DirectoryPath"
        [Environment]::SetEnvironmentVariable("Path", $newPath, "Machine")
        Write-Host "Directory added to PATH: $DirectoryPath"
    } else {
        Write-Host "Directory already in PATH: $DirectoryPath"
    }
}

function Kill-All {
    param(
        [Parameter(Mandatory=$true)]
        [string]$SubString,

        [switch]$Force
    )

    # Get all processes with names containing the specified substring
    $processes = Get-Process | Where-Object { $_.Name -like "*$SubString*" }

    # Check if there are any processes found
    if ($processes) {
        # List the processes with more information
        Write-Host "Found the following processes:"
        $counter = 1
        @($processes) | foreach { "[$counter] Name: $($_.Name) - PID: $($_.Id) - Description: $($_.Description)" ; $counter++ }

        # Ask the user to confirm whether they want to kill the processes
        $confirmation = if ($Force) { 'Y' } else { Read-Host "Do you want to kill these processes? (Y/N)" }

        if ($confirmation -eq 'Y') {
            # Kill the processes
            foreach ($process in $processes) {
                Write-Host "Killing process: $($process.Name) - PID: $($process.Id)"
                Stop-Process -InputObject $process -Force
            }
        } else {
            Write-Host "Process killing cancelled."
        }
    } else {
        Write-Host "No processes found with the specified substring: $SubString"
    }
}

#region conda initialize
# !! Contents within this block are managed by 'conda init' !!
If (Test-Path "C:\ProgramData\miniconda3\Scripts\conda.exe") {
    (& "C:\ProgramData\miniconda3\Scripts\conda.exe" "shell.powershell" "hook") | Out-String | ?{$_} | Invoke-Expression
}
#endregion

git config --global alias.s "status"

function Write-ColoredWithPaths {
    param(
        [Parameter(Mandatory=$true)]
        [string]$Text,
        
        [Parameter(Mandatory=$true)]
        [System.ConsoleColor]$BaseColor
    )
    
    $pathPattern = '(?:[a-zA-Z]:\\[^:]+)|(?:/[^:]+)'
    if ($Text -match $pathPattern) {
        $match = $matches[0]
        $parts = $Text -split [regex]::Escape($match)
        Write-Host $parts[0] -ForegroundColor $BaseColor -NoNewline
        Write-Host $match -ForegroundColor Blue -NoNewline
        Write-Host $parts[1] -ForegroundColor $BaseColor
    } else {
        Write-Host $Text -ForegroundColor $BaseColor
    }
}

function Pretty-Log {
    $input | ForEach-Object {
        $line = $_
        switch -Regex ($line) {
            '^error:|error:|error C\d+:|C\d+: error' { 
                Write-ColoredWithPaths -Text $line -BaseColor Red
                continue 
            }
            '^warning:|warning:|warning C\d+:|C\d+: warning|WARNING:' { 
                Write-ColoredWithPaths -Text $line -BaseColor Yellow
                continue 
            }
            '(?:[a-zA-Z]:\\[^:]+)|(?:/[^:]+)' {
                Write-ColoredWithPaths -Text $line -BaseColor Grey
                continue
            }
            '^\s*\[\s*\d+%\]' { 
                Write-Host $line -ForegroundColor Cyan
                continue 
            }
            '^--\[.*\]' { 
                Write-Host $line -ForegroundColor Green
                continue 
            }
            default { Write-Host $line }
        }
    }
}


function Get-GitBranch {
    try {
        $branch = git rev-parse --abbrev-ref HEAD 2>$null
        if ($branch) {
            return " [$branch]"
        }
    }
    catch {
        return ""
    }
    return ""
}

function prompt {
    $currentPath = Get-Location
    $gitBranch = Get-GitBranch
    
    # Display conda environment if it exists
    $condaEnv = if ($env:CONDA_DEFAULT_ENV) { "($env:CONDA_DEFAULT_ENV) " } else { "" }
    
    # Create the prompt
    Write-Host "$condaEnv" -NoNewline -ForegroundColor Green
	# Write-Host "PS " -NoNewline -ForegroundColor White # we don't need this right?	
    Write-Host "$currentPath" -NoNewline -ForegroundColor Blue
    Write-Host "$gitBranch" -NoNewline -ForegroundColor Yellow
    Write-Host "> " -NoNewline -ForegroundColor White
    
    return " "
}
