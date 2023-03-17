var shell = new ActiveXObject("Shell.Application")
var fso = new ActiveXObject("Scripting.FileSystemObject")

// Select Bouyomi Directory
var folder = shell.BrowseForFolder(0, "Select Bouyomi Directory", 0)
var folderPath = folder ? folder.Self.Path : ""

if (!folderPath) {
    WScript.Echo("You have to choose the Bouyomi Directory")
    WScript.Quit(1)
}

// Check if the directory contains a subdirectory named "AquesTalk"
var aquesTalkPath = fso.BuildPath(folderPath, "AquesTalk")
if (!fso.FolderExists(aquesTalkPath)) {
    WScript.Echo("Doesn't looks like a Bouyomi Directory")
    WScript.Quit(1)
}

// Rename all the files named "AquesTalk.dll" within the subdirectories to "_AquesTalk.dll"
var subfolders = new Enumerator(fso.GetFolder(aquesTalkPath).SubFolders)
for (; !subfolders.atEnd(); subfolders.moveNext()) {
    var folder = subfolders.item()
    var aquesTalkFiles = folder.Files
    for (var fileEnum = new Enumerator(aquesTalkFiles); !fileEnum.atEnd(); fileEnum.moveNext()) {
        var file = fileEnum.item()
        if (file.Name !== "AquesTalk.dll") {
            continue
        }
        file.Name.replace("AquesTalk.dll", "_AquesTalk.dll")
        // Copy Dll1.dll to the directory where AquesTalk.dll was renamed
        var dll1Path = fso.BuildPath(WScript.ScriptFullName, "..\\Dll1.dll")
        var destPath = fso.BuildPath(file.ParentFolder.Path, "AquesTalk.dll")
        fso.CopyFile(dll1Path, destPath, true)
    }
}

WScript.Echo("Done")
