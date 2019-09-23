// Pdf2Png.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "MuPDFConvert.h"
#include "afxwin.h"
#include <string>
using namespace std;
#include "pdf2png.h"

#ifndef _WIN64

void PDF2PNG(const CString &src, const CString &des)
{
	int pos = des.ReverseFind(_T('.'));
	CString output(des);
	if(pos == -1) output += ".png";
	CMuPDFConvert pdfConvert;
	char coutput[255] = {0};
	TcharToChar(output, coutput);
	int num = 0;
	pdfConvert.Pdf2Png(src, coutput, num);
}
// 
// int main(int argc, char* argv[])
// {
// 	int nNum = 0; string strTemp;
// 	//AfxMessageBox(_T("nishishui"));
// 	CMuPDFConvert pdfConvert;
// 	string suffix;
// 	if(argc == 3)
// 	{
// 		strTemp.append(argv[2]); 
// 		int pos = strTemp.find_last_of('.');
// 		strTemp = strTemp.substr(0, pos) + ".png";
// 	}
// 	else if(argc == 4)
// 		strTemp.append(argv[3]);
// 	else return 0;
// 	int nPos = strTemp.find_last_of('.'); 
// 	if(nPos == -1)
// 		strTemp.append(".png");
// 	else
// 	{
// 		suffix = strTemp.substr(nPos);
// 		strTemp = strTemp.substr(0, nPos) + ".png";
// 	}
// 	TCHAR param[255] = {0};
// 	CharToTchar(argv[2], param);
// 	pdfConvert.Pdf2Png(param, strTemp.c_str(), nNum);
// 	if(strcmp(argv[1], "1") == 0)
// 		DeleteFileA(argv[2]);
// 	if(suffix.compare(".png") == 0) return 1;
// 	string ret = strTemp.substr(0, nPos) + suffix;
// 	CopyFileA(strTemp.c_str(), ret.c_str(), FALSE);	
// 	DeleteFileA(strTemp.c_str());
// 	return 1; 
// }
// 
#endif