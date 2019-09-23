#ifndef XMJGMACRO
#define XMJGMACRO

#ifndef _MAP_
#include <map>
#endif /*_MAP_*/

#ifndef _VECTOR_
#include <vector>
#endif /*_VECTOR_*/

#include <tchar.h>

#include "smarttable_lua.h"

using namespace std;
using std::map;
using std::vector;

typedef unsigned char byte;
typedef unsigned long ulong;

#define ZDTK_SYQ   19					//�ڵ�ͼ�� ʹ��Ȩ
#define DKTK_LANDMANAGE 20		//cassũ����ؿ��ͼ
#define ZDTK_UNKNOW_A4_S	 21
#define	ZDTK_UNKNOW_A3_H  22		
#define	ZRZTK_UNKNOW_A4_H  31	
#define ads_pointcopy(from, to) to[X] = from[X]; to[Y] = from[Y]; to[Z] = from[Z];
#define CalculateCoor(pt, val) Calculate(pt.x, val); Calculate(pt.y, val);
typedef vector<CString> VStr;
typedef vector<CString>::iterator VSIter;
typedef map<CString, CString> MStr;
typedef map<CString, CString>::iterator MSIter;
typedef map<CString, MStr> MSStr;
typedef MSStr::iterator MSSIter;
typedef MStr::const_iterator MSCIter;
typedef map<CString, VStr> MVStr;
typedef MVStr::iterator MVSIter;
typedef MVStr::const_iterator MVSCIter;
typedef map<int, int> MIInt;
typedef map<int, VStr> MIVStr;
typedef MIVStr::iterator MIVSIter;
typedef MIVStr::const_iterator MIVSCIter;
typedef map<int, double> MIDouble;
typedef MIDouble::iterator MIDIter;
typedef MIDouble::const_iterator CMIDIter;
typedef map<int, CString> MIStr;
typedef map<CString, int> MSInt;
typedef MIStr::iterator MISIter;
typedef vector<VStr> VVStr;
typedef vector<MStr> VMStr;
typedef map<CString, VMStr> MVMStr;
typedef MVMStr::iterator MVMSIter;
typedef MVMStr::const_iterator MVMSCIter;
typedef map<float, CString> MFStr;
typedef MFStr::iterator MFSIter;
typedef MFStr::const_iterator MFSCIter;
typedef map<CString, AcDbObjectId> MSId;
typedef MSId::iterator MSIdIter;
typedef map<int, AcDbObjectId> MIId;
typedef MIId::iterator MIIdIter;
typedef vector<AcApDocument *> AcApDocumentArray;
typedef vector<AcDbObjectId> VObjectID;
typedef VObjectID::iterator VObjectIDIter;
typedef map<AcDbObjectId, CString> MIdStr;
typedef MIdStr::iterator MIDSIter;
typedef map<CString, AcDbObjectIdArray> MSIds;
typedef MSIds::iterator MSIdsIter;
typedef map<CString, MSIds> MSSIds;
typedef MSSIds::iterator MSSIdsIter;
typedef map<CString, MSId> MSSId;
typedef MSSId::iterator MSSIdIter;

#define SWAP(A, B)	\
	A = A + B;		\
	B = A - B;		\
	A = A - B;

#define SetProperty(Type, Name)\
	void set##Name(const Type &name)\
	{\
		m_##Name = name;\
	}\

#define GetProperty(Type, Name)\
	Type get##Name() const\
	{\
		return m_##Name;\
	}\

#define AddProperty(Type, Name)\
	SetProperty(Type, Name);\
	GetProperty(Type, Name);\

class XMJGStoreyInfo
{
public:

	AddProperty(CString, Type);

	AddProperty(double, Width);

	AddProperty(double, Height);

	AddProperty(double, param1);

	AddProperty(double, param2);

	AddProperty(int, layerCount);

	AddProperty(CString, param3);

	AddProperty(bool, ParapetWall);

	AddProperty(CString, EndLayer);

	AddProperty(CString, FloorName);

	AddProperty(CString, StartLayer);

	AddProperty(AcDbObjectId, ObjectId);

private:

	double m_Width;
	double m_Height;
	int m_layerCount;
	bool m_ParapetWall;
	CString m_EndLayer;
	CString m_StartLayer;
	CString m_FloorName;

	CString m_Type;
	double m_param1;
	double m_param2;
	CString m_param3;

	AcDbObjectId m_ObjectId;

};

typedef XMJGStoreyInfo StoreyInfo;

