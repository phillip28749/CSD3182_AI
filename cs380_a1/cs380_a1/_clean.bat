for %%I in (.) do set project=%%~nxI

del *.o
del *.out
del /A:H *.suo
del /A:H *.opensdf
del /A:H *.zip
rmdir /Q /S .vs
rmdir /Q /S .vscode
rmdir /Q /S x64
rmdir /Q /S Debug
rmdir /Q /S Release
rmdir /Q /S SVN
rmdir /Q /S VPL
rmdir /Q /S cs380_%project%
rmdir /Q /S %project%