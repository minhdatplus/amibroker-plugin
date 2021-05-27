@echo off
SET WIX_TEMP=C:/AmiPluginInstallerEnv/WixTemp
cd %2
copy %2\Welcome.png %3\Welcome.png
copy %2\Icon.ico %3\Icon.ico
copy %2\logo.ico %3\logo.ico
%1 %3\Product.wixobj %2\SSI_InstallDir.wixobj %2\WixUI_SSI.wixobj -ext WixUIExtension -sval -out %3\%4
cd %3
copy %2\eula.rtf %3\eula.rtf
%1 %2\Bundle.wixobj -ext WixBalExtension -ext WixUtilExtension -out %3\iData_x64.exe