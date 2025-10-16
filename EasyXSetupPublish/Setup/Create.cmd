cd VS
"F:\Program Files\7-Zip\7z.exe" a -t7z -mx=9 ..\EasyX_Setup_25.9.10.7z "*"
cd..
copy /b Res\7zS.sfx + Res\config.txt + EasyX_Setup_25.9.10.7z EasyX_Setup_25.9.10.exe
pause