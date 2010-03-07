/*
Copyright (c) 1993-2008, Cognitive Technologies
All rights reserved.

Разрешается повторное распространение и использование как в виде исходного кода,
так и в двоичной форме, с изменениями или без, при соблюдении следующих условий:

      * При повторном распространении исходного кода должны оставаться указанное
        выше уведомление об авторском праве, этот список условий и последующий
        отказ от гарантий.
      * При повторном распространении двоичного кода в документации и/или в
        других материалах, поставляемых при распространении, должны сохраняться
        указанная выше информация об авторском праве, этот список условий и
        последующий отказ от гарантий.
      * Ни название Cognitive Technologies, ни имена ее сотрудников не могут
        быть использованы в качестве средства поддержки и/или продвижения
        продуктов, основанных на этом ПО, без предварительного письменного
        разрешения.

ЭТА ПРОГРАММА ПРЕДОСТАВЛЕНА ВЛАДЕЛЬЦАМИ АВТОРСКИХ ПРАВ И/ИЛИ ДРУГИМИ ЛИЦАМИ "КАК
ОНА ЕСТЬ" БЕЗ КАКОГО-ЛИБО ВИДА ГАРАНТИЙ, ВЫРАЖЕННЫХ ЯВНО ИЛИ ПОДРАЗУМЕВАЕМЫХ,
ВКЛЮЧАЯ ГАРАНТИИ КОММЕРЧЕСКОЙ ЦЕННОСТИ И ПРИГОДНОСТИ ДЛЯ КОНКРЕТНОЙ ЦЕЛИ, НО НЕ
ОГРАНИЧИВАЯСЬ ИМИ. НИ ВЛАДЕЛЕЦ АВТОРСКИХ ПРАВ И НИ ОДНО ДРУГОЕ ЛИЦО, КОТОРОЕ
МОЖЕТ ИЗМЕНЯТЬ И/ИЛИ ПОВТОРНО РАСПРОСТРАНЯТЬ ПРОГРАММУ, НИ В КОЕМ СЛУЧАЕ НЕ
НЕСЁТ ОТВЕТСТВЕННОСТИ, ВКЛЮЧАЯ ЛЮБЫЕ ОБЩИЕ, СЛУЧАЙНЫЕ, СПЕЦИАЛЬНЫЕ ИЛИ
ПОСЛЕДОВАВШИЕ УБЫТКИ, СВЯЗАННЫЕ С ИСПОЛЬЗОВАНИЕМ ИЛИ ПОНЕСЕННЫЕ ВСЛЕДСТВИЕ
НЕВОЗМОЖНОСТИ ИСПОЛЬЗОВАНИЯ ПРОГРАММЫ (ВКЛЮЧАЯ ПОТЕРИ ДАННЫХ, ИЛИ ДАННЫЕ,
СТАВШИЕ НЕГОДНЫМИ, ИЛИ УБЫТКИ И/ИЛИ ПОТЕРИ ДОХОДОВ, ПОНЕСЕННЫЕ ИЗ-ЗА ДЕЙСТВИЙ
ТРЕТЬИХ ЛИЦ И/ИЛИ ОТКАЗА ПРОГРАММЫ РАБОТАТЬ СОВМЕСТНО С ДРУГИМИ ПРОГРАММАМИ,
НО НЕ ОГРАНИЧИВАЯСЬ ЭТИМИ СЛУЧАЯМИ), НО НЕ ОГРАНИЧИВАЯСЬ ИМИ, ДАЖЕ ЕСЛИ ТАКОЙ
ВЛАДЕЛЕЦ ИЛИ ДРУГОЕ ЛИЦО БЫЛИ ИЗВЕЩЕНЫ О ВОЗМОЖНОСТИ ТАКИХ УБЫТКОВ И ПОТЕРЬ.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.
    * Neither the name of the Cognitive Technologies nor the names of its
      contributors may be used to endorse or promote products derived from this
      software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 3.03.0110 */
/* at Thu Sep 11 10:57:03 1997
 */
/* Compiler settings for wtypes.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: none
*/
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __wtypes_h__
#define __wtypes_h__

/* Forward Declarations */

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * );

/****************************************
 * Generated header for interface: __MIDL_itf_wtypes_0000
 * at Thu Sep 11 10:57:03 1997
 * using MIDL 3.03.0110
 ****************************************/
/* [local] */


//+-------------------------------------------------------------------------
//
//  Microsoft Windows
//  Copyright 1992 - 1998 Microsoft Corporation.
//
//--------------------------------------------------------------------------


extern RPC_IF_HANDLE __MIDL_itf_wtypes_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_wtypes_0000_v0_0_s_ifspec;

#ifndef __IWinTypes_INTERFACE_DEFINED__
#define __IWinTypes_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IWinTypes
 * at Thu Sep 11 10:57:03 1997
 * using MIDL 3.03.0110
 ****************************************/
/* [auto_handle][unique][version][uuid] */


typedef struct  tagRemHGLOBAL {
    long fNullHGlobal;
    unsigned long cbData;
    /* [size_is] */
    byte data[ 1 ];
}   RemHGLOBAL;

typedef struct  tagRemHMETAFILEPICT {
    long mm;
    long xExt;
    long yExt;
    unsigned long cbData;
    /* [size_is] */
    byte data[ 1 ];
}   RemHMETAFILEPICT;

typedef struct  tagRemHENHMETAFILE {
    unsigned long cbData;
    /* [size_is] */
    byte data[ 1 ];
}   RemHENHMETAFILE;

typedef struct  tagRemHBITMAP {
    unsigned long cbData;
    /* [size_is] */
    byte data[ 1 ];
}   RemHBITMAP;

typedef struct  tagRemHPALETTE {
    unsigned long cbData;
    /* [size_is] */
    byte data[ 1 ];
}   RemHPALETTE;

typedef struct  tagRemBRUSH {
    unsigned long cbData;
    /* [size_is] */
    byte data[ 1 ];
}   RemHBRUSH;

#if !defined(_WIN32) && !defined(_MPPC_)
// The following code is for Win16 only
#ifndef WINAPI          // If not included with 3.1 headers...
#define FAR             _far
#define PASCAL          _pascal
#define CDECL           _cdecl
#define VOID            void
#define WINAPI      FAR PASCAL
#define CALLBACK    FAR PASCAL
#ifndef FALSE
#define FALSE 0
#define TRUE 1
#endif // !FALSE
#ifndef _BYTE_DEFINED
#define _BYTE_DEFINED
typedef unsigned char uchar;

