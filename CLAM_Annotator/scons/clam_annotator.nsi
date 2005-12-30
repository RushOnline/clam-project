; Script generated by the HM NIS Edit Script Wizard.

; HM NIS Edit Wizard helper defines
!define PRODUCT_NAME "CLAM-Annotator"
!define PRODUCT_VERSION "0.2"
!define PRODUCT_PUBLISHER "CLAM devel"
!define PRODUCT_WEB_SITE "http://www.iua.upf.es/mtg/clam"
!define PRODUCT_DIR_REGKEY "Software\Microsoft\Windows\CurrentVersion\App Paths\Annotator.exe"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"

; MUI 1.67 compatible ------
!include "MUI.nsh"

; MUI Settings
!define MUI_ABORTWARNING
!define MUI_ICON "${NSISDIR}\Contrib\Graphics\Icons\modern-install.ico"
!define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\modern-uninstall.ico"

!define ALL_USERS
!include WriteEnvStr.nsh # or the name you chose


; Language Selection Dialog Settings
!define MUI_LANGDLL_REGISTRY_ROOT "${PRODUCT_UNINST_ROOT_KEY}"
!define MUI_LANGDLL_REGISTRY_KEY "${PRODUCT_UNINST_KEY}"
!define MUI_LANGDLL_REGISTRY_VALUENAME "NSIS:Language"

; Welcome page
!insertmacro MUI_PAGE_WELCOME
; License page
!insertmacro MUI_PAGE_LICENSE "license.txt"
; Directory page
!insertmacro MUI_PAGE_DIRECTORY
; Instfiles page
!insertmacro MUI_PAGE_INSTFILES
; Finish page
;!define MUI_FINISHPAGE_RUN "$INSTDIR\bin\Annotator.exe"
!insertmacro MUI_PAGE_FINISH

; Uninstaller pages
!insertmacro MUI_UNPAGE_INSTFILES

; Language files
!insertmacro MUI_LANGUAGE "Catalan"
!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_LANGUAGE "Spanish"

; MUI end ------

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "${PRODUCT_NAME}-${PRODUCT_VERSION}_setup.exe"
InstallDirRegKey HKLM "${PRODUCT_DIR_REGKEY}" ""
ShowInstDetails show
ShowUnInstDetails show

Function .onInit
  !insertmacro MUI_LANGDLL_DISPLAY
   ReadEnvStr $0 CLAM_PATH
   StrCmp $0 "" 0 +6
   MessageBox MB_ICONEXCLAMATION|MB_YESNO "CLAM external dependencies have not been installed. Do you want to continue?" IDYES +2
   Abort
   MessageBox MB_ICONEXCLAMATION|MB_OK "The application won't work until you install the external libraries package located at ${PRODUCT_WEB_SITE}"
   StrCpy $INSTDIR "$PROGRAMFILES\CLAM\Annotator"
   Goto +2
   StrCpy $INSTDIR "$0\Annotator"
   
FunctionEnd

Section "Principal" SEC01
  SetOutPath "$INSTDIR\bin"
  SetOverwrite ifnewer
  File "Annotator.exe"
  CreateDirectory "$SMPROGRAMS\CLAM\Annotator"
  CreateShortCut "$SMPROGRAMS\CLAM\Annotator\Annotator.lnk" "$INSTDIR\bin\Annotator.exe"
  CreateShortCut "$DESKTOP\Annotator.lnk" "$INSTDIR\bin\Annotator.exe"
  SetOutPath "$INSTDIR\resources\sounds\"
  File "..\resources\sounds\click.wav"
  SetOutPath "$INSTDIR\Samples\"
  File "..\Samples\Schema.sc"
  File "..\Samples\Project.pro"
  SetOutPath "$INSTDIR\Samples\SongsTest"
  File "..\Samples\SongsTest\LisaRein-SomethingBetter.mp3"
  File "..\Samples\SongsTest\LisaRein-SomethingBetter.mp3.pool"
  File "..\Samples\SongsTest\LisaRein-spunkyfunk.mp3"
  File "..\Samples\SongsTest\LisaRein-spunkyfunk.mp3.pool"
  File "..\Samples\SongsTest\urls.txt"
SectionEnd

Section -AdditionalIcons
  SetOutPath $INSTDIR
  WriteIniStr "$INSTDIR\${PRODUCT_NAME}.url" "InternetShortcut" "URL" "${PRODUCT_WEB_SITE}"
  CreateShortCut "$SMPROGRAMS\CLAM\Annotator\Website.lnk" "$INSTDIR\${PRODUCT_NAME}.url"
  CreateShortCut "$SMPROGRAMS\CLAM\Annotator\Uninstall.lnk" "$INSTDIR\uninst.exe"
SectionEnd

Section -Post
  WriteUninstaller "$INSTDIR\uninst.exe"
  WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}" "" "$INSTDIR\bin\Annotator.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\bin\Annotator.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "${PRODUCT_WEB_SITE}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"
SectionEnd


Function un.onUninstSuccess
  HideWindow
  MessageBox MB_ICONINFORMATION|MB_OK "La desinstalaci�n de $(^Name) finaliz� satisfactoriamente."
FunctionEnd

Function un.onInit
!insertmacro MUI_UNGETLANGUAGE
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "�Est� completamente seguro que desea desinstalar $(^Name) junto con todos sus componentes?" IDYES +2
  Abort
FunctionEnd

Section Uninstall
  Delete "$INSTDIR\${PRODUCT_NAME}.url"
  Delete "$INSTDIR\uninst.exe"
  Delete "$INSTDIR\Samples\SongsTest\LisaRein-SomethingBetter.mp3"
  Delete "$INSTDIR\Samples\SongsTest\LisaRein-SomethingBetter.mp3.pool"
  Delete "$INSTDIR\Samples\SongsTest\LisaRein-spunkyfunk.mp3"
  Delete "$INSTDIR\Samples\SongsTest\LisaRein-spunkyfunk.mp3.pool"
  Delete "$INSTDIR\Samples\SongsTest\urls.txt"
  Delete "$INSTDIR\Samples\Project_win.pro"
  Delete "$INSTDIR\Samples\Schema.sc"
  Delete "$INSTDIR\resources\sounds\click.wav"
  Delete "$INSTDIR\bin\Annotator.exe"

  Delete "$SMPROGRAMS\CLAM\Annotator\Uninstall.lnk"
  Delete "$SMPROGRAMS\CLAM\Annotator\Website.lnk"
  Delete "$DESKTOP\Annotator.lnk"
  Delete "$SMPROGRAMS\CLAM\Annotator\Annotator.lnk"

  RMDir "$SMPROGRAMS\CLAM\Annotator"
  RMDir "$INSTDIR\Samples\SongsTest"
  RMDir "$INSTDIR\Samples\"
  RMDir "$INSTDIR\resources\sounds\"
  RMDir "$INSTDIR\resources\"
  RMDir "$INSTDIR\bin"

  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  DeleteRegKey HKLM "${PRODUCT_DIR_REGKEY}"
  SetAutoClose true
SectionEnd
