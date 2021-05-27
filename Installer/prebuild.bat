@echo off
wix311-binaries\candle.exe Product.wxs WixUI_CB180080.wxs CB180080_InstallDir.wxs Bundle.wxs -ext WixUtilExtension  -ext WixBalExtension -out ./obj/
wix311-binaries\light.exe ./obj/Product.wixobj ./obj/CB180080_InstallDir.wixobj ./obj/WixUI_CB180080.wixobj -ext WixUIExtension -sval -out ./obj/CB180080-SOICT-BKHN_x64.msi
wix311-binaries\light.exe ./obj/Bundle.wixobj -ext WixBalExtension  -ext WixUtilExtension -sval -out ./bin/CB180080-SOICT-BKHN_x64.exe
