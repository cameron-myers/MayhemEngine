 :echo off
echo.
echo. [Mayhem_Engine Build Script]
cd .\Mayhem_Engine

"C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\devenv.exe" "Mayhem_Engine.vcxproj" /build
rem *** Run the project ***
"Mayhem_Engine_x64_Debug.exe"

pause