#endif // !_BYTE_DEFINED
#ifndef _WORD_DEFINED
#define _WORD_DEFINED
typedef unsigned short uint16_t;

#endif // !_WORD_DEFINED
typedef unsigned int uint;

typedef int int16_t;

typedef long Bool;

#ifndef _LONG_DEFINED
#define _LONG_DEFINED
typedef long int32_t;

#endif // !_LONG_DEFINED
#ifndef _WPARAM_DEFINED
#define _WPARAM_DEFINED
typedef uint WPARAM;

#endif // _WPARAM_DEFINED
#ifndef _LPARAM_DEFINED
#define _LPARAM_DEFINED
typedef int32_t LPARAM;

#endif // !_LPARAM_DEFINED
#ifndef _LRESULT_DEFINED
#define _LRESULT_DEFINED
typedef int32_t LRESULT;

#endif // !_LRESULT_DEFINED
typedef void __RPC_FAR *Handle;

typedef void __RPC_FAR *HMODULE;

typedef void __RPC_FAR *HINSTANCE;

typedef void __RPC_FAR *HRGN;

typedef void __RPC_FAR *HTASK;

typedef void __RPC_FAR *HKEY;

typedef void __RPC_FAR *HDESK;

typedef void __RPC_FAR *HMF;

typedef void __RPC_FAR *HEMF;

typedef void __RPC_FAR *HPEN;

typedef void __RPC_FAR *HRSRC;

typedef void __RPC_FAR *HSTR;

typedef void __RPC_FAR *HWINSTA;

typedef void __RPC_FAR *HKL;

typedef void __RPC_FAR *HGDIOBJ;

typedef Handle HDWP;

#ifndef _HFILE_DEFINED
#define _HFILE_DEFINED
typedef int16_t HFILE;

#endif // !_HFILE_DEFINED
#ifndef _LPWORD_DEFINED
#define _LPWORD_DEFINED
typedef uint16_t __RPC_FAR *LPWORD;

#endif // !_LPWORD_DEFINED
#ifndef _LPDWORD_DEFINED
#define _LPDWORD_DEFINED
typedef uint32_t __RPC_FAR *LPDWORD;

#endif // !_LPDWORD_DEFINED
typedef char char;

typedef /* [string] */ char __RPC_FAR *char*;

typedef /* [string] */ const char __RPC_FAR *const char *;

#ifndef _WCHAR_DEFINED
#define _WCHAR_DEFINED
typedef wchar_t WCHAR;

typedef WCHAR TCHAR;

#endif // !_WCHAR_DEFINED
typedef /* [string] */ WCHAR __RPC_FAR *const char *;

typedef /* [string] */ TCHAR __RPC_FAR *char*;

typedef /* [string] */ const WCHAR __RPC_FAR *LPCWSTR;

typedef /* [string] */ const TCHAR __RPC_FAR *const char *;

typedef struct  tagPALETTEENTRY {
    uchar peRed;
    uchar peGreen;
    uchar peBlue;
    uchar peFlags;
}   PALETTEENTRY;

typedef struct tagPALETTEENTRY __RPC_FAR *PPALETTEENTRY;

typedef struct tagPALETTEENTRY __RPC_FAR *LPPALETTEENTRY;

#if 0
typedef struct  tagLOGPALETTE {
    uint16_t palVersion;
    uint16_t palNumEntries;
    /* [size_is] */
    PALETTEENTRY palPalEntry[ 1 ];
}   LOGPALETTE;

typedef struct tagLOGPALETTE __RPC_FAR *PLOGPALETTE;

typedef struct tagLOGPALETTE __RPC_FAR *LPLOGPALETTE;

#else
typedef struct tagLOGPALETTE {
    uint16_t        palVersion;
    uint16_t        palNumEntries;
    PALETTEENTRY        palPalEntry[1];
} LOGPALETTE, *PLOGPALETTE, *LPLOGPALETTE;
#endif
#ifndef _COLORREF_DEFINED
#define _COLORREF_DEFINED
typedef uint32_t COLORREF;

#endif // !_COLORREF_DEFINED
#ifndef _LPCOLORREF_DEFINED
#define _LPCOLORREF_DEFINED
typedef uint32_t __RPC_FAR *LPCOLORREF;

#endif // !_LPCOLORREF_DEFINED
typedef Handle __RPC_FAR *LPHANDLE;

typedef struct  _RECTL {
    int32_t left;
    int32_t top;
    int32_t right;
    int32_t bottom;
}   RECTL;

typedef struct _RECTL __RPC_FAR *PRECTL;

typedef struct _RECTL __RPC_FAR *LPRECTL;

#endif  //!WIN32 && !MPPC
#if defined(_WIN32) && !defined(OLE2ANSI)
typedef WCHAR OLECHAR;

typedef /* [string] */ OLECHAR __RPC_FAR *LPOLESTR;

typedef /* [string] */ const OLECHAR __RPC_FAR *LPCOLESTR;

#define OLESTR(str) L##str

#else

typedef char      OLECHAR;
typedef char*     LPOLESTR;
typedef const char *    LPCOLESTR;
#define OLESTR(str) str
#endif
#ifndef _WINDEF_
typedef const RECTL __RPC_FAR *LPCRECTL;

typedef void __RPC_FAR *pvoid;

typedef void __RPC_FAR *pvoid;

typedef float FLOAT;

typedef struct  tagRECT {
    int32_t left;
    int32_t top;
    int32_t right;
    int32_t bottom;
}   RECT;

typedef struct tagRECT __RPC_FAR *PRECT;

typedef struct tagRECT __RPC_FAR *LPRECT;

typedef const RECT __RPC_FAR *LPCRECT;

#endif  //_WINDEF_
typedef unsigned char UCHAR;

typedef short SHORT;

typedef unsigned short USHORT;

typedef double DOUBLE;

#ifndef _DWORDLONG_
typedef MIDL_uhyper DWORDLONG;

typedef DWORDLONG __RPC_FAR *PDWORDLONG;

