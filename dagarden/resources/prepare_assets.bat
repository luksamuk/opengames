@echo off

::Copy assets to temporary folder
echo ** Copying assets folder to temporary folder...
xcopy ..\assets .\assets_temp /i /s

::Remove all .png extensions
for /r .\assets_temp %%i in (*.png) do ren "%%i" "%%~ni"

::Remove all .ogg extensions
for /r .\assets_temp %%i in (*.ogg) do ren "%%i" "%%~ni"

::Compile all .gs scripts
echo ** Compiling Gongly Scripts...
mkdir .\assets_temp\scripts
cd ..\scripts
for /r .\ %%i in (*.gs) do echo *** Compiling %%i... && ggsc "%%i" -o "%%~ni" && move "%%~ni" ..\resources\assets_temp\scripts\
cd ..\resources

::Create compressed archive
echo ** Creating compressed folder...
7z a data.zip .\assets_temp\*

::If there's already a compressed folder, delete it
echo ** Deleting old compressed folder, if existant...
del ..\data.dat /Q

::Rename compressed folder to .dat format
echo ** Moving compressed folder...
rename data.zip data.dat
move data.dat ..\data.dat

::Remove temporary folder
echo ** Removing temporary assets folder...
rmdir .\assets_temp /s /q
echo ** Done!
