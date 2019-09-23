#include "stdafx.h"
#include "LichengExcel.h"

//把实数转换为里程形式
extern void ConvertDoubleToKplus(double dRRR, TCHAR *szStr, int nWidth, TCHAR *szLCZMC, bool bMid = false);

//bool getcomments(const tstring &str, const TCHAR *delim, vector<tstring> &ret)
//{
//    ret.clear();
//    size_t front = 0;
//    size_t next = 0;
//    tstring tmp = _T("");
//    for (next = str.find(delim); next != tstring::npos;
//         front = next + 1,
//         next = str.find(delim, front))
//    {
//        tmp = str.substr(front, (next - front));
//        ret.push_back(tmp);    
//		tmp = _T("");
//    }
//    tmp = str.substr(front);
//    ret.push_back(tmp);
//    return true;
//}

void VerFace::setNumPerPage(int nNum) { m_nNumPerPage = nNum; }
void VerFace::setDecimal(int nDec){	m_nDecimal = nDec; }

int VerFace::getDecimal(){	return m_nDecimal;}
int VerFace::getNumPerPage(){ return m_nNumPerPage; }

BOOL VerFace::xlsOut(
        const TCHAR *title,
        const CStringArray &pntNum,
        const AcGePoint2dArray &pntArr,
        const AcGeDoubleArray &heightArr,
		const CStringArray &remarks
        )
{
	COperateExcel operExcel;
	msexcel::_Application excelApp;
	msexcel::Workbooks celBooks;
	msexcel::_Workbook celBook;
	msexcel::Worksheets celSheets;
	msexcel::_Worksheet celSheet;
	msexcel::Range celRange;
	operExcel.Excel_CreateApplication(excelApp, celBooks);
	operExcel.Excel_AddWorkbook(celBooks, celBook);
	operExcel.Excel_GetWorksheets(celBook, celSheets);
	operExcel.Excel_GetWorksheet(celSheets, celSheet, 1);
	operExcel.Excel_GetRange(celSheet, celRange);
	// set column
	operExcel.Excel_SetColWidth(celRange, 1, 14.75);
	operExcel.Excel_SetColWidth(celRange, 2, 13);
	operExcel.Excel_SetColWidth(celRange, 3, 13);
	operExcel.Excel_SetColWidth(celRange, 4, 13);
	operExcel.Excel_SetColWidth(celRange, 5, 27.75);

	operExcel.Excel_MergeRange(1, 1, 1, 5, celRange);
	operExcel.Excel_SetFont(celRange, 1, 1, 1, 1, m_nTitleSize, m_csTitleName, m_bTitleBold);
	operExcel.Excel_SetRangeContent(1, 1, title, celRange); // set title
	// set content
	int nStartRow = 3;
	operExcel.Excel_SetRangeContent(nStartRow, 1, _T("点号"), celRange);
	operExcel.Excel_SetRangeContent(nStartRow, 2, _T("X(m)"), celRange);
	operExcel.Excel_SetRangeContent(nStartRow, 3, _T("Y(m)"), celRange);
	operExcel.Excel_SetRangeContent(nStartRow, 4, _T("H(m)"), celRange);
	operExcel.Excel_SetRangeContent(nStartRow, 5, _T("备注"), celRange);
	operExcel.Excel_SetRangeNormalBorder(nStartRow, 1, pntNum.GetSize() + nStartRow, 5, celRange);
	operExcel.Excel_SetDec(celRange, nStartRow, 1, pntNum.GetSize() + nStartRow, 5, "@");
	operExcel.Excel_SetFont(celRange, nStartRow, 1, pntNum.GetSize() + nStartRow, 5, m_nContSize, m_csContName, m_bContBold);
	++nStartRow;
	for (int m = 0; m < pntNum.GetSize(); ++m)
	{
		operExcel.Excel_SetRangeContent(nStartRow, 1, pntNum.GetAt(m), celRange);
		if (pntArr.length() > m){
			operExcel.Excel_SetRangeContent(nStartRow, 2, FormatDouble(pntArr.at(m).y, m_nDecimal), celRange);
			operExcel.Excel_SetRangeContent(nStartRow, 3, FormatDouble(pntArr.at(m).x, m_nDecimal), celRange);
		}
		if (heightArr.length() > m)
			operExcel.Excel_SetRangeContent(nStartRow, 4, FormatDouble(heightArr.at(m), m_nDecimal), celRange);
		if (remarks.GetSize() > m)
			operExcel.Excel_SetRangeContent(nStartRow, 5, remarks.GetAt(m), celRange);
		++nStartRow;
	}
	return TRUE;
}