#endif // !_DWORDLONG_
#ifndef _ULONGLONG_
typedef hyper LONGLONG;

typedef MIDL_uhyper ULONGLONG;

typedef LONGLONG __RPC_FAR *PLONGLONG;

typedef ULONGLONG __RPC_FAR *PULONGLONG;

#endif // _ULONGLONG_
#if 0
typedef struct  _LARGE_INTEGER {
    LONGLONG QuadPart;
}   LARGE_INTEGER;

typedef LARGE_INTEGER __RPC_FAR *PLARGE_INTEGER;

typedef struct  _ULARGE_INTEGER {
    ULONGLONG QuadPart;
}   ULARGE_INTEGER;

#endif // 0
#ifndef _WINBASE_
#ifndef _FILETIME_
#define _FILETIME_
typedef struct  _FILETIME {
    uint32_t dwLowDateTime;
    uint32_t dwHighDateTime;
}   FILETIME;

typedef struct _FILETIME __RPC_FAR *PFILETIME;

typedef struct _FILETIME __RPC_FAR *LPFILETIME;

#endif // !_FILETIME
#ifndef _SYSTEMTIME_
#define _SYSTEMTIME_
typedef struct  _SYSTEMTIME {
    uint16_t wYear;
    uint16_t wMonth;
    uint16_t wDayOfWeek;
    uint16_t wDay;
    uint16_t wHour;
    uint16_t wMinute;
    uint16_t wSecond;
    uint16_t wMilliseconds;
}   SYSTEMTIME;

typedef struct _SYSTEMTIME __RPC_FAR *PSYSTEMTIME;

typedef struct _SYSTEMTIME __RPC_FAR *LPSYSTEMTIME;

#endif // !_SYSTEMTIME
#ifndef _SECURITY_ATTRIBUTES_
#define _SECURITY_ATTRIBUTES_
typedef struct  _SECURITY_ATTRIBUTES {
    uint32_t nLength;
    /* [size_is] */
    pvoid lpSecurityDescriptor;
    Bool bInheritHandle;
}   SECURITY_ATTRIBUTES;

typedef struct _SECURITY_ATTRIBUTES __RPC_FAR *PSECURITY_ATTRIBUTES;

typedef struct _SECURITY_ATTRIBUTES __RPC_FAR *LPSECURITY_ATTRIBUTES;

#endif // !_SECURITY_ATTRIBUTES_
#ifndef SECURITY_DESCRIPTOR_REVISION
typedef USHORT SECURITY_DESCRIPTOR_CONTROL;

typedef USHORT __RPC_FAR *PSECURITY_DESCRIPTOR_CONTROL;

typedef pvoid PSID;

typedef struct  _ACL {
    UCHAR AclRevision;
    UCHAR Sbz1;
    USHORT AclSize;
    USHORT AceCount;
    USHORT Sbz2;
}   ACL;

typedef ACL __RPC_FAR *PACL;

typedef struct  _SECURITY_DESCRIPTOR {
    UCHAR Revision;
    UCHAR Sbz1;
    SECURITY_DESCRIPTOR_CONTROL Control;
    PSID Owner;
    PSID Group;
    PACL Sacl;
    PACL Dacl;
}   SECURITY_DESCRIPTOR;

typedef struct _SECURITY_DESCRIPTOR __RPC_FAR *PISECURITY_DESCRIPTOR;

#endif // !SECURITY_DESCRIPTOR_REVISION
#endif //_WINBASE_
typedef struct  _COAUTHIDENTITY {
    /* [size_is] */
    USHORT __RPC_FAR *User;
    ULONG UserLength;
    /* [size_is] */
    USHORT __RPC_FAR *Domain;
    ULONG DomainLength;
    /* [size_is] */
    USHORT __RPC_FAR *Password;
    ULONG PasswordLength;
    ulong Flags;
}   COAUTHIDENTITY;

typedef struct  _COAUTHINFO {
    uint32_t dwAuthnSvc;
    uint32_t dwAuthzSvc;
    const char * pwszServerPrincName;
    uint32_t dwAuthnLevel;
    uint32_t dwImpersonationLevel;
    COAUTHIDENTITY __RPC_FAR *pAuthIdentityData;
    uint32_t dwCapabilities;
}   COAUTHINFO;

typedef int32_t SCODE;

#ifndef _HRESULT_DEFINED
#define _HRESULT_DEFINED
typedef int32_t HRESULT;

#endif // !_HRESULT_DEFINED
typedef SCODE __RPC_FAR *PSCODE;

#ifndef GUID_DEFINED
#define GUID_DEFINED
typedef struct  _GUID {
    uint32_t Data1;
    uint16_t Data2;
    uint16_t Data3;
    uchar Data4[ 8 ];
}   GUID;

#endif // !GUID_DEFINED
#if !defined( __LPGUID_DEFINED__ )
#define __LPGUID_DEFINED__
typedef GUID __RPC_FAR *LPGUID;

#endif // !__LPGUID_DEFINED__
#ifndef __OBJECTID_DEFINED
#define __OBJECTID_DEFINED
#define _OBJECTID_DEFINED
typedef struct  _OBJECTID {
    GUID Lineage;
    unsigned long Uniquifier;
}   OBJECTID;

#endif // !_OBJECTID_DEFINED
#if !defined( __IID_DEFINED__ )
#define __IID_DEFINED__
typedef GUID IID;

typedef IID __RPC_FAR *LPIID;

#define IID_NULL            GUID_NULL
#define IsEqualIID(riid1, riid2) IsEqualGUID(riid1, riid2)
typedef GUID CLSID;

typedef CLSID __RPC_FAR *LPCLSID;

#define CLSID_NULL          GUID_NULL
#define IsEqualCLSID(rclsid1, rclsid2) IsEqualGUID(rclsid1, rclsid2)
typedef GUID FMTID;

typedef FMTID __RPC_FAR *LPFMTID;

#define FMTID_NULL          GUID_NULL
#define IsEqualFMTID(rfmtid1, rfmtid2) IsEqualGUID(rfmtid1, rfmtid2)
#if 0
typedef GUID __RPC_FAR *REFGUID;

typedef IID __RPC_FAR *REFIID;

typedef CLSID __RPC_FAR *REFCLSID;

typedef FMTID __RPC_FAR *REFFMTID;

