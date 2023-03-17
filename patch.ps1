Add-Type -AssemblyName System.Windows.Forms

# Select Bouyomi Directory
$dialog = New-Object System.Windows.Forms.FolderBrowserDialog
$dialog.ShowDialog() | Out-Null

if ($dialog.SelectedPath.Length -eq 0) {
    [System.Windows.Forms.MessageBox]::Show("You have to choose the Bouyomi Directory")
    exit 1
}

# Check if the directory contains a subdirectory named "AquesTalk"
$aquesTalkPath = Join-Path $dialog.SelectedPath "AquesTalk"
if (-not(Test-Path $aquesTalkPath -PathType Container)) {
    [System.Windows.Forms.MessageBox]::Show("Doesn't looks like a Bouyomi Directory")
    exit 1
}

# Rename all the files named "AquesTalk.dll" within the subdirectories to "_AquesTalk.dll"
$aquesTalkFiles = Get-ChildItem -Path $aquesTalkPath -Recurse -Filter "AquesTalk.dll"
foreach ($file in $aquesTalkFiles) {
    $newName = $file.Name -replace "AquesTalk.dll", "_AquesTalk.dll"
    Rename-Item -Path $file.FullName -NewName $newName -Force
    
    # Copy Dll1.dll to the directory where AquesTalk.dll was renamed
    $dll1Path = Join-Path (Split-Path $MyInvocation.MyCommand.Path) "Dll1.dll"
    $destPath = Join-Path (Split-Path $file.FullName) "AquesTalk.dll"
    Copy-Item -Path $dll1Path -Destination $destPath -Force
}

[System.Windows.Forms.MessageBox]::Show("Done")
