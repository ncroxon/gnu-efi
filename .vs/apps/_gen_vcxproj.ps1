$Source = "AllocPages"
$EfiArchName = @{
  "x64" = "x64"
  "Win32" = "ia32"
  "ARM64" = "aa64"
  "ARM" = "arm"
}
$i = 0
$sln = (Get-Content ..\..\gnu-efi.sln) -as [collections.arraylist]
Get-ChildItem "..\..\apps" -Filter *.c |
Foreach-Object {
  if ($_.BaseName -ne $Source) {
    $guid = [guid]::NewGuid().ToString().ToUpper()
    Write-Host $guid = $_.BaseName
    copy "$Source.vcxproj" "$($_.BaseName).vcxproj"
    copy "$Source.vcxproj.filters" "$($_.BaseName).vcxproj.filters"
    copy "$Source.vcxproj.user" "$($_.BaseName).vcxproj.user"
    (Get-Content "$($_.BaseName).vcxproj").Replace("DFA0BA98-D0BA-4176-9A34-B5BA6355B1DE", $guid).Replace($Source, $_.BaseName) | Set-Content "$($_.BaseName).vcxproj"
    (Get-Content "$($_.BaseName).vcxproj.filters").Replace($Source, $_.BaseName) | Set-Content "$($_.BaseName).vcxproj.filters"
    $line = 14 + 5 * $i
    $sln.insert($line,"Project(""{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}"") = ""$($_.BaseName)"", "".vs\apps\$($_.BaseName).vcxproj"", ""{$guid}""")
    $line++
    $sln.insert($line,"	ProjectSection(ProjectDependencies) = postProject")
    $line++
    $sln.insert($line,"		{3135D563-9596-4584-9ED6-616ADEC52974} = {3135D563-9596-4584-9ED6-616ADEC52974}")
    $line++
    $sln.insert($line,"	EndProjectSection")
    $line++
    $sln.insert($line,"EndProject")
    $line++
    $line += 44 + 16 * $i
    foreach ($Build in @("Debug", "Release")) {
      foreach ($Arch in @("ARM64", "ARM", "Win32", "x64")) {
        foreach ($Cfg in @("ActiveCfg", "Build.0")) {
          $sln.insert($line, "		{$guid}.$Build|$($EfiArchName.$Arch).$Cfg = $Build|$Arch")
          $line++
        }
      }
    }
    $line += 6 + $i
    $sln.insert($line, "		{$guid} = {DF105116-5A3A-4D48-AB1D-7221690ED545}")
    $i++
  }
}
$sln | Set-Content ..\..\gnu-efi.sln 