#endif // 0
#if defined(__cplusplus)
#ifndef _REFGUID_DEFINED
#define _REFGUID_DEFINED
#define REFGUID             const GUID &
#endif // !_REFGUID_DEFINED
#ifndef _REFIID_DEFINED
#define _REFIID_DEFINED
#define REFIID              const IID &
#endif // !_REFIID_DEFINED
#ifndef _REFCLSID_DEFINED
#define _REFCLSID_DEFINED
#define REFCLSID            const CLSID &
#endif // !_REFCLSID_DEFINED
#ifndef _REFFMTID_DEFINED
#define _REFFMTID_DEFINED
#define REFFMTID            const FMTID &
#endif // !_REFFMTID_DEFINED
#else // !__cplusplus
#ifndef _REFGUID_DEFINED
#define _REFGUID_DEFINED
#define REFGUID             const GUID * const
#endif // !_REFGUID_DEFINED
#ifndef _REFIID_DEFINED
#define _REFIID_DEFINED
#define REFIID              const IID * const
#endif // !_REFIID_DEFINED
#ifndef _REFCLSID_DEFINED
#define _REFCLSID_DEFINED
#define REFCLSID            const CLSID * const
#endif // !_REFCLSID_DEFINED
#ifndef _REFFMTID_DEFINED
#define _REFFMTID_DEFINED
#define REFFMTID            const FMTID * const
#endif // !_REFFMTID_DEFINED
#endif // !__cplusplus
#endif // !__IID_DEFINED__
typedef
enum tagMEMCTX {   MEMCTX_TASK = 1,
                   MEMCTX_SHARED   = 2,
                   MEMCTX_MACSYSTEM    = 3,
                   MEMCTX_UNKNOWN  = -1,
                   MEMCTX_SAME = -2
               }   MEMCTX;

#ifndef _ROTFLAGS_DEFINED
#define _ROTFLAGS_DEFINED
#define ROTFLAGS_REGISTRATIONKEEPSALIVE 0x1
#define ROTFLAGS_ALLOWANYCLIENT 0x2
#endif // !_ROTFLAGS_DEFINED
#ifndef _ROT_COMPARE_MAX_DEFINED
#define _ROT_COMPARE_MAX_DEFINED
#define ROT_COMPARE_MAX 2048
#endif // !_ROT_COMPARE_MAX_DEFINED
typedef
enum tagCLSCTX {   CLSCTX_INPROC_SERVER    = 0x1,
                   CLSCTX_INPROC_HANDLER   = 0x2,
                   CLSCTX_LOCAL_SERVER = 0x4,
                   CLSCTX_INPROC_SERVER16  = 0x8,
                   CLSCTX_REMOTE_SERVER    = 0x10,
                   CLSCTX_INPROC_HANDLER16 = 0x20,
                   CLSCTX_INPROC_SERVERX86 = 0x40,
                   CLSCTX_INPROC_HANDLERX86    = 0x80,
                   CLSCTX_ESERVER_HANDLER  = 0x100
               }   CLSCTX;

typedef
enum tagMSHLFLAGS {   MSHLFLAGS_NORMAL    = 0,
                      MSHLFLAGS_TABLESTRONG   = 1,
                      MSHLFLAGS_TABLEWEAK = 2,
                      MSHLFLAGS_NOPING    = 4
                  }   MSHLFLAGS;

typedef
enum tagMSHCTX {   MSHCTX_LOCAL    = 0,
                   MSHCTX_NOSHAREDMEM  = 1,
                   MSHCTX_DIFFERENTMACHINE = 2,
                   MSHCTX_INPROC   = 3
               }   MSHCTX;

typedef
enum tagDVASPECT {   DVASPECT_CONTENT    = 1,
                     DVASPECT_THUMBNAIL  = 2,
                     DVASPECT_ICON   = 4,
                     DVASPECT_DOCPRINT   = 8
                 }   DVASPECT;

typedef
enum tagSTGC {   STGC_DEFAULT    = 0,
                 STGC_OVERWRITE  = 1,
                 STGC_ONLYIFCURRENT  = 2,
                 STGC_DANGEROUSLYCOMMITMERELYTODISKCACHE = 4,
                 STGC_CONSOLIDATE    = 8
             }   STGC;

typedef
enum tagSTGMOVE {   STGMOVE_MOVE    = 0,
                    STGMOVE_COPY    = 1,
                    STGMOVE_SHALLOWCOPY = 2
                }   STGMOVE;

typedef
enum tagSTATFLAG {   STATFLAG_DEFAULT    = 0,
                     STATFLAG_NONAME = 1,
                     STATFLAG_NOOPEN = 2
                 }   STATFLAG;

typedef /* [context_handle] */ void __RPC_FAR *HCONTEXT;

#ifndef _LCID_DEFINED
#define _LCID_DEFINED
typedef uint32_t LCID;

#endif // !_LCID_DEFINED
typedef struct  _BYTE_BLOB {
    unsigned long clSize;
    /* [size_is] */
    byte abData[ 1 ];
}   BYTE_BLOB;

typedef /* [unique] */ BYTE_BLOB __RPC_FAR *UP_BYTE_BLOB;

typedef struct  _WORD_BLOB {
    unsigned long clSize;
    /* [size_is] */
    unsigned short asData[ 1 ];
}   WORD_BLOB;

typedef /* [unique] */ WORD_BLOB __RPC_FAR *UP_WORD_BLOB;

typedef struct  _DWORD_BLOB {
    unsigned long clSize;
    /* [size_is] */
    unsigned long alData[ 1 ];
}   DWORD_BLOB;

typedef /* [unique] */ DWORD_BLOB __RPC_FAR *UP_DWORD_BLOB;

typedef struct  _FLAGGED_BYTE_BLOB {
    unsigned long fFlags;
    unsigned long clSize;
    /* [size_is] */
    byte abData[ 1 ];
}   FLAGGED_BYTE_BLOB;

typedef /* [unique] */ FLAGGED_BYTE_BLOB __RPC_FAR *UP_FLAGGED_BYTE_BLOB;

typedef struct  _FLAGGED_WORD_BLOB {
    unsigned long fFlags;
    unsigned long clSize;
    /* [size_is] */
    unsigned short asData[ 1 ];
}   FLAGGED_WORD_BLOB;

