#include "stdafx.h"

#ifndef EPSDEF_H
#include "epsdef.h"
#endif /*EPSDEF_H*/

#include <string>
#include <fstream>
#include "tcode.h"
#include "apoint.hpp"
#include "XMJGTool.h"
using namespace std;

EPSDef::EPSDef()
{

}

EPSDef::~EPSDef()
{

}

bool EPSDef::initDef(CString path)
{
	if(false == initPath(path)) return false;
	if(false == readFile(path)) return false;
	return true;
}

bool EPSDef::initPath(CString &path)
{
	TCHAR dir[255] = {0};
	if(path.GetLength() == 0)
		if(RTNORM != ads_findfile(_T("eps.def"), dir))
		{
			acutPrintf(_T("\n无法找到[eps.def]文件"));
			return false;
		}
	path.Format(_T("%s"), dir);
	return true;
}

bool getcomments1(const tstring &str, const TCHAR *delim, vector<tstring> &ret)
{
	ret.clear();
	size_t front = 0;
	size_t next = 0;
	tstring tmp = _T("");
	int index = 0;
	for(next = str.find(delim);	(next != tstring::npos);
		front = next + 1, next = str.find(delim, front))
	{
		tmp = str.substr(front, (next - front));
		ret.push_back(tmp);
		tmp = _T("");
		++index;
	}
	tmp = str.substr(front);
	ret.push_back(tmp);
	return true;
}

bool EPSDef::readFile(const CString &file)
{
	tifstream fin(file);
	fin.imbue(std::locale(""));
	if(!fin) return false;
	m_db.clear();
	for(tstring str; getline(fin, str);)
	{
		trim(str);
		if(str.find(_T(",")) == tstring::npos)
			continue;
		vector<tstring> parts;
		getcomments1(str, _T(","), parts);
		if(parts.size() > 0)
		{
			epsdb db;
			switch(parts.size())
			{
				case 7:
					db.m_alias.Format(_T("%s"), parts[6].c_str());
				case 6:
					db.m_width = _tstof(parts[5].c_str());
				case 5:
					db.m_type.Format(_T("%s"), parts[4].c_str());
				case 4:
					db.m_color = _tstoi(parts[3].c_str());
				case 3:
					db.m_layer.Format(_T("%s"), parts[2].c_str());
				case 2:
					db.m_code.Format(_T("%s"), parts[1].c_str());
				case 1:
					db.m_jm.Format(_T("%s"), parts[0].c_str());
					break;
				default:
					break;
			}
			m_db.push_back(db);
		}
	}
	fin.close();
	return true;
}

bool EPSDef::getdbFromJM(const CString &jm, epsdb &db)
{
	for(int idx = 0; idx < (int)m_db.size(); ++idx)
	{
		epsdb tmp = m_db[idx];
		if(tmp.m_jm.CompareNoCase(jm) != 0) continue;
		db = tmp;
	}
	return true;
}

bool EPSDef::getdbFromCode(const CString &code, epsdb &db)
{
	for(int idx = 0; idx < (int)m_db.size(); ++idx)
	{
		epsdb tmp = m_db[idx];
		if(tmp.m_code.CompareNoCase(code) != 0) continue;
		db = tmp;
	}
	return true;
}

CorDef::CorDef()
{

}

CorDef::~CorDef()
{

}

void CorDef::drawData()
{
	CLockManager lock;
	EPSDef epsdef; if(epsdef.initDef() == false) return;
	for(MSVPIter it = m_pt.begin(); it != m_pt.end(); ++it)
	{
		epsdb db;
		if(false == epsdef.getdbFromJM(it->first, db)) continue;
		VPoint3dArray vnode = it->second;
		for(int idx = 0; idx < (int)vnode.size(); ++idx)
		{
			if(false == drawPipePoint(vnode[idx], db))
				drawPipeLine(vnode[idx], db);
			drawPipePointText(vnode[idx], db);
		}
	}
}

bool CorDef::initFile(const CString &file)
{
	tifstream fin(file);
	fin.imbue(std::locale(""));
	if(!fin) return false;
	m_pt.clear();
	for(tstring str; getline(fin, str);)
	{
		trim(str);
		if(str.find(_T(" ")) == tstring::npos)
			continue;
		vector<tstring> parts;
		getcomments1(str, _T(" "), parts);
		int count = _tstoi(parts[1].c_str());
		AcGePoint3dArray line;
		CString key(parts[0].c_str());
		for(int idx = 0; idx < count; ++idx)
		{
			getline(fin, str);
			getcomments1(str, _T(" "), parts);
			AcGePoint3d pt = analyzePoint(parts);
			line.append(pt);
		}
		m_pt[key].push_back(line);
	}
	return true;
}

void CorDef::drawPipeLine(AcGePoint3dArray &node, epsdb &db)
{
	AcGePoint2dArray line;
	for(int idx = 0; idx < node.length(); ++idx)
		line.append(apoint(node[idx]));
	db.m_id = DrawPolyline2D(line, 0, false, db.m_code, db.m_width);
	setlayer(aname(db.m_id), db.m_layer); 
	setlinetype(aname(db.m_id), db.m_type);
}

bool CorDef::drawPipePoint(AcGePoint3dArray &node, epsdb &db)
{
	TCHAR path[255] = {0};
	if(RTNORM != ads_findfile(db.m_type + _T(".dwg"), path)) return false;
	for(int idx = 0; idx < node.length(); ++idx)
	{
		ads_point pos = {node[idx].x, node[idx].y, node[idx].z};
		db.m_id = DrawInsert(apoint(node[idx]), db.m_type);
		addapp(aname(db.m_id), db.m_code); 
		setlayer(aname(db.m_id), db.m_layer);
		setcolor(aname(db.m_id), db.m_color);
		AddXdata(aname(db.m_id), _T("XH"), 0, db.m_jm);
	}
	return true;
}

AcGePoint3d CorDef::analyzePoint(const vector<tstring> &parts)
{
	AcGePoint3d pt;
	pt.x = _tstof(parts[1].c_str());
	pt.y = _tstof(parts[2].c_str());
	pt.z = _tstof(parts[3].c_str());
	return pt;
}

void CorDef::drawPipePointText(const AcGePoint3dArray &node, const epsdb &db)
{
	double scale = 500.0;
	read1(scale); scale /= 1000.0;
	TCHAR info[255] = {0};
	for(int idx = 0; idx < node.length(); ++idx)
	{
		AcDbObjectId id = DrawText(node[idx], db.m_jm, _T("BL"), _T(""), scale * 1.678);
		_stprintf(info, _T("%.3lf"), node[idx].z);
		setlayer(aname(id), _T("ZJ")); AcDbObjectId groupid;
		AddXdata(aname(id), _T("ZZB"), 0, info);
		AddXdata(aname(id), _T("XH"), 0, db.m_jm);
		AddXdata(aname(id), _T("ZJLX"), 0, _T("XH"));
		CreateGroup(aname(db.m_id), groupid);
		AddEntToGroup(id, groupid);
	}
}