BOOL VerFace::readFileOut(const TCHAR *fname, vector<Picket> &pckArr, int &left, int &right)
{
	tifstream in(fname);
	in.imbue(std::locale(""));
	if ( !in ) return FALSE;
	tstring str;
	Picket pick;
	int nTimes = 0, maxLeft = 0, maxRight = 0;
	left = 0; right = 0;
	int nIdx = 0;
	while ( getline(in, str) )
	{
		if (str.empty()) continue;
		if (nIdx == 0 && str.substr(0, 6) != _T("BEGIN,"))
			return FALSE;
		if (str.substr(0, 6) == _T("BEGIN,")){
			if (nTimes != 0) {
				pick.m_left = maxLeft;
				pick.m_right = maxRight;
				pckArr.push_back(pick);
				pick.m_num = _T("");
				pick.m_items.clear();
				if (maxLeft > left) left = maxLeft;
				if (maxRight > right) right = maxRight;
				maxLeft = 0; maxRight = 0;
			}
			vector<tstring> ret;
			getcomments(str.substr(6), _T(":"), ret);
			if ( !ret.empty() ){
				pick.m_num = ret.at(0);
			}
			++nTimes;
		}
		else
		{
			vector<tstring> ret;
			getcomments(str, _T(","), ret);
			if (ret.size() > 1)
			{
				double dDist = _tstof(ret.at(0).c_str());
				double dHeight = _tstof(ret.at(1).c_str());
				(dDist < 0) ? (++maxLeft) : (++maxRight);
				pairitem item( dDist, dHeight );
				pick.m_items.push_back(item);
			}
		}
		++nIdx;
	}
	if (nTimes != 0)
	{
		pick.m_left = maxLeft;
		pick.m_right = maxRight;
		pckArr.push_back(pick);
		pick.m_num = _T("");
		pick.m_items.clear();
		if (maxLeft > left) left = maxLeft;
		if (maxRight > right) right = maxRight;
		maxLeft = 0; maxRight = 0;
	}
	in.close();
	return TRUE;
}