typedef /* [unique] */ FLAGGED_WORD_BLOB __RPC_FAR *UP_FLAGGED_WORD_BLOB;

typedef struct  _BYTE_SIZEDARR {
    unsigned long clSize;
    /* [size_is] */
    byte __RPC_FAR *pData;
}   BYTE_SIZEDARR;

typedef struct  _SHORT_SIZEDARR {
    unsigned long clSize;
    /* [size_is] */
    unsigned short __RPC_FAR *pData;
}   WORD_SIZEDARR;

typedef struct  _LONG_SIZEDARR {
    unsigned long clSize;
    /* [size_is] */
    unsigned long __RPC_FAR *pData;
}   DWORD_SIZEDARR;

typedef struct  _HYPER_SIZEDARR {
    unsigned long clSize;
    /* [size_is] */
    hyper __RPC_FAR *pData;
}   HYPER_SIZEDARR;

#define WDT_INPROC_CALL ( 0x48746457 )

#define WDT_REMOTE_CALL ( 0x52746457 )

typedef struct  _userCLIPFORMAT {
    long fContext;
    /* [switch_is] */ /* [switch_type] */
    union __MIDL_IWinTypes_0001 {
        /* [case()] */
        uint32_t dwValue;
        /* [case()][string] */
        wchar_t __RPC_FAR *pwszName;
    }   u;
}   userCLIPFORMAT;

typedef /* [unique] */ userCLIPFORMAT __RPC_FAR *wireCLIPFORMAT;

typedef /* [wire_marshal] */ uint16_t CLIPFORMAT;

typedef struct  _GDI_NONREMOTE {
    long fContext;
    /* [switch_is] */ /* [switch_type] */
    union __MIDL_IWinTypes_0002 {
        /* [case()] */
        long hInproc;
        /* [case()] */
        DWORD_BLOB __RPC_FAR *hRemote;
    }   u;
}   GDI_NONREMOTE;

typedef struct  _userHGLOBAL {
    long fContext;
    /* [switch_is] */ /* [switch_type] */
    union __MIDL_IWinTypes_0003 {
        /* [case()] */
        long hInproc;
        /* [case()] */
        FLAGGED_BYTE_BLOB __RPC_FAR *hRemote;
        /* [default] */
        long hGlobal;
    }   u;
}   userHGLOBAL;

typedef /* [unique] */ userHGLOBAL __RPC_FAR *wireHGLOBAL;

typedef struct  _userHMETAFILE {
    long fContext;
    /* [switch_is] */ /* [switch_type] */
    union __MIDL_IWinTypes_0004 {
        /* [case()] */
        long hInproc;
        /* [case()] */
        BYTE_BLOB __RPC_FAR *hRemote;
        /* [default] */
        long hGlobal;
    }   u;
}   userHMETAFILE;

typedef struct  _remoteMETAFILEPICT {
    long mm;
    long xExt;
    long yExt;
    userHMETAFILE __RPC_FAR *hMF;
}   remoteMETAFILEPICT;

typedef struct  _userHMETAFILEPICT {
    long fContext;
    /* [switch_is] */ /* [switch_type] */
    union __MIDL_IWinTypes_0005 {
        /* [case()] */
        long hInproc;
        /* [case()] */
        remoteMETAFILEPICT __RPC_FAR *hRemote;
        /* [default] */
        long hGlobal;
    }   u;
}   userHMETAFILEPICT;

typedef struct  _userHENHMETAFILE {
    long fContext;
    /* [switch_is] */ /* [switch_type] */
    union __MIDL_IWinTypes_0006 {
        /* [case()] */
        long hInproc;
        /* [case()] */
        BYTE_BLOB __RPC_FAR *hRemote;
        /* [default] */
        long hGlobal;
    }   u;
}   userHENHMETAFILE;

typedef struct  _userBITMAP {
    int32_t bmType;
    int32_t bmWidth;
    int32_t bmHeight;
    int32_t bmWidthBytes;
    uint16_t bmPlanes;
    uint16_t bmBitsPixel;
    ulong cbSize;
    /* [size_is] */
    byte pBuffer[ 1 ];
}   userBITMAP;

typedef struct  _userHBITMAP {
    long fContext;
    /* [switch_is] */ /* [switch_type] */
    union __MIDL_IWinTypes_0007 {
        /* [case()] */
        long hInproc;
        /* [case()] */
        userBITMAP __RPC_FAR *hRemote;
        /* [default] */
        long hGlobal;
    }   u;
}   userHBITMAP;

typedef struct  tagrpcLOGPALETTE {
    uint16_t palVersion;
    uint16_t palNumEntries;
    /* [size_is] */
    PALETTEENTRY palPalEntry[ 1 ];
}   rpcLOGPALETTE;

typedef struct  _userHPALETTE {
    long fContext;
    /* [switch_is] */ /* [switch_type] */
    union __MIDL_IWinTypes_0008 {
        /* [case()] */
        long hInproc;
        /* [case()] */
        rpcLOGPALETTE __RPC_FAR *hRemote;
        /* [default] */
        long hGlobal;
    }   u;
}   userHPALETTE;

typedef struct  _RemotableHandle {
    long fContext;
    /* [switch_is] */ /* [switch_type] */
    union __MIDL_IWinTypes_0009 {
        /* [case()] */
        long hInproc;
        /* [case()] */
        long hRemote;
    }   u;
}   RemotableHandle;

typedef /* [unique] */ RemotableHandle __RPC_FAR *wireHWND;

typedef /* [unique] */ RemotableHandle __RPC_FAR *wireHMENU;

typedef /* [unique] */ RemotableHandle __RPC_FAR *wireHACCEL;

typedef /* [unique] */ RemotableHandle __RPC_FAR *wireHBRUSH;

typedef /* [unique] */ RemotableHandle __RPC_FAR *wireHFONT;

typedef /* [unique] */ RemotableHandle __RPC_FAR *wireHDC;

typedef /* [unique] */ RemotableHandle __RPC_FAR *wireHICON;

#if 0
typedef /* [wire_marshal] */ void __RPC_FAR *HWND;

typedef /* [wire_marshal] */ void __RPC_FAR *HMENU;

typedef /* [wire_marshal] */ void __RPC_FAR *HACCEL;

