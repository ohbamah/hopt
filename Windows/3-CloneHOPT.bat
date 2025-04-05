@echo off
SETLOCAL

git clone https://github.com/ohbamah/hopt.git
cd ./hopt/

set /p _in="Do you want to add HOPT to your PATH ? [y/n] : "

IF /i "%_in%"=="y" (
	set "newpath=%cd%"
	for /f "usebackq tokens=2*" %%a in (`reg query "HKLM\SYSTEM\CurrentControlSet\Control\Session Manager\Environment" /v Path`) do set "oldpath=%%b"

	setx PATH "%oldpath%;%newpath%" /M
	echo.
	IF %ERRORLEVEL% NEQ 0 (
		echo "❌ Error: Path "%newpath%" not added to the system PATH."
	)
	ELSE (
		echo "✅ Success: Path "%newpath%" added to the system PATH."
	)
)
ELSE (
	echo.
	echo "Finished."
)

ENDLOCAL
pause
