@echo off
SETLOCAL

:: Check if Chocolatery is installed
choco -v >nul 2>&1
IF %ERRORLEVEL% NEQ 0 (
	echo Please install Chocolatey manually or automatically with ChocoInstall.bat.
	ENDLOCAL
	pause
	exit /b 1
)

:: Upgrade Choco packages
choco upgrade chocolatey -y

:: llvm/clang verification
clang -v >nul 2>&1
IF %ERRORLEVEL% NEQ 0 (
	choco install -y llvm
	IF %ERRORLEVEL% NEQ 0 (
		echo llvm is now installed!
	) ELSE (
		echo Failed to install llvm..
	)
) ELSE (
	echo llvm already installed!
)

:: git verification
git -v >nul 2>&1
IF %ERRORLEVEL% NEQ 0 (
	choco install -y git
	IF %ERRORLEVEL% NEQ 0 (
		echo git is now installed!
	) ELSE (
		echo Failed to install git..
	)
) ELSE (
	echo git already installed!
)

:: make verification
make -v >nul 2>&1
IF %ERRORLEVEL% NEQ 0 (
	choco install -y make
	IF %ERRORLEVEL% NEQ 0 (
		echo make is now installed!
	) ELSE (
		echo Failed to install make..
	)
) ELSE (
	echo make already installed!
)

ENDLOCAL
pause
