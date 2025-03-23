for /f %%i in ('git rev-parse HEAD') do set GIT_COMMIT=%%i

7z a -mmt=on ZeppelinRush_%GIT_COMMIT%.zip Assets Mayhem_Engine Mayhem_Wwise Libraries *.dll *.bat