@echo off
SETLOCAL

:: Check if Chocolatery is installed
choco -v >nul 2>&1
IF %ERRORLEVEL% NEQ 0 (
	echo Chocolatey is not installed!
	echo Installation...
	powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0install_choco.ps1"

	IF %ERRORLEVEL% NEQ 0 (
		echo Installation failed!
		ENDLOCAL
		pause
		exit /b 1
	) ELSE (
		echo You can now run Requirements.bat to install clang, git and make.
		echo You need to restart your terminal.
	)
) ELSE (
	echo You can run Requirements.bat to install clang, git and make.
)

ENDLOCAL
pause
