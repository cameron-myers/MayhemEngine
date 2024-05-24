cd Mayhem_Engine
START "" "Mayhem_Engine_x64_Debug.exe"

:LOOP
PSLIST Mayhem_Engine_x64_Debug >nul 2>&1
IF ERRORLEVEL 1 (
  GOTO CONTINUE
) ELSE (
  ECHO Test is still running
  TIMEOUT /T 5
  GOTO LOOP
)

:CONTINUE
MAYHEM_ENGINE