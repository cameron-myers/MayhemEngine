for /f %%i in ('git rev-parse HEAD') do set commit_sha=%%i

7z a -mmt=on ZeppelinRush_%commit_sha.zip Assets Mayhem_Engine Mayhem_Wwise Libraries *.dll *.bat