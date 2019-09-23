#include "stdafx.h"
#include "autorebuild.h"

void CAutoRebuild::divEntity(AcDbObjectId objTB, AcDbObjectId objDiv, AcDbObjectIdArray &ResTBArr)
{
	int i = 0;
	AcGePoint2dArray nodesR;

	double dArea1 = 0.0, dArea2 = 0.0;
	GetEntArea(aname(objTB), dArea1);
	GetEntArea(aname(objDiv), dArea2);

	//�������1��
	//���и����뱻�и�ͼ���غ�ʱ��ֹͣ�и�
	if(fabs(dArea1 - dArea2) < 0.00001)
	{
		AcGePoint2dArray nodesTB, nodesDiv;
		GetPlList(aname(objTB), nodesTB);
		GetPlList(aname(objDiv), nodesDiv);

		ads_point ptMid1, ptMid2;
		GetNodesMiddlePoint(nodesTB, ptMid1);
		GetNodesMiddlePoint(nodesDiv, ptMid2);

		if(distOFpt(ptMid1, ptMid2) < 0.0001)
		{
			acutPrintf(_T("\nһ���غϵ��棬�����и"));
			return;
		}
	}


	//�������2��
	//�����и�Ĵ�ͼ�����и�ʵ������ʱ����ȫû�ཻ�����Ͱ��и�ʵ�帴��һ������
	//�����ǽ�����������ܰѵ��۳������ֲ���ɾ�������ͼ�ߣ��������������̻��
	//����Ĵ�ͼ��ɾ��
	
	if(dArea1 > dArea2)
	{
		AcGePoint2dArray nodesTB, nodesRes;
		AcGeDoubleArray doubleArr;
		GetPlList(aname(objTB), nodesTB);
		GetPlList(aname(objDiv), nodesR);
		if(distOF2d(nodesTB.first(), nodesTB.last()) >= EPS)	nodesTB.append(nodesTB.first());
		if(distOF2d(nodesR.first(), nodesR.last()) >= EPS)	nodesR.append(nodesR.first());

		//����ʵ��û�ཻ����ͼ��������и�ʵ�������
		polylineIntersection(nodesTB, nodesR, nodesRes, doubleArr);
		if(nodesRes.length() == 0)
		{
			AcDbObjectId objNew = CopyEntity(objTB);
			SetPolyCoord(aname(objNew), nodesR);
			ResTBArr.append(objNew);
			return;
		}

	}

	//����������
	CAutoDiv autoD;
	long nLen = 0;
	AcDbObjectIdArray objArr;
	objArr.append(objTB);
	objArr.append(objDiv);
	for ( i = 0 ; i < objArr.length() ; i++ )
	{
		int j = 0;
		AcGePoint2dArray nodes;
		AcDbObjectId idAdd = objArr.at(i);

		double dLength = 0;


		GetEntLength(aname(idAdd) , dLength );
		if ( dLength < 1e-7 )
			continue;
		GetPlList(aname(idAdd)  , nodes , true );
		if ( GetPolyType(aname(idAdd) , true ) >= 10 && nodes.first().distanceTo( nodes.last() ) > 1e-7 )
			nodes.append( nodes.first() );
		//����ϸ΢��
		for ( j = 0 ; j < nodes.length() ; j++ )
		{
			int nIdx = 0;
			if( _FindNode( nodesR , nodes[j] , nIdx ) )
			{
				nodes.setAt( j , nodesR[nIdx] );
			}
			else
			{
				nodesR.insertAt( nIdx , nodes[j] );
			}
		}		

		//����ع��ķ������ѽڵ��������ȥ
		for( j = 1; j < nodes.length(); j ++)
		{
			AcGePoint2dArray nodesE;
			nodesE.append(nodes.at(j));
			nodesE.append(nodes.at(j-1));

			autoD.AddNodeArr(nodesE, true );
		}

		AcGePoint2dArray nodesLast;
		nodesLast.append(nodes.first());
		nodesLast.append(nodes.last());
		autoD.AddNodeArr(nodesLast, true );

	}
	acedRestoreStatusBar();

	autoD.RunDiv(false);
	autoD.ShowMePolyline();
	CAutoLoop autoL;
	autoL.AddEdges( autoD.m_arrNodesOut /*anaPoly.m_arrNodes*/ );
	autoL.GetDoubleLoops();

	AcGePoint2dArray TBNodes;
	GetPlList(aname(objTB), TBNodes);
	CArray<AcGePoint2dArray, AcGePoint2dArray> TBNodesArr;
	TBNodesArr.Add(TBNodes);	
	bool bUpdate = false;		//ֻ��ʵ���и���ͼ�߲���Ҫɾ��ԭ��ͼ�ߣ���Ҫ�����ж�
	for (i = 0; i < autoL.m_arrLoops.GetSize(); ++i)
	{
		AcGePoint2dArray Nodes = autoL.m_arrLoops.GetAt(i);
		if (IsPolyInPoly(Nodes, TBNodes) && IsPolyValid(Nodes, TBNodesArr))
		{
			bUpdate = true;
			AcDbObjectId NewTBId = CopyEntity(objTB);
			SetPolyCoord(aname(NewTBId), Nodes);
			ResTBArr.append(NewTBId);
		}
	}
	if (bUpdate)
	{
		EntDel(aname(objTB));
	}
}


bool CAutoRebuild::_FindNode( AcGePoint2dArray & nodes , AcGePoint2d pt , int &nIdx )
{
	int low = 0 , high = nodes.length() - 1;
	int mid;

	while ( low <= high )
	{
		mid = ( low + high ) / 2;
		int es = _CompareNode( nodes[mid] , pt );

		if ( 0 == es ) 
		{
			nIdx = mid;
			return true;
		}
		else if ( -1 == es )
		{
			low = mid + 1;
		}
		else 
		{
			high = mid - 1;
		}
	}

	nIdx = low;
	return false;
}

int CAutoRebuild::_CompareNode( AcGePoint2d pt1 , AcGePoint2d pt2 )
{
	if ( (pt1.x - pt2.x)*(pt1.x - pt2.x) + (pt1.y - pt2.y)*(pt1.y - pt2.y) < 1e-10 )
		//if( pt1.distanceTo( pt2 ) < 1e-5 )
	{
		return 0;
	}
	if ( pt1.x < pt2.x  )
	{
		return -1;
	}
	else if ( pt1.x > pt2.x  )
	{
		return 1;
	}
	if ( pt1.y < pt2.y )
	{
		return -1;
	}
	return 1;
}


CAutoRebuild::CAutoRebuild(void)
{
}

CAutoRebuild::~CAutoRebuild(void)
{
}
