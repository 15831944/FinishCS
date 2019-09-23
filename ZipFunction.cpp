#include "stdafx.h"
#include "ZipFunction.h"
#include <io.h>

namespace ZipUtils
{
	int fileCount = 0;
	ZRESULT ExtractZipToDir(LPCTSTR lpszZipFullName, CStringArray& szFilePathArr, LPCTSTR lpszUnZipPath)
	{
		TCHAR buffer[MAX_PATH] = {0};
		CString strUnZipPath = lpszUnZipPath;
		DWORD zResult = ZR_OK;

		if(!strUnZipPath.IsEmpty())
		{
			// 如果文件路径不存在先创建,存在不做任何修改
			SHCreateDirectoryEx(NULL, lpszUnZipPath, NULL);
		}
		else
		{
			GetCurrentDirectory(MAX_PATH, (LPTSTR)&buffer);
			strUnZipPath = buffer;
			SHCreateDirectoryEx(NULL, strUnZipPath, NULL);
		}

		HZIP hz = OpenZip(lpszZipFullName, 0);
		ZIPENTRY ze;

		GetZipItem(hz, -1, &ze);
		int numitems = ze.index;

		for(int zi = 0; zi < numitems; zi++)
		{
			ZIPENTRY ze;
			GetZipItem(hz, zi, &ze);
			zResult = UnzipItem(hz, zi, (CString)strUnZipPath + _T("\\") + ze.name);
			szFilePathArr.Add(ze.name);
			if(zResult != ZR_OK)
			{
#ifndef _UNICODE 
				// 判断文件是否存在
				if(_access(szFilePathArr[zi], 0))
				{
					// 文件不存在的时候
					return zResult;
				}
#else
				if(_access((char *)(LPTSTR)(LPCTSTR)szFilePathArr[zi], 0))
				{
					// 文件不存在的时候
					return zResult;
				}
#endif
			}
		}

		CloseZip(hz);
		return zResult;
	}

	ZRESULT DirToZip(LPCTSTR lpszSrcPath, LPCTSTR lpszZipName, HZIP& hz, LPCTSTR lpszDestPath, CString zipDir)
	{
		fileCount++;
		DWORD zResult = ZR_OK;
		TCHAR buffer[MAX_PATH] = {0};

		CString strDestPath = lpszDestPath;

		if(fileCount == 1)
		{
			// 这里边的只执行一次
			if(!strDestPath.IsEmpty())
			{
				// 如果解压路径文件夹不存在先创建,存在 不做任何修改
				SHCreateDirectoryEx(NULL, lpszDestPath, NULL);
			}
			else
			{
				GetCurrentDirectory(MAX_PATH, (LPTSTR)&buffer);
				strDestPath = buffer;
				SHCreateDirectoryEx(NULL, strDestPath, NULL);
			}
			hz = CreateZip((CString)strDestPath + _T("\\") + (CString)lpszZipName, 0);
		}

		HANDLE file;
		WIN32_FIND_DATA fileData;
		file = FindFirstFile((CString)lpszSrcPath + _T("\\*.*"), &fileData);
		FindNextFile(file, &fileData);
		while(FindNextFile(file, &fileData))
		{
			// 如果是一个文件目录
			if(fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if(zipDir.IsEmpty())
				{
					ZipAddFolder(hz, fileData.cFileName);
				}
				else
				{
					ZipAddFolder(hz, zipDir + _T("\\") + fileData.cFileName);
				}
				// 存在子文件夹 递归调用
				DirToZip((CString)lpszSrcPath + _T("\\") + fileData.cFileName, lpszZipName, hz, lpszDestPath, zipDir + _T("\\") + fileData.cFileName);
			}
			else
			{
				CString strTempPath;
				strTempPath.Format(_T("%s\\%s"), (CString)lpszSrcPath, fileData.cFileName);
				if(zipDir.IsEmpty())
				{
					ZipAdd(hz, fileData.cFileName, strTempPath);
				}
				else
				{
					ZipAdd(hz, zipDir + _T("\\") + fileData.cFileName, strTempPath);
				}
				if(zResult != ZR_OK)
				{
					return zResult;
				}
				DeleteFile(strTempPath);
			}
		}
		return zResult;
	}

	ZRESULT CompressDirToZip(LPCTSTR lpszSrcPath, LPCTSTR lpszZipName, LPCTSTR lpszDestPath)
	{
		HZIP hz; fileCount = 0;
		DWORD zResult = ZR_OK;
		zResult = DirToZip(lpszSrcPath, lpszZipName, hz, lpszDestPath, _T(""));
		if(zResult == ZR_OK)
		{
			CloseZip(hz);
		}
		return zResult;
	}
}