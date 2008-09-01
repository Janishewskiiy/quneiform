/*
Copyright (c) 2008, Jussi Pakkanen

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

#ifndef WINFUNCS_H_
#define WINFUNCS_H_

#if WIN32

#include<windows.h>

#else

int LoadString(HINSTANCE hInstance, UINT uID, LPTSTR lpBuffer, int nBufferMax);

int CreateDirectory(const char *dir, void *dummy);
DWORD GetTempPath(DWORD nBufferLength, LPTSTR lpBuffer);
int RemoveDirectory(const char *d);
void* GlobalAlloc(UINT uFlags, int dwBytes);
HGLOBAL GlobalFree(void *f);
void* GlobalReAlloc(void* hMem, int dwBytes, UINT uFlags);
int GetTempFileName(LPCTSTR lpPathName, LPCTSTR lpPrefixString,
                    UINT uUnique, LPTSTR lpTempFileName);
int GetLastError();

DWORD GetModuleFileName(HMODULE hModule, LPTSTR lpFilename, DWORD nSize);
BOOL CloseHandle(HANDLE hObject);
HANDLE CreateFile(LPCTSTR lpFileName, DWORD dwDesiredAccess,
DWORD dwShareMode, void* lpSecurityAttributes,
DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);

HWND FindWindow(LPCTSTR lpClassName, LPCTSTR lpWindowName);
UINT RegisterWindowMessage(LPCTSTR lpString);

int _findclose(long handle);
long _findfirst(const char *filespec, struct _finddata_t *fileinfo);
int _findnext(long handle, struct _finddata_t *fileinfo);
long _tell(int handle);

BOOL GetComputerName(LPTSTR lpBuffer, long unsigned int *lpnSize);

LONG RegOpenKeyEx(HKEY hKey, LPCTSTR lpSubKey, DWORD ulOptions,
  REGSAM samDesired, PHKEY phkResult);
LONG RegQueryValueEx(HKEY hKey, LPCTSTR lpValueName,
LPDWORD lpReserved, LPDWORD lpType, LPBYTE lpData,
LPDWORD lpcbData);

BOOL GetClientRect(HWND hWnd, LPRECT lpRect);
BOOL WritePrivateProfileString(LPCTSTR lpAppName,
        LPCTSTR lpKeyName, LPCTSTR lpString, LPCTSTR lpFileName);
DWORD GetPrivateProfileString(LPCTSTR lpAppName, LPCTSTR lpKeyName,
LPCTSTR lpDefault, LPTSTR lpReturnedString, DWORD nSize, LPCTSTR lpFileName);
UINT GetPrivateProfileInt(LPCTSTR lpAppName,
  LPCTSTR lpKeyName, INT nDefault, LPCTSTR lpFileName);

int WideCharToMultiByte(UINT CodePage, DWORD dwFlags,LPCWSTR lpWideCharStr,
  int cchWideChar, LPSTR lpMultiByteStr, int cbMultiByte,
  LPCSTR lpDefaultChar, LPBOOL lpUsedDefaultChar);

BOOL ShowWindow(HWND hWnd, int nCmdShow);

long _filelength(int fd);
long _msize(void *memblock);
int _access(const char *filename, int mode);
BOOL SetWindowText(HWND hWnd,LPCTSTR lpString);
int ReleaseDC(HWND hWnd, HDC hDC);
BOOL IsIconic(HWND hWnd);
HDC GetDC(HWND hWnd);
BOOL EndPaint(HWND hWnd, ...);
HDC BeginPaint(HWND hwnd,...);
LRESULT SendMessage(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
void strlwr(char *foo);
HWND CreateWindow(
    LPCTSTR lpClassName,
    LPCTSTR lpWindowName,
    DWORD dwStyle,
    int x,
    int y,
    int nWidth,
    int nHeight,
    HWND hWndParent,
    HMENU hMenu,
    HINSTANCE hInstance,
    LPVOID lpParam
);

HGDIOBJ SelectObject(HDC hdc, HGDIOBJ hgdiobj);
LPTSTR lstrcat(LPTSTR lpString1, LPTSTR lpString2);
int lstrlen(LPCTSTR lpString);
int lstrcmp(LPCTSTR lpString1, LPCTSTR lpString2);
LPTSTR lstrcpy(LPTSTR lpString1, LPTSTR lpString2);
int wsprintf(LPTSTR lpOut, LPCTSTR lpFmt, ...);
int lstrcmpi(LPCTSTR lpString1, LPCTSTR lpString2);

BOOL DeleteObject(HGDIOBJ hObject);

#define GetGValue(rgb) ((BYTE) (((WORD) (rgb)) >> 8))
#define GetBValue(rgb) ((BYTE) ((rgb) >> 16))
#define GetRValue(rgb) ((BYTE) (rgb))

HWND GetFocus();
int MessageBox(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType);

int WINAPI GlobalSize(HGLOBAL hMem);
LPVOID GlobalLock(HGLOBAL hMem);
BOOL GlobalUnlock(HGLOBAL hMem);
BOOL IsBadWritePtr(LPVOID lp, int ucb);
void OutputDebugString(LPCTSTR lpOutputString);
BOOL SetRect(LPRECT lprc, int xLeft, int yTop,
int xRight, int yBottom);
BOOL PtInRect(const RECT *lprc, POINT pt);
BOOL IntersectRect(LPRECT lprcDst, const RECT *lprcSrc1, const RECT *lprcSrc2);
BOOL UnionRect(LPRECT lprcDst, const RECT *lprcSrc1,const RECT *lprcSrc2);

HWND GetActiveWindow();
HFONT CreateFont(int nHeight, int nWidth, int nEscapement,
int nOrientation, int fnWeight, DWORD fdwItalic, DWORD fdwUnderline,
DWORD fdwStrikeOut, DWORD fdwCharSet, DWORD fdwOutputPrecision,
DWORD fdwClipPrecision, DWORD fdwQuality, DWORD fdwPitchAndFamily,
LPCTSTR lpszFace);
BOOL GetTextExtentPoint32(HDC hdc, LPCTSTR lpString, int c, LPSIZE lpSize);
BOOL EnumWindows(WNDENUMPROC lpEnumFunc, LPARAM lParam);
int GetWindowText(HWND hWnd, LPTSTR lpString, int nMaxCount);

HMODULE LoadLibrary(LPCTSTR lpFileName);
BOOL FreeLibrary(HMODULE hModule);
void* GetProcAddress(HMODULE hModule, LPCSTR lpProcName);

HGDIOBJ GetStockObject(int fnObject);
BOOL IsWindowVisible(HWND hWnd);
LRESULT DefWindowProc(HWND hWnd, UINT Msg, WPARAM wParam,
    LPARAM lParam);
LONG GetWindowLong(HWND hWnd, int nIndex);
BOOL RegisterClass(const WNDCLASS *lpWndClass);
HMODULE GetModuleHandle(LPCTSTR lpModuleName);
HICON LoadIcon(HINSTANCE hInstance, LPCTSTR lpIconName);

long filelength(int fd);
int LoadCursor(HINSTANCE hInstance, LPCTSTR lpCursorName);
BOOL Rectangle(HDC hdc,
  int nLeftRect, int nTopRect, int nRightRect, int nBottomRect);
char* _strupr(char*s);

/* These are not windows compat functions, but general helper functions.
 * I had no other place to put them.
 */

int open_data_file(const char *basename, int mode);
int data_file_exists(const char *basename);

void split_path(const char *fname,
        char *file_path,
        char *basename,
        char *ext);

void make_path(char *opath,
        const char *dir,
        const char *basename,
        const char *ext);

void winpath_to_internal(char *p);

unsigned int curr_dir(unsigned int bsize, char* buf);

#endif /* not WIN32 */

#endif
