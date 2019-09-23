
#ifndef	RTREE_H
#define	RTREE_H


#include <vector>


/*namespace PIPETREE{*/
#ifndef NULL
#define NULL 0
#endif

#define RTREE_DIMS_NUMB 2//number of dimensions
#define RTREE_SIDES_NUMB 2*RTREE_DIMS_NUMB
#define RTREE_MAXCARD 9//max branching factor of a node
#define RTREE_METHODS 1

//If passed to a tree search, this callback function will be called
//with the ID of each data mbr that overlaps the search mbr
//plus whatever user specific pointer was passed to the search.
//It can terminate the search early by returning 0 in which case
//the search will return the number of hits found up to that point.
typedef int(*pfnRTreeCallback)(void* id, void* pfnParam);

//用于返回找到的id及其外包矩形rect(CRTreeMBR类型)
typedef int(*pfnRTreeCallbackRect)(void *id, void *rect, void* pfnParam);

class CRTreeMBR;
class CRTreeBranch;
class CRTreeNode;
class CRTreeListNode;
class CRTreePartition;
class CRTree;

class  CRTreeMBR
{
public:
	double m_bound[RTREE_SIDES_NUMB];
	CRTreeMBR(void)
	{
		for(int i = 0; i < RTREE_SIDES_NUMB; i++)
		{
			m_bound[i] = 0.0;
		}
	}

	void InitRect(void)
	{
		for(int i = 0; i < RTREE_SIDES_NUMB; i++)
		{
			m_bound[i] = 0.0;
		}
	}

	bool IsValideRect(void)//检查边界是否有效,有效返回true,否则返回false
	{
		for(int i = 0; i<RTREE_DIMS_NUMB; i++)
		{
			if(m_bound[i] > m_bound[i + RTREE_DIMS_NUMB])
			{
				return false;
			}
		}
		return true;
	}

	void SetInvalidRect(void)
	{
		m_bound[0] = 1.0;
		m_bound[RTREE_DIMS_NUMB] = -1.0;
		for(int i = 1; i < RTREE_DIMS_NUMB; i++)
		{
			m_bound[i] = m_bound[i + RTREE_DIMS_NUMB] = 0.0;
		}
	}
};

class  CRTreeBranch
{
public:
	CRTreeMBR m_mbr;
	CRTreeNode *m_child;

	~CRTreeBranch(void)
	{
		m_child = NULL;
	}

	void InitBranch(void)
	{
		m_child = NULL;
		m_mbr.InitRect();
	}

	CRTreeBranch(CRTreeMBR mbr, CRTreeNode *node)
	{
		m_mbr = mbr;
		m_child = node;
	}

	CRTreeBranch(void)
	{
		m_child = NULL;
	}
};
class  CRTreePartition
{
public:
	int m_partition[RTREE_MAXCARD + 1];
	bool m_taken[RTREE_MAXCARD + 1];
	int m_total;
	int m_minfill;
	int m_count[2];
	CRTreeMBR m_cover[2];
	double m_area[2];

	CRTreePartition(void);


	void Init(void);
};
class  CRTree
{
public:
	~CRTree(void);
	CRTree(void);

	void Init(void);
	void Destroy(void);
	bool InsertRect(CRTreeMBR *rc, void* tid, int level = 0);
	bool DeleteRect(CRTreeMBR *rc, void* tid);
	bool DeleteID(void* tid);
	bool DeleteRectOverlap(CRTreeMBR *rc, pfnRTreeCallback pfnSHCB = NULL, void* pfnParam = NULL);
	bool DeleteRectInclude(CRTreeMBR *rc, pfnRTreeCallback pfnSHCB = NULL, void* pfnParam = NULL);
	void GetMinMax(double *mat);
	void Traversal(pfnRTreeCallback pfnSHCB, void* pfnParam = NULL);
	void TraversalRect(pfnRTreeCallbackRect pfnSHCB, void* pfnParam = NULL);
	int GetInclude(CRTreeMBR *rc, std::vector<void*> &vID);
	int GetOverlap(CRTreeMBR *rc, std::vector<void*> &vID);
	int SearchOverlap(CRTreeMBR *rc, pfnRTreeCallback pfnSHCB = NULL, void* pfnParam = NULL);
	int SearchOverlapRect(CRTreeMBR *rc, pfnRTreeCallbackRect pfnSHCB = NULL, void* pfnParam = NULL);
	int SearchInclude(CRTreeMBR *rc, pfnRTreeCallback pfnSHCB = NULL, void* pfnParam = NULL);
	int SearchIncludeRect(CRTreeMBR *rc, pfnRTreeCallbackRect pfnSHCB = NULL, void* pfnParam = NULL);
	int GetCount(void)
	{
		return m_idNum;
	}

private:
	CRTreeNode *m_root;
	int m_idNum;
	double m_unitSphereVolumes[21];
	int m_branchCount;
	double m_coverSplitArea;
	CRTreeMBR m_coverSplit;
	CRTreeBranch m_branchBuf[RTREE_MAXCARD + 1];
	CRTreePartition m_partitions[RTREE_METHODS];