struct CmpCstr
{
	bool operator()(const CString& x, const CString& y) const
	{
		CString str1 = x;
		CString str2 = y;
		TCHAR *arr1 = str1.GetBuffer();
		TCHAR *arr2 = str2.GetBuffer();
		int nLen1 = x.GetLength();
		int nLen2 = y.GetLength();
		int i = 0, j = 0;
		TCHAR *sz = _T(".");
		while(i < nLen1 && j < nLen2)
		{
			if(isdigit(arr1[i]) && isdigit(arr2[j]))
			{
				int il = (unsigned int)arr1[i] - '0';
				int ir = (unsigned int)arr2[j] - '0';
				if ((0 == il && il + ir != 0) || (0 == ir && il + ir != 0))
					return il < ir;
				else if (0 == il && 0 == ir)
				{
					i++; j++;
				}

				int m = 0, n = 0;
				TCHAR s1[20] = _T(""), s2[20] = _T("");
				while(i < nLen1 && (isdigit(arr1[i]) || arr1[i] == sz[0]))
				{
					s1[m++] = arr1[i];
					i++;
				}
				while(j < nLen2 && (isdigit(arr2[j]) || arr1[j] == sz[0]))
				{
					s2[n++] = arr2[j];
					j++;
				}
				if(_ttoi(s1) == _ttoi(s2)) break;
				else return  _ttoi(s1) < _ttoi(s2);
			}
			else if (isalpha(arr1[i]) && isdigit(arr2[j]))
			{
				return true;
			}
			else if (isdigit(arr1[i]) && isalpha(arr2[j]))
			{
				return false;
			}
			else if (isalpha(arr1[i]) && isalpha(arr2[j]))
			{
				while (i < nLen1 && j < nLen2 && arr1[i] == arr2[j] && isalpha(arr1[i]) && isalpha(arr2[j]))
				{
					i++; j++;
				}
				if (isalpha(arr1[i]) && isalpha(arr2[j]))
					return arr1[i] < arr2[j];

			}
			else
			{
				i++;
				j++;
			}
		}
		return x < y;
	}
};

typedef map<CString, StoreyInfo, CmpCstr> MIStoreyInfo;
typedef map<double, StoreyInfo> MDStoreyInfo;
typedef MIStoreyInfo::iterator MIStoreyInfoIter;
typedef MDStoreyInfo::iterator MDStoreyInfoIter;
typedef MIStoreyInfo::const_iterator CMIStoryInfoIter;
typedef MDStoreyInfo::const_iterator CMDStoryInfoIter;

#define RETFALSE(FLAG) if(FLAG == false) return 
#define RETURNFLAG(FLAG, TYPE) if(FLAG == false) return TYPE

typedef struct tagTextProperty
{
	int _color;			//��ɫ
	float _size;		//�����С
	CString _font;		//����	
	CString _layer;		//ͼ��

	tagTextProperty()
		: _color(-1), _size(-1.0)
		, _font(_T("")), _layer(_T("0"))
	{

	}

}TextProperty;

enum ExportDWGType
{
	ZongPingTu = 0,
	ZhaoPianShiYiTu = 1,
	DLZhaoPianShiYiTu = 2,
	GHSTYSGSTBYZ = 3,
	DiXingTu
};

using namespace SmartTable;

using SmartTable::Word;

//��ӡͼƬ�ĸ�ʽ��ʽ
enum RasterFormat1
{
	Raster_Jpg1 = 0,
	Raster_Bmp1 = 1
};

/* @struction �����˷�λ�;���
* @author ���
* @time 2017��3��13��
*/
struct AzimuthDist
{
	CString Azimuth;	//��λ
	CString dist;		//����
};

enum DrawDir
{
	TopDir = 0x0001,
	ButtomDir = 0x0002,
	LeftDir = 0x0004,
	RightDir = 0x0008,
	AllDir = 0x0016
};

typedef struct tagXMJGFunctionHAH
{
	float m_cg;
	CString m_bh;
	double m_jzmj;		//�������
	double m_jrmj;		//�������
	double m_jrxs;
	tagXMJGFunctionHAH()
		: m_cg(0.0),
		m_jrmj(0.0), m_jzmj(0.0)
	{

	}

	bool operator!=(const tagXMJGFunctionHAH &o)
	{
		return m_cg != o.m_cg && m_jrxs != o.m_jrxs;
	}

	bool operator==(const tagXMJGFunctionHAH &o)
	{
		return m_jrxs == o.m_jrxs && m_cg == o.m_cg;
	}

	bool operator<(const tagXMJGFunctionHAH &o)
	{
		if(m_cg < o.m_cg) return true;
		else if(fabs(m_cg - o.m_cg) < EPS)
			return m_jrxs < o.m_jrxs;
		return false;
	}

	bool operator>(const tagXMJGFunctionHAH &o)
	{
		if(m_cg > o.m_cg) return true;
		else if(fabs(m_cg - o.m_cg) < EPS)
			return m_jrxs > o.m_jrxs;
		return false;
	}

}FunctionHAH;

typedef vector<FunctionHAH> VFuns;
typedef map<float, FunctionHAH> MFFun;
typedef MFFun::iterator MFFIter;
typedef MFFun::const_iterator MFFCIter;

