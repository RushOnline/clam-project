; Script generated by the HM NIS Edit Script Wizard.

; HM NIS Edit Wizard helper defines
!define PRODUCT_NAME "CLAM-Voice2MIDI"
!define PRODUCT_VERSION "0.90-pre2"
!define PRODUCT_PUBLISHER "CLAM devel"
!define PRODUCT_WEB_SITE "http://www.iua.upf.es/mtg/clam"
!define PRODUCT_DIR_REGKEY "Software\Microsoft\Windows\CurrentVersion\App Paths\Voice2MIDI.exe"
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
   StrCpy $INSTDIR "$PROGRAMFILES\CLAM\Voice2MIDI"
   Goto +2
   StrCpy $INSTDIR "$0\Voice2MIDI"
   
FunctionEnd

Section "Principal" SEC01
  SetOutPath "$INSTDIR\bin"
  SetOverwrite ifnewer
  File "Voice2MIDI.exe"
  CreateDirectory "$SMPROGRAMS\CLAM\Voice2MIDI"
  CreateShortCut "$SMPROGRAMS\CLAM\Voice2MIDI\Voice2MIDI.lnk" "$INSTDIR\bin\Voice2MIDI.exe"
  CreateShortCut "$DESKTOP\Voice2MIDI.lnk" "$INSTDIR\bin\Voice2MIDI.exe"
SectionEnd

Section -AdditionalIcons
  SetOutPath $INSTDIR
  WriteIniStr "$INSTDIR\${PRODUCT_NAME}.url" "InternetShortcut" "URL" "${PRODUCT_WEB_SITE}"
  CreateShortCut "$SMPROGRAMS\CLAM\Voice2MIDI\Website.lnk" "$INSTDIR\${PRODUCT_NAME}.url"
  CreateShortCut "$SMPROGRAMS\CLAM\Voice2MIDI\Uninstall.lnk" "$INSTDIR\uninst.exe"
SectionEnd

Section -Post
  WriteUninstaller "$INSTDIR\uninst.exe"
  WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}" "" "$INSTDIR\bin\Voice2MIDI.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\bin\Voice2MIDI.exe"
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
  Delete "$INSTDIR\bin\Voice2MIDI.exe"

  Delete "$SMPROGRAMS\CLAM\Voice2MIDI\Uninstall.lnk"
  Delete "$SMPROGRAMS\CLAM\Voice2MIDI\Website.lnk"
  Delete "$DESKTOP\Voice2MIDI.lnk"
  Delete "$SMPROGRAMS\CLAM\Voice2MIDI\Voice2MIDI.lnk"

  RMDir "$SMPROGRAMS\CLAM\Voice2MIDI"
  RMDir "$INSTDIR\bin"

  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  DeleteRegKey HKLM "${PRODUCT_DIR_REGKEY}"
  SetAutoClose true
SectionEnd