typedef /* [wire_marshal] */ void __RPC_FAR *HBRUSH;

typedef /* [wire_marshal] */ void __RPC_FAR *HFONT;

typedef /* [wire_marshal] */ void __RPC_FAR *HDC;

typedef /* [wire_marshal] */ void __RPC_FAR *HICON;

#ifndef _HCURSOR_DEFINED
#define _HCURSOR_DEFINED
typedef HICON HCURSOR;

#endif // !_HCURSOR_DEFINED
/* tagTEXTMETRICW was copied from wingdi.h for MIDL */
typedef struct  tagTEXTMETRICW {
    int32_t tmHeight;
    int32_t tmAscent;
    int32_t tmDescent;
    int32_t tmInternalLeading;
    int32_t tmExternalLeading;
    int32_t tmAveCharWidth;
    int32_t tmMaxCharWidth;
    int32_t tmWeight;
    int32_t tmOverhang;
    int32_t tmDigitizedAspectX;
    int32_t tmDigitizedAspectY;
    WCHAR tmFirstChar;
    WCHAR tmLastChar;
    WCHAR tmDefaultChar;
    WCHAR tmBreakChar;
    uchar tmItalic;
    uchar tmUnderlined;
    uchar tmStruckOut;
    uchar tmPitchAndFamily;
    uchar tmCharSet;
}   TEXTMETRICW;

#endif //0
#ifndef _WIN32           // The following code is for Win16 only
#ifndef WINAPI          // If not included with 3.1 headers...
typedef struct  tagMSG {
    HWND hwnd;
    uint message;
    WPARAM wParam;
    LPARAM lParam;
    uint32_t time;
    Point16 pt;
}   MSG;

typedef struct tagMSG __RPC_FAR *PMSG;

typedef struct tagMSG __RPC_FAR *NPMSG;

typedef struct tagMSG __RPC_FAR *LPMSG;

#endif // _WIN32
#endif // WINAPI
typedef /* [unique] */ userHBITMAP __RPC_FAR *wireHBITMAP;

typedef /* [unique] */ userHPALETTE __RPC_FAR *wireHPALETTE;

typedef /* [unique] */ userHENHMETAFILE __RPC_FAR *wireHENHMETAFILE;

typedef /* [unique] */ userHMETAFILE __RPC_FAR *wireHMETAFILE;

typedef /* [unique] */ userHMETAFILEPICT __RPC_FAR *wireHMETAFILEPICT;

#if 0
typedef /* [wire_marshal] */ void __RPC_FAR *HGLOBAL;

typedef HGLOBAL HLOCAL;

typedef /* [wire_marshal] */ void __RPC_FAR *HBITMAP;

typedef /* [wire_marshal] */ void __RPC_FAR *HPALETTE;

typedef /* [wire_marshal] */ void __RPC_FAR *HENHMETAFILE;

typedef /* [wire_marshal] */ void __RPC_FAR *HMETAFILE;

#endif //0
typedef /* [wire_marshal] */ void __RPC_FAR *HMETAFILEPICT;



extern RPC_IF_HANDLE IWinTypes_v0_1_c_ifspec;
extern RPC_IF_HANDLE IWinTypes_v0_1_s_ifspec;
#endif /* __IWinTypes_INTERFACE_DEFINED__ */

/****************************************
 * Generated header for interface: __MIDL_itf_wtypes_0001
 * at Thu Sep 11 10:57:03 1997
 * using MIDL 3.03.0110
 ****************************************/
/* [local] */


#if ( _MSC_VER >= 800 )
#pragma warning(disable:4201)
#endif
typedef double DATE;

#ifndef _tagCY_DEFINED
#define _tagCY_DEFINED
#define _CY_DEFINED
#if 0
/* the following isn't the real definition of CY, but it is */
/* what RPC knows how to remote */
typedef struct  tagCY {
    LONGLONG int64;
}   CY;

#else /* 0 */
/* real definition that makes the C++ compiler happy */
typedef union tagCY {
    struct {
#ifdef _MAC
        long      Hi;
        long Lo;
#else
unsigned long Lo;
long      Hi;
#endif
    };
    LONGLONG int64;
} CY;
#endif /* 0 */
#endif /* _tagCY_DEFINED */
typedef CY __RPC_FAR *LPCY;

#if 0 /* _tagDEC_DEFINED */
/* The following isn't the real definition of Decimal type, */
/* but it is what RPC knows how to remote */
typedef struct  tagDEC {
    USHORT wReserved;
    uchar scale;
    uchar sign;
    ulong Hi32;
    ULONGLONG Lo64;
}   DECIMAL;

#else /* _tagDEC_DEFINED */
/* real definition that makes the C++ compiler happy */
typedef struct tagDEC {
    USHORT wReserved;
    union {
        struct {
            uchar scale;
            uchar sign;
        };
        USHORT signscale;
    };
    ulong Hi32;
    union {
        struct {
#ifdef _MAC
            ulong Mid32;
            ulong Lo32;
#else
ulong Lo32;
ulong Mid32;
#endif
        };
        ULONGLONG Lo64;
    };
} DECIMAL;
#define DECIMAL_NEG ((uchar)0x80)
#define DECIMAL_SETZERO(dec) \
        {(dec).Lo64 = 0; (dec).Hi32 = 0; (dec).signscale = 0;}
#endif /* _tagDEC_DEFINED */
typedef DECIMAL __RPC_FAR *LPDECIMAL;

#if ( _MSC_VER >= 800 )
#pragma warning(default:4201)
#endif
typedef /* [unique] */ FLAGGED_WORD_BLOB __RPC_FAR *wireBSTR;

typedef /* [wire_marshal] */ OLECHAR __RPC_FAR *BSTR;

typedef BSTR __RPC_FAR *LPBSTR;

/* 0 == FALSE, -1 == TRUE */
typedef short VARIANT_BOOL;

#if !__STDC__ && (_MSC_VER <= 1000)
/* For backward compatibility */
typedef VARIANT_BOOL _VARIANT_BOOL;

#else
/* ANSI C/C++ reserve bool as keyword */
#define _VARIANT_BOOL    /##/
#endif
typedef boolean BOOLEAN;

