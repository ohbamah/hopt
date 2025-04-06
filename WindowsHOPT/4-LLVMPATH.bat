@echo off
SETLOCAL

set /p _in="Do you want to add HOPT to LLVM PATH ? [n] : "

IF /i "%_in%"=="y" (
	set /p _llvm_path="Enter LLVM PATH [C:\Program Files\LLVM\] : "
)

IF /i "%_in%"=="y" (
	IF /i "%_llvm_path%"=="" (
		set _llvm_path=C:\Program Files\LLVM\
	)
)

IF /i "%_in%"=="y" (

	echo LLVM PATH: '%_llvm_path%'
	dir %_llvm_path% >nul

	echo Copy of ..\includes\hopt.h to %_llvm_path%\include\
	copy ..\includes\hopt.h "%_llvm_path%\include\"
	echo Copy of .\libhopt.lib to %_llvm_path%\lib\
	copy .\libhopt.lib "%_llvm_path%\lib\"
)

echo.
echo "Finished."

ENDLOCAL
pause