typedef struct tagLayerHAH
{
	CString m_lch;		//¥���
	int m_slayer;
	int m_elayer;
	double m_lcg;		//¥���
	double m_jzzmj;		//���������
	double m_jrzmj;		//���������
	int funccount;
	MFFun m_function;
	tagLayerHAH()
		: m_lch(_T("")), funccount(0),
		m_jrzmj(0.0), m_jzzmj(0.0)
		, m_lcg(0.0), m_slayer(-999)
		, m_elayer(-999)
	{

	}


}LayerHAH;

typedef map<int, LayerHAH> MILayer;
typedef MILayer::iterator MILIter;
typedef MILayer::const_iterator MILCIter;
typedef map<CString, LayerHAH> MSLayer;
typedef MSLayer::iterator MSLIter;
typedef MSLayer::const_iterator MSLCIter;

typedef struct tagBuildHAH
{
	CString m_build;
	double m_jzgd;		//�����߶�
	double m_jrmjxj;	//�������С��
	double m_jzmjxj;	//�������С��
	double m_jzzdmj;	//����ռ�����
	int funccount;
	MILayer m_layers;
	tagBuildHAH()
		: m_jzzdmj(0.0), m_jrmjxj(0.0),
		m_jzmjxj(0.0), funccount(0)
		, m_jzgd(0.0)
	{

	}
}BuildHAH;

typedef struct FunctionNameTable
{
	CString name;
	CString mjxs;
	CString jrxs;
}FNT;

struct FunctionNameTableEx
	: public FunctionNameTable
{
	int len;
	int code;			//���
	CString jm;			//��д
	CString tm;			//�ݼ�����
	CString cn;			//����+���
	bool m_init;
	CString name;		//����
	CString gaodu;		//�߶�
	bool m_isMainHeight;
	bool m_island;

	FunctionNameTableEx()
		: m_init(false)
		, m_isMainHeight(false)
	{

	}

};

typedef map<CString, BuildHAH> MSBuild;
typedef MSBuild::iterator MSBIter;
typedef MSBuild::const_iterator MSBCIter;

typedef struct PipeLineTable
{
	PipeLineTable()
		: strKey(_T("")), strGDBM(_T(""))
		, strGXBM(_T("")), strGXXX(_T(""))
		, strValue(_T("")), strExtra(_T(""))
		, strGXXL(_T("")), strGXTC(_T(""))
	{

	}
	CString strKey;
	CString strGDBM;
	CString strGXBM;
	CString strGXTC;
	CString strGXXX;
	CString strGXXL;
	CString strValue;
	CString strExtra;
}PLT;

typedef struct RXBridgeTable
{
	RXBridgeTable()
		: strXMMC(_T("")), strQLJG(_T(""))
		, strQLK(_T("")), strRemark(_T(""))
		, strIndex(_T(""))
	{

	}
	CString strXMMC;
	CString strQLJG;
	CString strQLK;
	CString strRemark;
	CString strIndex;
}RXB;

enum FILETERTYPE
{
	FilterLayer = 1,
	FilterColor = 2,
	FilterCode
};

typedef struct TableContrast //���ձ�
{
	CString dh;		//���
	CString tzd;		//������
	CString fsw;		//������
	double x;			//X����
	double y;			//Y����
	double mx;			//X����
	double my;			//Y����
	double dmgc;		//����߳�
	double gxgc;		//���߸߳�
	double mdmgc;       //����߳�
	double mgxgc;       //���߸߳�
	double gj;			//�ܾ�
	double mgj;			//�ܾ�
	CString cz;			//����
	CString msfs;		//���跽ʽ
	CString mcz;		//����
	CString mmsfs;		//���跽ʽ
}TCT;

struct PipePointProperty
{
	CString strDH;
	CString strLJD;
	CString strTZD;
	CString strXZB;
	CString strYZB;
	CString strMXZB;
	CString strMYZB;
	CString strFSW;
	CString strDMGC;
	CString strGXGC;
	CString strGJ;
	CString strTGCC;
	CString strCZ;
	CString strYL;
	CString strDLGS;
	CString strGKS;
	CString strMSFS;
	CString strMSRQ;
	CString strDLMC;
	CString strBZ;
};

struct GHSSJBQKInfo
{
	struct RoadWidth
	{
		VStr m_scRoadWidth;
		VStr m_sjRoadWidth;
	};
	struct RoadHeight
	{
		VStr m_scRoadHeight;
		VStr m_sjRoadHeight;
	};
	struct RoadLength
	{
		VStr m_scRoadLength;
		VStr m_sjRoadLength;
	};

	RoadWidth roadWidth;
	RoadHeight roadHeight;
	RoadLength roadLength;
};

struct GHSSJBQKPipe
{
	VStr m_scPipeLine;
	VStr m_sjPipeLine;
};

#endif /*XMJGMACRO*/