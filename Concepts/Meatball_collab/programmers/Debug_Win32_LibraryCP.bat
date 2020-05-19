@echo off	

REM To do: sdl2_net; sdl2_ttf; freetype (?).
REM Clean up, learn how to concatenate strings.
REM Rename SDL2-nfiwenf to SDL after extracting

	Set /P vDest= Path (Meatball truck directory):

	Echo Copying libraries to: %vDest%

	Echo Continue?

	pause

REM Echo --- SDL include folder ---

	Set vSource=SDL\include
	REM xcopy %vSource% %vDest%\Lib\sdl2\include /f

REM Echo --- SDL.dll; SDL2.lib; SDL2main.lib ---

REM Echo From C:\Users\Cameron\Desktop\MEATBALL\SDL\VisualC\SDL\Win32\Debug

REM Echo From C:\Users\Cameron\Desktop\MEATBALL\SDL\VisualC\SDLmain\Win32\Debug

REM Echo To C:\Users\Cameron\Desktop\meatball-code-131-trunk\Lib\sdl2\md\libd\x86

	Set vSource=SDL\VisualC\SDL\Win32\Debug\SDL2.dll
	xcopy %vSource% %vDest%\Lib\sdl2\md\libd\x86 /f

	Set vSource=SDL\VisualC\SDL\Win32\Debug\SDL2.lib
	xcopy %vSource% %vDest%\Lib\sdl2\md\libd\x86 /f

	Set vSource=SDL\VisualC\SDLmain\Win32\Debug\SDL2main.lib
	xcopy %vSource% %vDest%\Lib\sdl2\md\libd\x86 /f

REM Echo --- SDL2_image external ---

REM Echo From C:\Users\Cameron\Desktop\MEATBALL\SDL2_image-2.0.0\VisualC\external\lib\x86

REM Echo To C:\Users\Cameron\Desktop\meatball-code-131-trunk\Lib\sdl2_image\external

	Set vSource=SDL2_image-2.0.0\VisualC\external\lib\x86
	xcopy %vSource% %vDest%\Lib\sdl2_image\external /f

REM Echo --- SDL2_image include ---

REM Echo From C:\Users\Cameron\Desktop\MEATBALL\SDL2_image-2.0.0\SDL_image.h

REM Echo To C:\Users\Cameron\Desktop\meatball-code-131-trunk\Lib\sdl2_image\include

	Set vSource=SDL2_image-2.0.0\SDL_image.h
	xcopy %vSource% %vDest%\Lib\sdl2_image\include /f

REM Echo --- SDL2_image.dll; SDL2_image.lib ---

REM Echo From C:\Users\Cameron\Desktop\MEATBALL\SDL2_image-2.0.0\VisualC\Win32\Debug

REM Echo To C:\Users\Cameron\Desktop\meatball-code-131-trunk\Lib\sdl2_image\md\libd\x86

	Set vSource=SDL2_image-2.0.0\VisualC\Win32\Debug\SDL2_image.dll
	xcopy %vSource% %vDest%\Lib\sdl2_image\md\libd\x86 /f

	Set vSource=SDL2_image-2.0.0\VisualC\Win32\Debug\SDL2_image.lib
	xcopy %vSource% %vDest%\Lib\sdl2_image\md\libd\x86 /f

REM Echo --- SDL2_mixer external ---

REM Echo From C:\Users\Cameron\Desktop\MEATBALL\SDL2_mixer-2.0.0\VisualC\external\lib\x86

REM Echo To C:\Users\Cameron\Desktop\meatball-code-131-trunk\Lib\sdl2_mixer\external

	Set vSource=SDL2_mixer-2.0.0\VisualC\external\lib\x86
	xcopy %vSource% %vDest%\Lib\sdl2_mixer\external /f

REM Echo --- SDL2_mixer include ---

REM Echo From C:\Users\Cameron\Desktop\MEATBALL\SDL2_mixer-2.0.0

REM Echo To C:\Users\Cameron\Desktop\meatball-code-131-trunk\Lib\sdl2_mixer\include

	Set vSource=SDL2_mixer-2.0.0\SDL_mixer.h
	xcopy %vSource% %vDest%\Lib\sdl2_mixer\include /f

REM Echo --- SDL2_image.dll; SDL2_image.lib ---

REM Echo From C:\Users\Cameron\Desktop\MEATBALL\SDL2_mixer-2.0.0\VisualC\Debug

REM Echo To C:\Users\Cameron\Desktop\meatball-code-131-trunk\Lib\sdl2_mixer\md\libd\x86

	Set vSource=SDL2_mixer-2.0.0\VisualC\Debug\SDL2_mixer.dll
	xcopy %vSource% %vDest%\Lib\sdl2_mixer\md\libd\x86 /f

	Set vSource=SDL2_mixer-2.0.0\VisualC\Debug\SDL2_mixer.lib
	xcopy %vSource% %vDest%\Lib\sdl2_mixer\md\libd\x86 /f

Echo Done.

pause