/* The BSTRBLOB structure is used by some implementations */
/* of the IPropertyStorage interface when marshaling BSTRs */
/* on systems which don't support BSTR marshaling. */
#ifndef _tagBSTRBLOB_DEFINED
#define _tagBSTRBLOB_DEFINED
typedef struct  tagBSTRBLOB {
    ulong cbSize;
    /* [size_is] */
    uchar __RPC_FAR *pData;
}   BSTRBLOB;

typedef struct tagBSTRBLOB __RPC_FAR *LPBSTRBLOB;

#endif
#define VARIANT_TRUE ((VARIANT_BOOL)0xffff)
#define VARIANT_FALSE ((VARIANT_BOOL)0)
#ifndef _tagBLOB_DEFINED
#define _tagBLOB_DEFINED
#define _BLOB_DEFINED
#define _LPBLOB_DEFINED
typedef struct  tagBLOB {
    ulong cbSize;
    /* [size_is] */
    uchar __RPC_FAR *pBlobData;
}   BLOB;

typedef struct tagBLOB __RPC_FAR *LPBLOB;

#endif
typedef struct  tagCLIPDATA {
    ulong cbSize;
    long ulClipFmt;
    /* [size_is] */
    uchar __RPC_FAR *pClipData;
}   CLIPDATA;

// Macro to calculate the size of the above pClipData
#define CBPCLIPDATA(clipdata)    ( (clipdata).cbSize - sizeof((clipdata).ulClipFmt) )
typedef unsigned short VARTYPE;

/*
 * VARENUM usage key,
 *
 * * [V] - may appear in a VARIANT
 * * [T] - may appear in a TYPEDESC
 * * [P] - may appear in an OLE property set
 * * [S] - may appear in a Safe Array
 *
 *
 *  VT_EMPTY            [V]   [P]     nothing
 *  VT_NULL             [V]   [P]     SQL style Null
 *  VT_I2               [V][T][P][S]  2 byte signed int
 *  VT_I4               [V][T][P][S]  4 byte signed int
 *  VT_R4               [V][T][P][S]  4 byte real
 *  VT_R8               [V][T][P][S]  8 byte real
 *  VT_CY               [V][T][P][S]  currency
 *  VT_DATE             [V][T][P][S]  date
 *  VT_BSTR             [V][T][P][S]  OLE Automation string
 *  VT_DISPATCH         [V][T][P][S]  IDispatch *
 *  VT_ERROR            [V][T][P][S]  SCODE
 *  VT_BOOL             [V][T][P][S]  True=-1, False=0
 *  VT_VARIANT          [V][T][P][S]  VARIANT *
 *  VT_UNKNOWN          [V][T]   [S]  IUnknown *
 *  VT_DECIMAL          [V][T]   [S]  16 byte fixed point
 *  VT_RECORD           [V]   [P][S]  user defined type
 *  VT_I1               [V][T][P][s]  signed char
 *  VT_UI1              [V][T][P][S]  unsigned char
 *  VT_UI2              [V][T][P][S]  unsigned short
 *  VT_UI4              [V][T][P][S]  unsigned short
 *  VT_I8                  [T][P]     signed 64-bit int
 *  VT_UI8                 [T][P]     unsigned 64-bit int
 *  VT_INT              [V][T][P][S]  signed machine int
 *  VT_UINT             [V][T]   [S]  unsigned machine int
 *  VT_VOID                [T]        C style void
 *  VT_HRESULT             [T]        Standard return type
 *  VT_PTR                 [T]        pointer type
 *  VT_SAFEARRAY           [T]        (use VT_ARRAY in VARIANT)
 *  VT_CARRAY              [T]        C style array
 *  VT_USERDEFINED         [T]        user defined type
 *  VT_LPSTR               [T][P]     null terminated string
 *  VT_LPWSTR              [T][P]     wide null terminated string
 *  VT_FILETIME               [P]     FILETIME
 *  VT_BLOB                   [P]     Length prefixed bytes
 *  VT_STREAM                 [P]     Name of the stream follows
 *  VT_STORAGE                [P]     Name of the storage follows
 *  VT_STREAMED_OBJECT        [P]     Stream contains an object
 *  VT_STORED_OBJECT          [P]     Storage contains an object
 *  VT_BLOB_OBJECT            [P]     Blob contains an object
 *  VT_CF                     [P]     Clipboard format
 *  VT_CLSID                  [P]     A Class ID
 *  VT_VECTOR                 [P]     simple counted array
 *  VT_ARRAY            [V]           SAFEARRAY*
 *  VT_BYREF            [V]           void* for local use
 *  VT_BSTR_BLOB                      Reserved for system use
 */

enum VARENUM {   VT_EMPTY    = 0,
                 VT_NULL = 1,
                 VT_I2   = 2,
                 VT_I4   = 3,
                 VT_R4   = 4,
                 VT_R8   = 5,
                 VT_CY   = 6,
                 VT_DATE = 7,
                 VT_BSTR = 8,
                 VT_DISPATCH = 9,
                 VT_ERROR    = 10,
                 VT_BOOL = 11,
                 VT_VARIANT  = 12,
                 VT_UNKNOWN  = 13,
                 VT_DECIMAL  = 14,
                 VT_I1   = 16,
                 VT_UI1  = 17,
                 VT_UI2  = 18,
                 VT_UI4  = 19,
                 VT_I8   = 20,
                 VT_UI8  = 21,
                 VT_INT  = 22,
                 VT_UINT = 23,
                 VT_VOID = 24,
                 VT_HRESULT  = 25,
                 VT_PTR  = 26,
                 VT_SAFEARRAY    = 27,
                 VT_CARRAY   = 28,
                 VT_USERDEFINED  = 29,
                 VT_LPSTR    = 30,
                 VT_LPWSTR   = 31,
                 VT_RECORD   = 36,
                 VT_FILETIME = 64,
                 VT_BLOB = 65,
                 VT_STREAM   = 66,
                 VT_STORAGE  = 67,
                 VT_STREAMED_OBJECT  = 68,
                 VT_STORED_OBJECT    = 69,
                 VT_BLOB_OBJECT  = 70,
                 VT_CF   = 71,
                 VT_CLSID    = 72,
                 VT_BSTR_BLOB    = 0xfff,
                 VT_VECTOR   = 0x1000,
                 VT_ARRAY    = 0x2000,
                 VT_BYREF    = 0x4000,
                 VT_RESERVED = 0x8000,
                 VT_ILLEGAL  = 0xffff,
                 VT_ILLEGALMASKED    = 0xfff,
                 VT_TYPEMASK = 0xfff
             };
