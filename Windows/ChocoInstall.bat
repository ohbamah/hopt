@echo off
SETLOCAL

:: Check if Chocolatery is installed
choco -v >nul 2>&1
IF %ERRORLEVEL% NEQ 0 (
	echo "Chocolatey is not installed!"
	echo "Installation..."
	powershell -NoProfile -ExecutionPolicy Bypass -Command ^
	 "Set-ExecutionPolicy Bypass -Scope Process -Force; ^
	  [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072: ^
	  iex ((New-Object System.Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1'))"
	IF %ERRORLEVEL% NEQ 0 (
		echo "Installation failed!"
		ENDLOCAL
		pause
		exit /b 1
	)
	ELSE (
		echo "You can now run Requirements.bat to install clang, git and make."
	)
)
ELSE (
	echo "You can run Requirements.bat to install clang, git and make."
)

ENDLOCAL
pause