	bool _InsertRect(CRTreeMBR *rc, void* tid, CRTreeNode *node, CRTreeNode **new_node, int level);
	void _InitBranch(CRTreeBranch *br);
	void _GetBranches(CRTreeNode *node, CRTreeBranch *br);
	void _MethodZero(CRTreePartition *p, int minfill);
	void _Classify(int i, int group, CRTreePartition *p);
	void _LoadNodes(CRTreeNode *n, CRTreeNode *q, CRTreePartition *p);
	void _InitPart(CRTreePartition *p, int maxrects, int minfill);
	void _PickSeeds(CRTreePartition *p);
	void _DisconnectBranch(CRTreeNode *node, int k);
	void _DestroyNode(CRTreeNode *node);
	void _SplitNode(CRTreeNode *node, CRTreeBranch *br, CRTreeNode **new_node);
	void _Traversal(CRTreeNode *node, pfnRTreeCallback pfnSHCB, void* pfnParam);
	void _TraversalRect(CRTreeNode *node, pfnRTreeCallbackRect pfnSHCB, void* pfnParam);
	bool _AddBranch(CRTreeBranch *br, CRTreeNode *node, CRTreeNode **new_node);
	bool _DeleteRect(CRTreeMBR *rc, void* tid, CRTreeNode *node, CRTreeListNode **ee);
	bool _IsOverlap(CRTreeMBR *rc1, CRTreeMBR *rc2);
	bool _IsInclude(CRTreeMBR *rc1, CRTreeMBR *rc2);
	int _PickBranch(CRTreeMBR *rc, CRTreeNode *node);
	int _SearchOverlap(CRTreeNode *node, CRTreeMBR *rc, pfnRTreeCallback pfnSHCB, void* pfnParam);
	int _SearchOverlapRect(CRTreeNode *node, CRTreeMBR *rc, pfnRTreeCallbackRect pfnSHCB, void* pfnParam);
	int _SearchInclude(CRTreeNode *node, CRTreeMBR *rc, pfnRTreeCallback pfnSHCB, void* pfnParam);
	int _SearchIncludeRect(CRTreeNode *node, CRTreeMBR *rc, pfnRTreeCallbackRect pfnSHCB, void* pfnParam);
	int _GetInclude(CRTreeNode *node, CRTreeMBR *rc, std::vector<void*> &vID);
	int _GetOverlap(CRTreeNode *node, CRTreeMBR *rc, std::vector<void*> &vID);
	double _RectSphericalVolume(CRTreeMBR *rc);
	CRTreeMBR _CombineRect(CRTreeMBR *rc1, CRTreeMBR *rc2);
	CRTreeMBR _NodeCover(CRTreeNode *node);
	bool _DeleteRectOverlap(CRTreeMBR *rc, CRTreeNode *node, CRTreeListNode **ee, pfnRTreeCallback pfnSHCB, void* pfnParam);
	bool _DeleteRectInclude(CRTreeMBR *rc, CRTreeNode *node, CRTreeListNode **ee, pfnRTreeCallback pfnSHCB, void* pfnParam);
	bool _DeleteID(CRTreeNode *node, void* tid, CRTreeListNode **ee);
};


class  CRTreeListNode
{
public:
	CRTreeNode *m_node;
	CRTreeListNode *m_next;

	~CRTreeListNode(void)
	{
		m_next = NULL;
		m_node = NULL;
	}

	CRTreeListNode(void)
	{
		m_next = NULL;
		m_node = NULL;
	}

	CRTreeListNode(CRTreeNode *node, CRTreeListNode *next)
	{
		m_node = node;
		m_next = next;
	}
};
class  CRTreeNode
{
public:
	int m_count;
	int m_level;
	CRTreeBranch m_branch[RTREE_MAXCARD];

	CRTreeNode(void);


	void InitNode(void);

};
#endif /* RTREE_H_INCLUDED */