typedef ulong PROPID;

#ifndef SID_IDENTIFIER_AUTHORITY_DEFINED
#define SID_IDENTIFIER_AUTHORITY_DEFINED
typedef struct  _SID_IDENTIFIER_AUTHORITY {
    UCHAR Value[ 6 ];
}   SID_IDENTIFIER_AUTHORITY;

typedef struct _SID_IDENTIFIER_AUTHORITY __RPC_FAR *PSID_IDENTIFIER_AUTHORITY;

#endif
#ifndef SID_DEFINED
#define SID_DEFINED
typedef struct  _SID {
    UCHAR Revision;
    UCHAR SubAuthorityCount;
    SID_IDENTIFIER_AUTHORITY IdentifierAuthority;
    /* [size_is] */
    ULONG SubAuthority[ 1 ];
}   SID;

typedef struct _SID __RPC_FAR *PISID;

#endif
typedef GUID APPID;

typedef struct  tagCSPLATFORM {
    uint32_t dwPlatformId;
    uint32_t dwVersionHi;
    uint32_t dwVersionLo;
    uint32_t dwProcessorArch;
}   CSPLATFORM;

typedef struct  tagQUERYCONTEXT {
    uint32_t dwContext;
    CSPLATFORM Platform;
    LCID Locale;
    uint32_t dwVersionHi;
    uint32_t dwVersionLo;
}   QUERYCONTEXT;

typedef /* [v1_enum] */
enum tagTYSPEC {   TYSPEC_CLSID    = 0,
                   TYSPEC_FILEEXT  = TYSPEC_CLSID + 1,
                   TYSPEC_MIMETYPE = TYSPEC_FILEEXT + 1,
                   TYSPEC_PROGID   = TYSPEC_MIMETYPE + 1,
                   TYSPEC_IID  = TYSPEC_PROGID + 1,
                   TYSPEC_TYPELIB  = TYSPEC_IID + 1,
                   TYSPEC_FILENAME = TYSPEC_TYPELIB + 1,
                   TYSPEC_JAVACLASS    = TYSPEC_FILENAME + 1,
                   TYSPEC_PACKAGENAME  = TYSPEC_JAVACLASS + 1
               }   TYSPEC;

typedef /* [public] */ struct  __MIDL___MIDL_itf_wtypes_0001_0001 {
    uint32_t tyspec;
    /* [switch_is] */ /* [switch_type] */
    union __MIDL___MIDL_itf_wtypes_0001_0003 {
        /* [case()] */
        CLSID clsid;
        /* [case()] */
        IID iid;
        /* [case()] */
        GUID typelibID;
        /* [case()] */
        LPOLESTR pFileExt;
        /* [case()] */
        LPOLESTR pMimeType;
        /* [case()] */
        LPOLESTR pProgId;
        /* [case()] */
        LPOLESTR pFileName;
        /* [case()] */
        LPOLESTR pJavaClassName;
        /* [case()] */
        LPOLESTR pPackageName;
    }   tagged_union;
}   uCLSSPEC;

typedef struct  _PUBLISHEDAPPINFO {
    LPOLESTR pwszFileExtension;
    LPOLESTR pwszDisplayName;
    LPOLESTR pwszPackagePath;
}   PUBLISHEDAPPINFO;

typedef /* [v1_enum] */
enum tagAPPINFOTYPE {   APPINFO_PUBLISHED   = 1,
                        APPINFO_SHELLNEW    = APPINFO_PUBLISHED + 1,
                        APPINFO_INSERTABLE  = APPINFO_SHELLNEW + 1
                    }   APPINFOTYPE;

#define ACTFLG_RunLocally   ( 1 )

#define ACTFLG_RunOnce  ( 2 )

#define ACTFLG_SystemWide   ( 4 )

#define ACTFLG_Published    ( 8 )

#define ACTFLG_Assigned ( 16 )

#define ACTFLG_UserInstall  ( 32 )

typedef /* [v1_enum] */
enum _CLASSPATHTYPE {   ExeNamePath = 0,
                        DllNamePath = ExeNamePath + 1,
                        TlbNamePath = DllNamePath + 1,
                        CabFilePath = TlbNamePath + 1,
                        InfFilePath = CabFilePath + 1,
                        DrwFilePath = InfFilePath + 1,
                        SetupNamePath   = DrwFilePath + 1
                    }   CLASSPATHTYPE;

typedef struct  tagAPPDETAIL {
    GUID AppID;
    uint32_t cClasses;
    /* [size_is] */
    CLSID __RPC_FAR *prgClsIdList;
    uint32_t cTypeLibIds;
    /* [size_is] */
    CLSID __RPC_FAR *prgTypeLibIdList;
    uint32_t cServers;
    /* [size_is] */
    LPOLESTR __RPC_FAR *prgServerNames;
}   APPDETAIL;

typedef struct  tagPACKAGEDETAIL {
    CLASSPATHTYPE PathType;
    LPOLESTR pszPath;
    LPOLESTR pszIconPath;
    LPOLESTR pszSetupCommand;
    uint32_t dwActFlags;
    LPOLESTR pszVendor;
    LPOLESTR pszPackageName;
    LPOLESTR pszProductName;
    uint32_t dwContext;
    CSPLATFORM Platform;
    LCID Locale;
    uint32_t dwVersionHi;
    uint32_t dwVersionLo;
    ULONGLONG Usn;
    uint32_t cApps;
    /* [size_is] */
    APPDETAIL __RPC_FAR *pAppDetail;
}   PACKAGEDETAIL;

typedef struct  tagPACKAGEINFO {
    LPOLESTR pszClassIconPath;
    CLSID __RPC_FAR *pTreatAsClsid;
    uint32_t cPackages;
    /* [size_is] */
    PACKAGEDETAIL __RPC_FAR *pPackageDetail;
}   PACKAGEINFO;



extern RPC_IF_HANDLE __MIDL_itf_wtypes_0001_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_wtypes_0001_v0_0_s_ifspec;

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#endif