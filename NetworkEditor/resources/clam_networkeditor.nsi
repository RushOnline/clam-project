; Script generated by the HM NIS Edit Script Wizard.

; HM NIS Edit Wizard helper defines
!define PRODUCT_NAME "CLAM-NetworkEditor"
!define PRODUCT_VERSION "${VERSION}"
!define PRODUCT_PUBLISHER "CLAM devel"
!define PRODUCT_WEB_SITE "http://clam.iua.upf.edu"
!define PRODUCT_DIR_REGKEY "Software\Microsoft\Windows\CurrentVersion\App Paths\NetworkEditor.exe"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"

; MUI 1.67 compatible ------
!include "MUI.nsh"

; MUI Settings
!define MUI_ABORTWARNING
!define MUI_ICON "${NSISDIR}\Contrib\Graphics\Icons\modern-install.ico"
!define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\modern-uninstall.ico"

!define ALL_USERS

; Language Selection Dialog Settings
!define MUI_LANGDLL_REGISTRY_ROOT "${PRODUCT_UNINST_ROOT_KEY}"
!define MUI_LANGDLL_REGISTRY_KEY "${PRODUCT_UNINST_KEY}"
!define MUI_LANGDLL_REGISTRY_VALUENAME "NSIS:Language"

; Welcome page
!insertmacro MUI_PAGE_WELCOME
; License page
!insertmacro MUI_PAGE_LICENSE "../COPYING"
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
OutFile "..\${PRODUCT_NAME}-${PRODUCT_VERSION}_setup.exe"
InstallDirRegKey HKLM "${PRODUCT_DIR_REGKEY}" ""
ShowInstDetails show
ShowUnInstDetails show


Function .onInit
  !insertmacro MUI_LANGDLL_DISPLAY
   StrCpy $INSTDIR "$PROGRAMFILES\CLAM\NetworkEditor"
   
FunctionEnd

Section "Principal" SEC01
  SetOutPath "$INSTDIR\bin"
  SetOverwrite ifnewer
  File "..\NetworkEditor.exe"
  File "..\Prototyper.exe"
  File '${QTDIR}\bin\designer.exe'
  File '${QTDIR}\lib\QtCore4.dll'
  File '${QTDIR}\lib\QtGui4.dll'
  File '${QTDIR}\lib\QtOpenGL4.dll'
  File '${QTDIR}\lib\QtXml4.dll'
  File '${QTDIR}\lib\QtNetwork4.dll'
  File '${QTDIR}\lib\QtDesigner4.dll'
  File '${QTDIR}\lib\QtDesignerComponents4.dll'
  File '${QTDIR}\lib\QtAssistantClient4.dll'
  File '${EXTERNALDLLDIR}\libsndfile.dll'
  File '${EXTERNALDLLDIR}\ogg.dll'
  File '${EXTERNALDLLDIR}\pthreadVCE.dll'
  File '${EXTERNALDLLDIR}\qt-mt322.dll'
  File '${EXTERNALDLLDIR}\vorbis.dll'
  File '${EXTERNALDLLDIR}\vorbisenc.dll'
  File '${EXTERNALDLLDIR}\vorbisfile.dll'
  File '${EXTERNALDLLDIR}\xerces-c_2_3_0.dll'
  File '${EXTERNALDLLDIR}\portaudio.dll'
  File '${VCRUNTIMEDIR}\msvcp71.dll'
  File '${VCRUNTIMEDIR}\msvcr71.dll'
  SetOutPath "$INSTDIR\bin\designer"
  File "..\CLAMWidgets.dll"
  SetOutPath "$INSTDIR\example-data\"
  File "..\example-data\*"
  SetOutPath "$INSTDIR\share\networkeditor\i18n"
  File "..\src\i18n\NetworkEditor_ca.qm"
  File "..\src\i18n\NetworkEditor_es.qm"

!define Index "Line${__LINE__}"
  ReadRegStr $1 HKCR ".clamnetwork" ""
  StrCmp $1 "" "${Index}-NoBackup"
    StrCmp $1 "CLAM-NetworkEditor.Network" "${Index}-NoBackup"
    WriteRegStr HKCR ".clamnetwork" "backup_val" $1
"${Index}-NoBackup:"
  WriteRegStr HKCR ".clamnetwork" "" "CLAM-NetworkEditor.Network"
  ReadRegStr $0 HKCR "CLAM-NetworkEditor.Network" ""
  StrCmp $0 "" 0 "${Index}-Skip"
	WriteRegStr HKCR "CLAM-NetworkEditor.Network" "" "CLAM processing network definition"
	WriteRegStr HKCR "CLAM-NetworkEditor.Network\shell" "" "open"
	WriteRegStr HKCR "CLAM-NetworkEditor.Network\DefaultIcon" "" "$INSTDIR\bin\NetworkEditor.exe,0"