BOOL VerFace::xlsOut(const TCHAR *fname, const TCHAR *title)
{
	vector<Picket> pickArr;
	const LONG lxNull = -4142L;
	const LONG lxContinous = 1L;
	int nLeft = 0, nRight = 0;
	if ( !readFileOut(fname, pickArr, nLeft, nRight) ) return FALSE;
	if ( pickArr.empty() ) return FALSE;
	COperateExcel operExcel;
	msexcel::_Application excelApp;
	msexcel::Workbooks celBooks;
	msexcel::_Workbook celBook;
	msexcel::Worksheets celSheets;
	msexcel::_Worksheet celSheet;
	msexcel::Range celRange;
	operExcel.Excel_CreateApplication(excelApp, celBooks);
	operExcel.Excel_AddWorkbook(celBooks, celBook);
	operExcel.Excel_GetWorksheets(celBook, celSheets);
	operExcel.Excel_GetWorksheet(celSheets, celSheet, 1);
	operExcel.Excel_GetRange(celSheet, celRange);
	operExcel.Excel_SetColWidth(celRange, 1, 0xff, 9.0);
	int leftCol = (nLeft<<1) + 1, rightCol = (nRight<<1);
	int i = 0, m = 0;
	for (i = 1; i < leftCol+ rightCol +1; i += 2)  operExcel.Excel_SetColWidth(celRange, i, (float)0.69);

	int nStartRow = 1;
	int nHowManyPages = pickArr.size() / m_nNumPerPage + 1;
	if ( nHowManyPages > 1 && pickArr.size() % m_nNumPerPage == 0 ) --nHowManyPages;
	for (i = 0; i < nHowManyPages; ++i)
	{
		int nStep = nStartRow;
		int nColStep = 0;
		operExcel.Excel_MergeRange(nStartRow, 1, nStartRow, leftCol + rightCol, celRange);
		operExcel.Excel_SetFont(celRange, nStartRow, 1, nStartRow, 1, m_nTitleSize, m_csTitleName, m_bTitleBold);
		operExcel.Excel_SetRangeContent(nStartRow, 1, title, celRange);
		operExcel.Excel_SetRowHight(celRange, ++nStep, 6.75);
		operExcel.Excel_SetRowHight(celRange, ++nStep, 3.75);
		if (nLeft > 1) {
			int posLeft = (nLeft - 2 + 1) << 1;
			operExcel.Excel_SetRangeBorder(celRange, nStep, posLeft, nStep, posLeft + 9, lxNull, lxContinous, lxContinous, lxNull);
		}
		++nStep; ++nStep;
		for (m = 0; m < 8; ++m)
		{
			++nColStep;
			operExcel.Excel_MergeRange(nStep,  nColStep, nStep,  nColStep + 1, celRange);
			++nColStep;
		}
		operExcel.Excel_SetFont(celRange, nStep, 1, nStep, leftCol + rightCol, m_nSurSize, m_csSurName, m_bSurBold);
		operExcel.Excel_SetRangeContent(nStep, 1, _T("观测："), celRange);
		operExcel.Excel_SetRangeContent(nStep, 3, m_csObserver, celRange);
		operExcel.Excel_SetRangeContent(nStep, 5, _T("记录："), celRange);
		operExcel.Excel_SetRangeContent(nStep, 7, m_csRecord, celRange);
		operExcel.Excel_SetRangeContent(nStep, 9, _T("量距"), celRange);
		operExcel.Excel_SetRangeContent(nStep, 11, m_csMeasure, celRange);
		operExcel.Excel_SetRangeContent(nStep, 13, _T("计算："), celRange);
		operExcel.Excel_SetRangeContent(nStep, 15, m_csCompute, celRange);
		++nStep;
		operExcel.Excel_MergeRange(nStep, 1, nStep, leftCol, celRange);
		operExcel.Excel_MergeRange(nStep, leftCol + 2, nStep, leftCol + rightCol, celRange);
		operExcel.Excel_SetFont(celRange, nStep, 1, nStep, leftCol + rightCol, m_nInfoSize, m_csInfoName, m_bInfoBold);
		operExcel.Excel_SetRangeContent(nStep, 1, _T("左边：（以面向前进方向）"), celRange);
		operExcel.Excel_SetRangeContent(nStep, leftCol + 1, _T("桩号"), celRange);
		operExcel.Excel_SetRangeContent(nStep, leftCol + 2, _T("右边：（以面向前进方向）"), celRange);
		operExcel.Excel_SetColWidth(celRange, leftCol + 1, (float)11.88);
		int out_begin = i * m_nNumPerPage;
		int out_end = (i + 1) * m_nNumPerPage;
		// handle borders
		operExcel.Excel_SetRangeNormalBorder(nStep, 1, nStartRow+(m_nNumPerPage<<1)+5, leftCol+rightCol, celRange);
		int nBorderRow = nStep;
		for (m = out_begin; m < out_end; ++m)
		{
			++nBorderRow;
			int nBorderRowDown = nBorderRow; ++nBorderRowDown;
			for (int k = 1; k < leftCol+ rightCol + 1; k+=2)
			{
				if (k == 1) {
					operExcel.Excel_SetRangeBorder(celRange, nBorderRow, k, nBorderRow, k, lxContinous, lxContinous, lxNull, lxNull);
					operExcel.Excel_SetRangeBorder(celRange, nBorderRowDown, k, nBorderRowDown, k, lxContinous, lxNull, lxContinous, lxNull);
				}
				else if (k == leftCol + rightCol){
					operExcel.Excel_SetRangeBorder(celRange, nBorderRow, k, nBorderRow, k, lxNull, lxContinous, lxNull, lxContinous);
					operExcel.Excel_SetRangeBorder(celRange, nBorderRowDown, k, nBorderRowDown, k, lxNull, lxNull, lxContinous, lxContinous);
				}
				else {
					operExcel.Excel_SetRangeBorder(celRange, nBorderRow, k, nBorderRow, k, lxNull, lxContinous, lxNull, lxNull);
					operExcel.Excel_SetRangeBorder(celRange, nBorderRowDown, k, nBorderRowDown, k, lxNull, lxNull, lxContinous, lxNull);
				}
			}
			operExcel.Excel_SetRangeBorder(celRange, nBorderRow, leftCol + 1, nBorderRow, leftCol + 1, lxContinous, lxContinous, lxNull, lxContinous);
			operExcel.Excel_SetRangeBorder(celRange, nBorderRowDown, leftCol + 1, nBorderRowDown, leftCol + 1, lxContinous, lxNull, lxContinous, lxContinous);
			++nBorderRow;
		}
		if (out_end > (int)pickArr.size())
		{
			out_end = pickArr.size();
		}
		for (m = out_begin; m < out_end; ++m)
		{
			int pos = (nLeft - pickArr.at(m).m_left + 1) << 1;
			++nStep; 
			int tmpStep = nStep; ++tmpStep;
			operExcel.Excel_SetDec(celRange, nStep, 1, nStartRow+(m_nNumPerPage<<1)+5, leftCol+rightCol, "@");
			operExcel.Excel_SetFont(celRange, nStep, 1, nStartRow+(m_nNumPerPage<<1)+5, leftCol+rightCol, m_nContSize, m_csContName, m_bContBold);

			// handle datas
			for (size_t si = 0; si < pickArr.at(m).m_items.size(); ++si)
			{
				CString csText = FormatDouble(fabs(pickArr.at(m).m_items.at(si).first), m_nDecimal);
				if ( fabs(pickArr.at(m).m_items.at(si).first - 0.0) < 0.0001 )
				{
					//csText = _T("K0+") ;
					//csText += FormatDouble(pickArr.at(m).m_num.c_str(), m_nDecimal);
					TCHAR szStr[100];
					double dRRR = _tstof(pickArr.at(m).m_num.c_str());
					TCHAR szLCZMC[50];
					ConvertDoubleToKplus(dRRR,szStr,3,szLCZMC);
					csText.Format(_T("%s"),szStr);
				}
				operExcel.Excel_SetRangeContent(
					nStep, pos,
					csText,
					celRange);
				operExcel.Excel_SetRangeContent(
					tmpStep, pos,
					FormatDouble(pickArr.at(m).m_items.at(si).second, m_nDecimal),
					celRange);
				pos += 2;
			}
			++nStep;
		}
		nStartRow += (m_nNumPerPage<<1) + 6;
	}
	return TRUE;
}

void VerFace::setTitleFont(int size, const TCHAR *fontname, BOOL bold)
{
    m_nTitleSize = size;
    m_csTitleName = fontname;
    m_bTitleBold = bold;
}
    
void VerFace::setSurveyFont(int size, const TCHAR *fontname, BOOL bold)
{
    m_nSurSize = size;
    m_csSurName = fontname;
    m_bSurBold = bold;
}

void VerFace::setInfoFont(int size, const TCHAR *fontname, BOOL bold)
{
    m_nInfoSize = size;
    m_csInfoName = fontname;
    m_bInfoBold = bold;
}

void VerFace::setContentFont(int size, const TCHAR *fontname, BOOL bold)
{
    m_nContSize = size;
    m_csContName = fontname;
    m_bContBold = bold;
}
