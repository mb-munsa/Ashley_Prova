$uproject = Get-ChildItem "*.uproject" -Name;
$bin = & { (Get-ItemProperty 'Registry::HKEY_CLASSES_ROOT\Unreal.ProjectFile\shell\rungenproj' -Name 'Icon' ).'Icon' };
$uprojectFolder = (Get-Item .).FullName
$uprojectPath = "`"$uprojectFolder\$uproject`""

$Refreshcommand = $bin + " -projectfiles $uprojectPath"
$OpenProjectcommand = $bin + " -editor $uprojectPath";

$VSFolder = "$uprojectFolder\.vs"
$BinariesFolder = "$uprojectFolder\Binaries"
$IntermidiateFolder = "$uprojectFolder\Intermediate"
$SLNPathr = "$uprojectFolder\*.sln"

Remove-Item -Recurse -Force -ErrorAction SilentlyContinue $VSFolder 
Remove-Item -Recurse -Force -ErrorAction SilentlyContinue $BinariesFolder
Remove-Item -Recurse -Force -ErrorAction SilentlyContinue $IntermidiateFolder
remove-item $SLNPathr

cmd.exe /c $Refreshcommand
cmd.exe /c $OpenProjectcommand