"${Index}-Skip:"
  WriteRegStr HKCR "CLAM-NetworkEditor.Network\shell\open\command" "" '$INSTDIR\bin\Prototyper.exe "%1"'
  WriteRegStr HKCR "CLAM-NetworkEditor.Network\shell\edit" "" "Edit"
  WriteRegStr HKCR "CLAM-NetworkEditor.Network\shell\edit\command" "" '$INSTDIR\bin\NetworkEditor.exe "%1"'
 
  System::Call 'Shell32::SHChangeNotify(i 0x8000000, i 0, i 0, i 0)'
!undef Index

  SetOutPath "$INSTDIR\example-data\"
  CreateDirectory "$SMPROGRAMS\CLAM\NetworkEditor"
  CreateDirectory "$SMPROGRAMS\CLAM\NetworkEditor\Examples"
  CreateShortCut "$SMPROGRAMS\CLAM\NetworkEditor\Examples\Tonal Analysis.lnk" "$INSTDIR\example-data\tonalAnalysis-file.clamnetwork"
  CreateShortCut "$SMPROGRAMS\CLAM\NetworkEditor\NetworkEditor.lnk" "$INSTDIR\bin\NetworkEditor.exe"
  CreateShortCut "$SMPROGRAMS\CLAM\NetworkEditor\Prototyper.lnk" "$INSTDIR\bin\Prototyper.exe"
  CreateShortCut "$SMPROGRAMS\CLAM\NetworkEditor\QtDesigner.lnk" "$INSTDIR\bin\designer.exe"
  CreateShortCut "$DESKTOP\NetworkEditor.lnk" "$INSTDIR\bin\NetworkEditor.exe"
  CreateShortCut "$DESKTOP\Prototyper.lnk" "$INSTDIR\bin\Prototyper.exe"
SectionEnd

Section -AdditionalIcons
  SetOutPath $INSTDIR
  WriteIniStr "$INSTDIR\${PRODUCT_NAME}.url" "InternetShortcut" "URL" "${PRODUCT_WEB_SITE}"
  CreateShortCut "$SMPROGRAMS\CLAM\NetworkEditor\Website.lnk" "$INSTDIR\${PRODUCT_NAME}.url"
  CreateShortCut "$SMPROGRAMS\CLAM\NetworkEditor\Uninstall.lnk" "$INSTDIR\uninst.exe"
SectionEnd

Section -Post
  WriteUninstaller "$INSTDIR\uninst.exe"
  WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}" "" "$INSTDIR\bin\NetworkEditor.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\bin\NetworkEditor.exe"
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
  Delete "$DESKTOP\NetworkEditor.lnk"
  Delete "$DESKTOP\Prototyper.lnk"
  Delete "$SMPROGRAMS\CLAM\NetworkEditor\Examples\*"
  Delete "$SMPROGRAMS\CLAM\NetworkEditor\Uninstall.lnk"
  Delete "$SMPROGRAMS\CLAM\NetworkEditor\Website.lnk"
  Delete "$SMPROGRAMS\CLAM\NetworkEditor\NetworkEditor.lnk"
  Delete "$SMPROGRAMS\CLAM\NetworkEditor\Prototyper.lnk"
  Delete "$SMPROGRAMS\CLAM\NetworkEditor\QtDesigner.lnk"
  RMDir "$SMPROGRAMS\CLAM\NetworkEditor\Examples"
  RMDir "$SMPROGRAMS\CLAM\NetworkEditor"
  RMDir "$SMPROGRAMS\CLAM"

  Delete "$INSTDIR\${PRODUCT_NAME}.url"
  Delete "$INSTDIR\uninst.exe"

  Delete "$INSTDIR\example-data\*"
  RMDir "$INSTDIR\example-data"
  Delete "$INSTDIR\share\networkeditor\i18n\*"
  RMDir "$INSTDIR\share\networkeditor\i18n"
  RMDir "$INSTDIR\share\networkeditor"
  RMDir "$INSTDIR\share"
  Delete "$INSTDIR\bin\designer\*"
  RMDir "$INSTDIR\bin\designer"
  Delete "$INSTDIR\bin\*"
  RMDir "$INSTDIR\bin"
  RMDir "$INSTDIR"

!define Index "Line${__LINE__}"
  ReadRegStr $1 HKCR ".clamnetwork" ""
  StrCmp $1 "CLAM-NetworkEditor.Network" 0 "${Index}-NoOwn" ; only do this if we own it
    ReadRegStr $1 HKCR ".clamnetwork" "backup_val"
    StrCmp $1 "" 0 "${Index}-Restore" ; if backup="" then delete the whole key
      DeleteRegKey HKCR ".clamnetwork"
    Goto "${Index}-NoOwn"
"${Index}-Restore:"
      WriteRegStr HKCR ".clamnetwork" "" $1
      DeleteRegValue HKCR ".clamnetwork" "backup_val"
   
    DeleteRegKey HKCR "CLAM-NetworkEditor.Network" ;Delete key with association settings
 
    System::Call 'Shell32::SHChangeNotify(i 0x8000000, i 0, i 0, i 0)'
"${Index}-NoOwn:"
!undef Index

  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  DeleteRegKey HKLM "${PRODUCT_DIR_REGKEY}"
  SetAutoClose true
SectionEnd
