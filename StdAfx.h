// StdAfx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#if !defined(AFX_STDAFX_H__4375E7D6_7EE8_4D87_93EA_479ABD8D4A5B__INCLUDED_)
#define AFX_STDAFX_H__4375E7D6_7EE8_4D87_93EA_479ABD8D4A5B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable: 4786)
//#pragma warning(disable: 4098)

#define STRICT
#ifndef _WIN32_WINNT
	#if _MSC_VER >= 1500
		#define _WIN32_WINNT 0x0501
	#else
		#define _WIN32_WINNT 0x0400
	#endif
#endif

// 'DEBUG workaround' below prevents the MFC or ATL #include-s 
// from pulling in "afx.h" that would force the debug CRT through
// #pragma-s.
#if defined(_DEBUG) && !defined(_FULLDEBUG_)
#define _DEBUG_WAS_DEFINED
#undef _DEBUG
#pragma message ("     Compiling MFC header files in release mode.")
#endif

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC Automation classes
#endif // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_DB_SUPPORT
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>         // MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT

#include <afxdtctl.h>       // MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>         // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <vector>
using namespace std;

#ifdef S4NEWNET
	#ifdef _WIN64
		#include "c:\\90\\x64\\sense4.h"
	#else
		#include "c:\\90\\newnet\\sense4.h"
	#endif
#else
	#include "c:\\90\\sense4.h"
#endif

#ifdef _WIN64
	#ifdef S4NEWNET
		#pragma comment(lib, "c:\\90\\x64\\net\\VS2008_lib\\release_md_vs2008\\sense4userSt_20120809.lib")
	#else
		#pragma comment(lib, "c:\\90\\x64\\single\\release_md_vs2008\\Sense4ST.lib")
	#endif
	#pragma comment(lib, "c:\\90\\x64\\single\\vs2008\\Cryptst.lib")
#else
	#ifdef S4NEWNET
		#pragma comment(lib, "c:\\90\\sense4userSt.lib")
	#else
		#pragma comment(lib, "c:\\90\\Sense4ST.lib")
	#endif
	#pragma comment(lib, "c:\\90\\Cryptst.lib")
#endif

#ifdef CODEMETER
#ifdef _WIN64
#pragma comment(lib, "c:\\90\\WupiEngine64.lib")
#pragma comment(lib, "c:\\90\\WibuCm64.lib")
#else
#pragma comment(lib, "c:\\90\\WupiEngine32.lib")
#pragma comment(lib, "c:\\90\\WibuCm32.lib")
#endif
#endif

// AutoCAD MFC Extensions:
#include "adui.h"
#include "acui.h"
#include "acuiComboBox.h"
#include "acuiDialog.h"
#include "dbgroup.h"
#include "gecurv2d.h"
#include "gecurv3d.h"
#include "adsdef.h"
#include "dbmain.h"
#include "dbregion.h"
#include "dbspline.h"
#include "dbray.h"
#include "dbtrans.h"
#include "actrans.h"
#include "dbxrecrd.h"
#include "dbtable.h"
#include "dbhatch.h "
#include "dbelipse.h"

//cass include files
#include "AutoLoop.h"
#include "AutoDiv.h"
#include "AutoGrid.h"
#include "AutoInfo.h"


#ifndef __CASS90_GLOBAL__
#include "C:\90\cassGlobal.h"
#endif /*__CASS90_GLOBAL__*/

#include "entitysOpe.h"

//
using namespace tool;
using namespace AutoB;
//namespace cass{};
using namespace cass;


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#ifdef _DEBUG_WAS_DEFINED
#define _DEBUG
#undef _DEBUG_WAS_DEFINED
#endif


extern HINSTANCE _hdllInstance ;
#include "rxdebug.h"
// RX Includes
#include "acdb.h"               // acdb definitions
#include "rxregsvc.h"           // ARX linker
#include "dbapserv.h"           // Host application services
#include "aced.h"               // aced stuff
#include "adslib.h"             // RXADS definitions
#include "acdocman.h"           // MDI document manager
#include "rxmfcapi.h"           // ObjectARX MFC support
#include "AcExtensionModule.h"  // Utility class for extension dlls
#include <afxcontrolbars.h>


#ifdef _FULLDEBUG_
#pragma message ("Warning! You should better not run this application in a Standard AutoCAD release.")
#pragma message ("         Due to the memory managment constraints.")
#endif

#endif // !defined(AFX_STDAFX_H__4375E7D6_7EE8_4D87_93EA_479ABD8D4A5B__INCLUDED)
