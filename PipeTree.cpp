//////////////////////////////////////////////////////////////////////////////////
//RTree索引类的定义
//2013年2月22日
//邓德标
//该类由R-Tree library改进(AUTHOR(S): Antonin Guttman. LAST MODIFY: ZhangLiang 2007-11)
//////////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "PipeTree.h"
#include <assert.h>
#include <math.h>
#include <fstream>
using namespace std;


/*namespace PIPETREE{*/
#ifdef Q_OS_WIN
#pragma execution_character_set("utf-8")
#define isnan _isnan
#endif

CRTree::~CRTree(void)
{
	if (m_root != NULL)
	{
		Destroy();
	}
}

CRTree::CRTree(void)
{
	m_unitSphereVolumes[0] = 0.0;
	m_unitSphereVolumes[1] = 2.0;
	m_unitSphereVolumes[2] = 3.141593;
	m_unitSphereVolumes[3] = 4.188790;
	m_unitSphereVolumes[4] = 4.934802;
	m_unitSphereVolumes[5] = 5.263789;
	m_unitSphereVolumes[6] = 5.167713;
	m_unitSphereVolumes[7] = 4.724766;
	m_unitSphereVolumes[8] = 4.058712;
	m_unitSphereVolumes[9] = 3.298509;
	m_unitSphereVolumes[10] = 2.550164;
	m_unitSphereVolumes[11] = 1.884104;
	m_unitSphereVolumes[12] = 1.335263;
	m_unitSphereVolumes[13] = 0.910629;
	m_unitSphereVolumes[14] = 0.599265;
	m_unitSphereVolumes[15] = 0.381443;
	m_unitSphereVolumes[16] = 0.235331;
	m_unitSphereVolumes[17] = 0.140981;
	m_unitSphereVolumes[18] = 0.082146;
	m_unitSphereVolumes[19] = 0.046622;
	m_unitSphereVolumes[20] = 0.025807;

	m_root = new CRTreeNode;
	m_idNum = 0;
	m_root->m_level = 0;
	m_branchCount = 0;
	m_coverSplitArea = 0.0;
}

//Inserts a new data rectangle into the index structure.
//Recursively descends tree, propagates splits back up.
//Returns false if node was not split. Old node updated.
//If node was split, returns true and sets the pointer pointed to by
//new_node to point to the new node. Old node updated to become one of two.
//The level argument specifies the number of steps up from the leaf
//level to insert; e.g. a data rectangle goes in at level = 0.
bool CRTree::_InsertRect(CRTreeMBR *rc, void* tid, CRTreeNode *node, CRTreeNode **new_node, int level)
{
	assert(node && new_node);
	assert(level >= 0 && level <= node->m_level);
	CRTreeBranch b;
	CRTreeNode *n2 = NULL;
	//Still above level for insertion, go down tree recursively
	if (node->m_level > level)
	{
		int i = _PickBranch(rc, node);
		if (!_InsertRect(rc, tid, node->m_branch[i].m_child, &n2, level))
		{
			//child was not split
			node->m_branch[i].m_mbr = _CombineRect(rc, &(node->m_branch[i].m_mbr));
			return false;
		}

		//child was split
		node->m_branch[i].m_mbr = _NodeCover(node->m_branch[i].m_child);
		b.m_child = n2;
		b.m_mbr = _NodeCover(n2);
		return _AddBranch(&b, node, new_node);
	} 
	else if (node->m_level == level)//Have reached level for insertion. Add mbr, split if necessary
	{
		m_idNum++;
		b.m_mbr = *rc;

#pragma warning(push)//C4312
#pragma warning(disable : 4312)
		b.m_child = (CRTreeNode*)tid;
#pragma warning(pop)

		//child field of leaves contains tid of data record
		return _AddBranch(&b, node, new_node);
	}
	//Not supposed to happen
	assert(false);
	return false;
}

//Add a branch to a node. Split the node if necessary.
//Returns 0 if node not split. Old node updated.
//Returns 1 if node split, sets *new_node to address of new node.
//Old node updated, becomes one of two.
bool CRTree::_AddBranch(CRTreeBranch *br, CRTreeNode *node, CRTreeNode **new_node)
{
	assert(br && node);
	if (node->m_count < RTREE_MAXCARD)//split won't be necessary
	{
		for (int i=0; i<RTREE_MAXCARD; i++)//find empty branch
		{
			if (node->m_branch[i].m_child == NULL)
			{
				node->m_branch[i] = *br;
				node->m_count++;
				break;
			}
		}
		return false;
	}
	assert(new_node);
	_SplitNode(node, br, new_node);
	return true;
}

//Split a node.
//Divides the nodes branches and the extra one between two nodes.
//Old node is one of the new ones, and one really new one is created.
//Tries more than one method for choosing a partition, uses best result.
void CRTree::_SplitNode(CRTreeNode *node, CRTreeBranch *br, CRTreeNode **new_node)
{
	assert(node && br);
	//load all the branches into a buffer, initialize old node
	int level = node->m_level;
	_GetBranches(node, br);
	//find partition
	CRTreePartition *p = &m_partitions[0];
	//Note: can't use MINFILL(n) below since node was cleared by GetBranches()
	_MethodZero(p, RTREE_MAXCARD/2);
	//put branches from buffer into 2 nodes according to chosen partition
	*new_node = new CRTreeNode;
	(*new_node)->m_level = node->m_level = level;
	_LoadNodes(node, *new_node, p);
	assert(node->m_count + (*new_node)->m_count == p->m_total);
}

//Method #0 for choosing a partition:
//As the seeds for the two groups, pick the two rects that would waste the
//most area if covered by a single rectangle, i.e. evidently the worst pair
//to have in the same group.
//Of the remaining, one at a time is chosen to be put in one of the two groups.
//The one chosen is the one with the greatest difference in area expansion
//depending on which group - the mbr most strongly attracted to one group
//and repelled from the other.
//If one group gets too full (more would force other group to violate min
//fill requirement) then other group gets the rest.
//These last are the ones that can go in either group most easily.
void CRTree::_MethodZero(CRTreePartition *p, int minfill)
{
	assert(p);
	double biggestDiff;
	int group, chosen=0, betterGroup=0;

	_InitPart(p, m_branchCount, minfill);
	_PickSeeds(p);

	while (p->m_count[0] + p->m_count[1] < p->m_total &&
		p->m_count[0] < p->m_total - p->m_minfill &&
		p->m_count[1] < p->m_total - p->m_minfill)
	{
		biggestDiff = -1.0;
		for (int i=0; i<p->m_total; i++)
		{
			if (!p->m_taken[i])
			{
				CRTreeMBR *r, rect_0, rect_1;
				double growth0, growth1, diff;

				r = &m_branchBuf[i].m_mbr;
				rect_0 = _CombineRect(r, &p->m_cover[0]);
				rect_1 = _CombineRect(r, &p->m_cover[1]);
				growth0 = _RectSphericalVolume(&rect_0) - p->m_area[0];
				growth1 = _RectSphericalVolume(&rect_1) - p->m_area[1];
				diff = growth1 - growth0;
				if (diff >= 0)
				{
					group = 0;
				}
				else
				{
					group = 1;
					diff = -diff;
				}
				if (diff > biggestDiff)
				{
					biggestDiff = diff;
					chosen = i;
					betterGroup = group;
				}
				//else if (diff == biggestDiff && p->m_count[group] < p->m_count[betterGroup])
				else if (fabs(diff - biggestDiff) < 1e-6 && p->m_count[group] < p->m_count[betterGroup])
				{
					chosen = i;
					betterGroup = group;
				}
				//新增加,确保chosen不会使用上次循环的值
				else
				{
					if (chosen == -1)
					{
						chosen = i;
						if (p->m_count[group] < p->m_count[betterGroup])
						{
							betterGroup = group;
						}
					}
				}
			}
		}
		_Classify(chosen, betterGroup, p);
	}

	//if one group too full, put remaining rects in the other
	if (p->m_count[0] + p->m_count[1] < p->m_total)
	{
		if (p->m_count[0] >= p->m_total - p->m_minfill)
		{
			group = 1;
		}
		else
		{
			group = 0;
		}

		for (int i=0; i<p->m_total; i++)
		{
			if (!p->m_taken[i])
			{
				_Classify(i, group, p);
			}
		}
	}
	assert(p->m_count[0] + p->m_count[1] == p->m_total);
	assert(p->m_count[0] >= p->m_minfill && p->m_count[1] >= p->m_minfill);
}

//Put a branch in one of the groups.
void CRTree::_Classify(int i, int group, CRTreePartition *p)
{
	assert(p);
	assert(!p->m_taken[i]);

	p->m_partition[i] = group;
	p->m_taken[i] = true;

	if (p->m_count[group] == 0)
	{
		p->m_cover[group] = m_branchBuf[i].m_mbr;
	}
	else
	{
		p->m_cover[group] = _CombineRect(&m_branchBuf[i].m_mbr, &p->m_cover[group]);
	}

	p->m_area[group] = _RectSphericalVolume(&p->m_cover[group]);
	p->m_count[group]++;
}

//Pick two rects from set to be the first elements of the two groups.
//Pick the two that waste the most area if covered by a single rectangle.
void CRTree::_PickSeeds(CRTreePartition *p)
{
	int seed0 = 0, seed1 = 0;
	double worst, waste;
	double area[RTREE_MAXCARD+1];

	for (int i=0; i<p->m_total; i++)
	{
		area[i] = _RectSphericalVolume(&m_branchBuf[i].m_mbr);
	}
	worst = -m_coverSplitArea - 1;

	for (int i=0; i<p->m_total-1; i++)
	{
		for (int j=i+1; j<p->m_total; j++)
		{
			CRTreeMBR one_rect;
			one_rect = _CombineRect(&m_branchBuf[i].m_mbr, &m_branchBuf[j].m_mbr);
			waste = _RectSphericalVolume(&one_rect) - area[i] - area[j];
			if (waste > worst)
			{
				worst = waste;
				seed0 = i;
				seed1 = j;
			}
		}
	}
	_Classify(seed0, 0, p);
	_Classify(seed1, 1, p);
}

//Copy branches from the buffer into two nodes according to the partition.
void CRTree::_LoadNodes(CRTreeNode *n, CRTreeNode *q, CRTreePartition *p)
{
	assert(n && q && p);
	for (int i=0; i<p->m_total; i++)
	{
		assert(p->m_partition[i] == 0 || p->m_partition[i] == 1);
		if (p->m_partition[i] == 0)
		{
			_AddBranch(&m_branchBuf[i], n, NULL);
		}
		else if (p->m_partition[i] == 1)
		{
			_AddBranch(&m_branchBuf[i], q, NULL);
		}
	}
}

//Initialize a RTREEPARTITION structure.
void CRTree::_InitPart(CRTreePartition *p, int maxrects, int minfill)
{
	assert(p);
	p->m_count[0] = p->m_count[1] = 0;
	p->m_cover[0].SetInvalidRect();
	p->m_cover[1].SetInvalidRect();
	p->m_area[0] = p->m_area[1] = 0.0;
	p->m_total = maxrects;
	p->m_minfill = minfill;
	for (int i=0; i<maxrects; i++)
	{
		p->m_taken[i] = false;
		p->m_partition[i] = -1;
	}
}

//Load branch buffer with branches from full node plus the extra branch.
void CRTree::_GetBranches(CRTreeNode *node, CRTreeBranch *br)
{
	assert(node && br);
	//load the branch buffer
	for (int i=0; i<RTREE_MAXCARD; i++)
	{
		assert(node->m_branch[i].m_child);//n should have every entry full
		m_branchBuf[i] = node->m_branch[i];
	}
	m_branchBuf[RTREE_MAXCARD] = *br;
	m_branchCount = RTREE_MAXCARD + 1;

	//calculate mbr containing all in the set
	m_coverSplit = m_branchBuf[0].m_mbr;
	for (int i=1; i<RTREE_MAXCARD+1; i++)
	{
		m_coverSplit = _CombineRect(&m_coverSplit, &m_branchBuf[i].m_mbr);
	}
	m_coverSplitArea = _RectSphericalVolume(&m_coverSplit);
	node->InitNode();
}

//Pick a branch. Pick the one that will need the smallest increase
//in area to accomodate the new rectangle. This will result in the
//least total area for the covering rectangles in the current node.
//In case of a tie, pick the one which was smaller before, to get
//the best resolution when searching.
int CRTree::_PickBranch(CRTreeMBR *rc, CRTreeNode *node)
{
	assert(rc && node);
	CRTreeMBR *r;
	int first_time = 1;
	double increase, area, bestIncr = -1.0, bestArea = 0.0;
	int best = 0;
	CRTreeMBR tmp_rect;

	for (int i=0; i<RTREE_MAXCARD; i++)
	{
		if (node->m_branch[i].m_child)
		{
			r = &node->m_branch[i].m_mbr;
			area = _RectSphericalVolume(r);
			tmp_rect = _CombineRect(rc, r);
			increase = _RectSphericalVolume(&tmp_rect) - area;
			if (increase < bestIncr || first_time)
			{
				best = i;
				bestArea = area;
				bestIncr = increase;
				first_time = 0;
			}
			else if (increase == bestIncr && area < bestArea)
			{
				best = i;
				bestArea = area;
				bestIncr = increase;
			}
		}
	}
	return best;
}

double CRTree::_RectSphericalVolume(CRTreeMBR *rc)
{
	assert(rc);
	double sum_of_squares = 0.0;
	for (int i=0; i<RTREE_DIMS_NUMB; i++)
	{
		double half_extent = 0.5*(rc->m_bound[i+RTREE_DIMS_NUMB] - rc->m_bound[i]);
		sum_of_squares += half_extent*half_extent;
	}
	double radius = sqrt(sum_of_squares);
	return pow(radius, RTREE_DIMS_NUMB)*m_unitSphereVolumes[RTREE_DIMS_NUMB];
}

//Combine two rectangles, make one that includes both.
CRTreeMBR CRTree::_CombineRect(CRTreeMBR *rc1, CRTreeMBR *rc2)
{
	assert(rc1 && rc2);
	CRTreeMBR new_rect = *rc1;
	for (int i=0; i<RTREE_DIMS_NUMB; i++)
	{
		if (rc1->m_bound[i] < rc2->m_bound[i])
		{
			new_rect.m_bound[i] = rc1->m_bound[i];
		}
		else
		{
			new_rect.m_bound[i] = rc2->m_bound[i];
		}

		int j = i + RTREE_DIMS_NUMB;
		if (rc1->m_bound[j] > rc2->m_bound[j])
		{
			new_rect.m_bound[j] = rc1->m_bound[j];
		}
		else
		{
			new_rect.m_bound[j] = rc2->m_bound[j];
		}
	}
	return new_rect;
}

//Find the smallest rectangle that includes all rectangles in branches of a node
CRTreeMBR CRTree::_NodeCover(CRTreeNode *node)
{
	assert(node);
	CRTreeMBR rc;
	bool first_time = true;
	for (int i=0; i<RTREE_MAXCARD; i++)
	{
		if (node->m_branch[i].m_child)
		{
			if (first_time)
			{
				rc = node->m_branch[i].m_mbr;
				first_time = false;
			}
			else
			{
				rc = _CombineRect(&rc, &(node->m_branch[i].m_mbr));
			}
		}
	}
	return rc;
}

//Search in an index tree or subtree for all data rectangles that overlap the argument rectangle.
//Return the number of qualifying data rects.
int CRTree::_SearchOverlap(CRTreeNode *node, CRTreeMBR *rc, pfnRTreeCallback pfnSHCB, void* pfnParam)
{
	assert(node);
	assert(node->m_level >= 0);

	//Fix not yet tested
	int hitCount = 0;
	if (node->m_level > 0)//this is an internal node in the tree
	{
		for (int i=0; i<RTREE_MAXCARD; i++)
		{
			if (node->m_branch[i].m_child != NULL)
			{
				if (_IsOverlap(rc, &node->m_branch[i].m_mbr))
				{
					hitCount += _SearchOverlap(node->m_branch[i].m_child, rc, pfnSHCB, pfnParam);
				}
			}
		}
	}
	else//this is a leaf node
	{
#pragma warning(push)//C4311
#pragma warning(disable : 4311)
		for (int i=0; i<RTREE_MAXCARD; i++)
		{
			if (node->m_branch[i].m_child != NULL)
			{
				if (_IsOverlap(rc, &node->m_branch[i].m_mbr))
				{
					hitCount++;
					//call the user-provided callback and return if callback wants to terminate search early
					if (pfnSHCB != NULL)
					{
						if (!pfnSHCB((void*)node->m_branch[i].m_child, pfnParam))
						{
							return hitCount;
						}
					}
				}
			}
		}
#pragma warning(pop)
	}
	return hitCount;
}

int CRTree::_SearchOverlapRect(CRTreeNode *node, CRTreeMBR *rc, pfnRTreeCallbackRect pfnSHCB, void* pfnParam)
{
	assert(node);
	assert(node->m_level >= 0);

	//Fix not yet tested
	int hitCount = 0;
	if (node->m_level > 0)//this is an internal node in the tree
	{
		for (int i=0; i<RTREE_MAXCARD; i++)
		{
			if (node->m_branch[i].m_child != NULL)
			{
				if (_IsOverlap(rc, &node->m_branch[i].m_mbr))
				{
					hitCount += _SearchOverlapRect(node->m_branch[i].m_child, rc, pfnSHCB, pfnParam);
				}
			}
		}
	}
	else//this is a leaf node
	{
#pragma warning(push)//C4311
#pragma warning(disable : 4311)
		for (int i=0; i<RTREE_MAXCARD; i++)
		{
			if (node->m_branch[i].m_child != NULL)
			{
				if (_IsOverlap(rc, &node->m_branch[i].m_mbr))
				{
					hitCount++;
					//call the user-provided callback and return if callback wants to terminate search early
					if (pfnSHCB != NULL)
					{
						if (!pfnSHCB((void*)node->m_branch[i].m_child, (void *)&node->m_branch[i].m_mbr, pfnParam))
						{
							return hitCount;
						}
					}
				}
			}
		}
#pragma warning(pop)
	}
	return hitCount;
}

//Decide whether two rectangles overlap.
bool CRTree::_IsOverlap(CRTreeMBR *rc1, CRTreeMBR *rc2)
{
	assert(rc1 && rc2);
	for (int i=0; i<RTREE_DIMS_NUMB; i++)
	{
		int j = i + RTREE_DIMS_NUMB;//index for high sides
		if (rc1->m_bound[i] > rc2->m_bound[j] || rc2->m_bound[i] > rc1->m_bound[j])
		{
			return false;
		}
	}
	return true;
}

//Delete a data rectangle from an index structure.
//Pass in a pointer to a RTREEMBR, the tid of the record, ptr to ptr to root node.
//Returns true if record not found, false if success.
//RTreeDeleteRect provides for eliminating the root.
//在指定边界rc内查找指定元素tid并删除
//若边界不正确则在最后调用DeleteID进行删除
//删除成功返回true,否则返回false
bool CRTree::DeleteRect(CRTreeMBR *rc, void* tid)
{
	//int nID = (int)tid;
	//QMessageBox::information(NULL, "Delete", QString::number(nID));

	if (!rc->IsValideRect() || tid == NULL)
	{
		return false;
	}
	CRTreeNode *tmp_nptr = NULL;
	CRTreeListNode *reInsertList = NULL;
	CRTreeListNode *e;
	if (_DeleteRect(rc, tid, m_root, &reInsertList))
	{
		//found and deleted a data item
		//reinsert any branches from eliminated nodes
		while (reInsertList != NULL)
		{
			tmp_nptr = reInsertList->m_node;

#pragma warning(push)//C4311
#pragma warning( disable : 4311)
			for (int i=0; i<RTREE_MAXCARD; i++)
			{
				if (tmp_nptr->m_branch[i].m_child != NULL)
				{
					InsertRect(&(tmp_nptr->m_branch[i].m_mbr), (void*)tmp_nptr->m_branch[i].m_child, tmp_nptr->m_level);
				}
			}
#pragma warning(pop)

			e = reInsertList;
			reInsertList = reInsertList->m_next;
			assert(e->m_node);
			delete e->m_node;
			e->m_node = NULL;
			delete e;
			e = NULL;
		}

		//check for redundant root (not leaf, 1 child) and eliminate
		if (m_root->m_count == 1 && m_root->m_level > 0)
		{
			for (int i=0; i<RTREE_MAXCARD; i++)
			{
				tmp_nptr = m_root->m_branch[i].m_child;
				if (tmp_nptr != NULL)
				{
					break;
				}
			}
			assert(tmp_nptr);
			assert(m_root);
			delete m_root;
			m_root = tmp_nptr;
		}
		return true;
	}//end if

	//删除不成功，遍历一次进行删除
	return DeleteID(tid);

//	//删除不成功，获取极值边界重新进行删除
//	double mat[RTREE_SIDES_NUMB];
//	GetMinMax(mat);
//	CRTreeMBR mbr;
//	for (int i=0; i<RTREE_DIMS_NUMB; i++)
//	{
//		int j = i + RTREE_DIMS_NUMB;
//		mbr.m_bound[i] = mat[i] - 0.000000001;
//		mbr.m_bound[j] = mat[j] + 0.000000001;
//	}
//
//	*rc = mbr;
//	if (_DeleteRect(rc, tid, m_root, &reInsertList))
//	{
//		//found and deleted a data item
//		//reinsert any branches from eliminated nodes
//		while (reInsertList != NULL)
//		{
//			tmp_nptr = reInsertList->m_node;
//
//#pragma warning(push)//C4311
//#pragma warning( disable : 4311)
//			for (int i=0; i<RTREE_MAXCARD; i++)
//			{
//				if (tmp_nptr->m_branch[i].m_child != NULL)
//				{
//					InsertRect(&(tmp_nptr->m_branch[i].m_mbr), (void*)tmp_nptr->m_branch[i].m_child, tmp_nptr->m_level);
//				}
//			}
//#pragma warning(pop)
//
//			e = reInsertList;
//			reInsertList = reInsertList->m_next;
//			assert(e->m_node);
//			delete e->m_node;
//			e->m_node = NULL;
//			delete e;
//			e = NULL;
//		}
//
//		//check for redundant root (not leaf, 1 child) and eliminate
//		if (m_root->m_count == 1 && m_root->m_level > 0)
//		{
//			for (int i=0; i<RTREE_MAXCARD; i++)
//			{
//				tmp_nptr = m_root->m_branch[i].m_child;
//				if (tmp_nptr != NULL)
//				{
//					break;
//				}
//			}
//			assert(tmp_nptr);
//			assert(m_root);
//			delete m_root;
//			m_root = tmp_nptr;
//		}
//		return true;
//	}//end if
//	return false;
}

//Delete a rectangle from non-root part of an index structure.
//Called by RTreeDeleteRect. Descends tree recursively,
//merges branches on the way back up.
//Returns 1 if success, 0 if record not found.
bool CRTree::_DeleteRect(CRTreeMBR *rc, void* tid, CRTreeNode *node, CRTreeListNode **ee)
{
	if (node == NULL)
	{
		return false;
	}
	if (node->m_level > 0)//not a leaf node
	{
		for (int i=0; i<RTREE_MAXCARD; i++)
		{
			if (node->m_branch[i].m_child != NULL)
			{
				if (_IsOverlap(rc, &(node->m_branch[i].m_mbr)))
				{
					if (_DeleteRect(rc, tid, node->m_branch[i].m_child, ee))
					{
						if (node->m_branch[i].m_child->m_count >= RTREE_MAXCARD/2)
						{
							node->m_branch[i].m_mbr = _NodeCover(node->m_branch[i].m_child);
						}
						else
						{
							//not enough entries in child, eliminate child node
							(*ee) = new CRTreeListNode(node->m_branch[i].m_child, *ee);
							_DisconnectBranch(node, i);
						}
						return true;
					}
				}
			}
		}
		return false;
	}
	else
	{
#pragma warning(push)//C4312
#pragma warning( disable : 4312 )
		for (int i=0; i<RTREE_MAXCARD; i++)
		{
			if (node->m_branch[i].m_child != NULL)
			{
				if (node->m_branch[i].m_child == (CRTreeNode*)tid)
				{
					m_idNum--;
					_DisconnectBranch(node, i);
					return true;
				}
			}
		}
#pragma warning(pop)
	}
	return false;
}

//删除指定元素tid,删除成功返回true,否则返回false
bool CRTree::DeleteID(void* tid)
{
	if (tid == NULL)
	{
		return false;
	}

	CRTreeNode *tmp_nptr = NULL;
	CRTreeListNode *reInsertList = NULL;
	CRTreeListNode *e = NULL;
	if (_DeleteID(m_root, tid, &reInsertList))
	{
		while (reInsertList != NULL)
		{
			tmp_nptr = reInsertList->m_node;

#pragma warning(push)//C4311
#pragma warning( disable : 4311)
			for (int i=0; i<RTREE_MAXCARD; i++)
			{
				if (tmp_nptr->m_branch[i].m_child != NULL)
				{
					InsertRect(&(tmp_nptr->m_branch[i].m_mbr), (void*)tmp_nptr->m_branch[i].m_child, tmp_nptr->m_level);
				}
			}
#pragma warning(pop)

			e = reInsertList;
			reInsertList = reInsertList->m_next;
			assert(e->m_node);
			delete e->m_node;
			e->m_node = NULL;
			delete e;
			e = NULL;
		}

		//check for redundant root (not leaf, 1 child) and eliminate
		if (m_root->m_count == 1 && m_root->m_level > 0)
		{
			for (int i=0; i<RTREE_MAXCARD; i++)
			{
				tmp_nptr = m_root->m_branch[i].m_child;
				if (tmp_nptr != NULL)
				{
					break;
				}
			}
			assert(tmp_nptr);
			assert(m_root);
			delete m_root;
			m_root = tmp_nptr;
		}
		return true;
	}//end if
	return false;
}

//根据id号删除元素
bool CRTree::_DeleteID(CRTreeNode *node, void* tid, CRTreeListNode **ee)
{
	if (node->m_level > 0)
	{
		for (int i=0; i<RTREE_MAXCARD; i++)
		{
			if (node->m_branch[i].m_child != NULL)
			{
				if (_DeleteID(node->m_branch[i].m_child, tid, ee))
				{
					if (node->m_branch[i].m_child->m_count >= RTREE_MAXCARD/2)
					{
						node->m_branch[i].m_mbr = _NodeCover(node->m_branch[i].m_child);
					}
					else
					{
						(*ee) = new CRTreeListNode(node->m_branch[i].m_child, *ee);
						_DisconnectBranch(node, i);
					}
					return true;
				}
			}
		}
	}
	else
	{
//#pragma warning(push)//C4312
//#pragma warning( disable : 4312 )
		for (int i=0; i<RTREE_MAXCARD; i++)
		{
			if (node->m_branch[i].m_child == (CRTreeNode*)tid)
			{
				m_idNum--;
				_DisconnectBranch(node, i);
				return true;
			}
		}
//#pragma warning(pop)
	}
	return false;	
}

//删除与rc相交的元素
bool CRTree::DeleteRectOverlap(CRTreeMBR *rc, pfnRTreeCallback pfnSHCB, void* pfnParam)
{
	assert(rc);
	CRTreeNode *tmp_nptr = NULL;
	CRTreeListNode *reInsertList = NULL;
	CRTreeListNode *e;
	if (!_DeleteRectOverlap(rc, m_root, &reInsertList, pfnSHCB, pfnParam))
	{
		//found and deleted a data item
		//reinsert any branches from eliminated nodes
		while (reInsertList != NULL)
		{
			tmp_nptr = reInsertList->m_node;

#pragma warning(push)//C4311
#pragma warning( disable : 4311 )
			for (int i=0; i<RTREE_MAXCARD; i++)
			{
				if (tmp_nptr->m_branch[i].m_child != NULL)
				{
					InsertRect(&(tmp_nptr->m_branch[i].m_mbr), (void*)tmp_nptr->m_branch[i].m_child, tmp_nptr->m_level);
				}
			}
#pragma warning(pop)

			e = reInsertList;
			reInsertList = reInsertList->m_next;
			assert(e->m_node);
			delete e->m_node;
			e->m_node = NULL;
			delete e;
			e = NULL;
		}

		//check for redundant root (not leaf, 1 child) and eliminate
		if (m_root->m_count == 1 && m_root->m_level > 0)
		{
			for (int i=0; i<RTREE_MAXCARD; i++)
			{
				tmp_nptr = m_root->m_branch[i].m_child;
				if (tmp_nptr != NULL)
				{
					break;
				}
			}
			assert(tmp_nptr);
			assert(m_root);
			delete m_root;
			m_root = tmp_nptr;
		}
		return false;
	}
	return true;
}

//删除与rc相交的元素
bool CRTree::_DeleteRectOverlap(CRTreeMBR *rc, CRTreeNode *node, CRTreeListNode **ee, pfnRTreeCallback pfnSHCB, void* pfnParam)
{
	assert(node && ee);
	assert(node->m_level >= 0);
	if (node->m_level > 0)//not a leaf node
	{
		for (int i=0; i<RTREE_MAXCARD; i++)
		{
			if (node->m_branch[i].m_child != NULL)
			{
				if (_IsOverlap(rc, &(node->m_branch[i].m_mbr)))
				{
					if (!_DeleteRectOverlap(rc, node->m_branch[i].m_child, ee, pfnSHCB, pfnParam))
					{
						if (node->m_branch[i].m_child->m_count >= RTREE_MAXCARD/2)
						{
							node->m_branch[i].m_mbr = _NodeCover(node->m_branch[i].m_child);
						}
						else
						{
							//not enough entries in child, eliminate child node
							(*ee) = new CRTreeListNode(node->m_branch[i].m_child, *ee);
							_DisconnectBranch(node, i);
						}
						return false;
					}
				}
			}
		}
		return true;
	}

#pragma warning(push)//C4312
#pragma warning( disable : 4312)

	//a leaf node
	for (int i=0; i<RTREE_MAXCARD; i++)
	{
		if (node->m_branch[i].m_child != NULL)
		{
			m_idNum--;
			_DisconnectBranch(node, i);
			if (pfnSHCB != NULL)
			{
				pfnSHCB((void*)node->m_branch[i].m_child, pfnParam);
			}
			return false;
		}
	}
#pragma warning(pop)
	return true;
}

//删除与rc相交的元素
bool CRTree::DeleteRectInclude(CRTreeMBR *rc, pfnRTreeCallback pfnSHCB, void* pfnParam)
{
	assert(rc);
	CRTreeNode *tmp_nptr = NULL;
	CRTreeListNode *reInsertList = NULL;
	CRTreeListNode *e;
	if (!_DeleteRectInclude(rc, m_root, &reInsertList, pfnSHCB, pfnParam))
	{
		//found and deleted a data item
		//reinsert any branches from eliminated nodes
		while (reInsertList != NULL)
		{
			tmp_nptr = reInsertList->m_node;

#pragma warning(push)//C4311
#pragma warning( disable : 4311 )
			for (int i=0; i<RTREE_MAXCARD; i++)
			{
				if (tmp_nptr->m_branch[i].m_child != NULL)
				{
					InsertRect(&(tmp_nptr->m_branch[i].m_mbr), (void*)tmp_nptr->m_branch[i].m_child, tmp_nptr->m_level);
				}
			}
#pragma warning(pop)

			e = reInsertList;
			reInsertList = reInsertList->m_next;
			assert(e->m_node);
			delete e->m_node;
			e->m_node = NULL;
			delete e;
			e = NULL;
		}

		//check for redundant root (not leaf, 1 child) and eliminate
		if (m_root->m_count == 1 && m_root->m_level > 0)
		{
			for (int i=0; i<RTREE_MAXCARD; i++)
			{
				tmp_nptr = m_root->m_branch[i].m_child;
				if (tmp_nptr != NULL)
				{
					break;
				}
			}
			assert(tmp_nptr);
			assert(m_root);
			delete m_root;
			m_root = tmp_nptr;
		}
		return false;
	}
	return true;
}

//删除与rc相交的元素
bool CRTree::_DeleteRectInclude(CRTreeMBR *rc, CRTreeNode *node, CRTreeListNode **ee, pfnRTreeCallback pfnSHCB, void* pfnParam)
{
	assert(node && ee);
	assert(node->m_level >= 0);

	if (node->m_level > 0)//not a leaf node
	{
		for (int i=0; i<RTREE_MAXCARD; i++)
		{
			if (node->m_branch[i].m_child != NULL)
			{
				if (_IsOverlap(rc, &(node->m_branch[i].m_mbr)))
				{
					if (!_DeleteRectInclude(rc, node->m_branch[i].m_child, ee, pfnSHCB, pfnParam))
					{
						if (node->m_branch[i].m_child->m_count >= RTREE_MAXCARD/2)
						{
							node->m_branch[i].m_mbr = _NodeCover(node->m_branch[i].m_child);
						}
						else
						{
							//not enough entries in child, eliminate child node
							(*ee) = new CRTreeListNode(node->m_branch[i].m_child, *ee);
							_DisconnectBranch(node, i);
						}
						return false;
					}
				}
			}
		}
		return true;
	}

#pragma warning(push)//C4312
#pragma warning( disable : 4312 )

	//a leaf node
	for (int i=0; i<RTREE_MAXCARD; i++)
	{
		if (_IsInclude(rc, &(node->m_branch[i].m_mbr)))
		{
			if (node->m_branch[i].m_child != NULL)
			{
				m_idNum--;
				_DisconnectBranch(node, i);
				if (pfnSHCB != NULL)
				{
					pfnSHCB((void*)node->m_branch[i].m_child, pfnParam);
				}
				return false;
			}
		}
	}
#pragma warning(pop)
	return true;
}

//Disconnect a dependent node.
void CRTree::_DisconnectBranch(CRTreeNode *node, int i)
{
	assert(node && i >= 0 && i < RTREE_MAXCARD);
	assert(node->m_branch[i].m_child);
	node->m_branch[i].InitBranch();
	node->m_count--;
}

//Destroy (free) node recursively.
void CRTree::_DestroyNode(CRTreeNode *node)
{
	if (node != NULL)
	{
		if (node->m_level > 0)
		{
			//it is not leaf -> destroy childs
			for (int i=0; i<RTREE_MAXCARD; i++)
			{
				if (node->m_branch[i].m_child != NULL)
				{
					_DestroyNode(node->m_branch[i].m_child);
				}
			}
		}
		delete node;
		node = NULL;
	}
}

//判断rc1是否包含rc2
bool CRTree::_IsInclude(CRTreeMBR *rc1, CRTreeMBR *rc2)
{
	assert(rc1 && rc2);
	for (int i=0; i<RTREE_DIMS_NUMB; i++)
	{
		int j = i + RTREE_DIMS_NUMB;
		if (rc1->m_bound[i] > rc2->m_bound[i] || rc1->m_bound[j] < rc2->m_bound[j])
		{
			return false;
		}
	}
	return true;
}

int CRTree::_SearchInclude(CRTreeNode *node, CRTreeMBR *rc, pfnRTreeCallback pfnSHCB, void* pfnParam)
{
	assert(node);
	assert(node->m_level >= 0);

	int hitCount = 0;
	if (node->m_level > 0)
	{
		for (int i=0; i<RTREE_MAXCARD; i++)
		{
			if (node->m_branch[i].m_child != NULL)
			{
				if (_IsOverlap(rc, &node->m_branch[i].m_mbr))
				{
					hitCount += _SearchInclude(node->m_branch[i].m_child, rc, pfnSHCB, pfnParam);
				}
			}
		}
	}
	else
	{
#pragma warning(push)
#pragma warning(disable : 4311)
		for (int i=0; i<RTREE_MAXCARD; i++)
		{
			if (node->m_branch[i].m_child != NULL)
			{
				if (_IsInclude(rc, &node->m_branch[i].m_mbr))
				{
					hitCount++;
					if (pfnSHCB != NULL)
					{
						if (!pfnSHCB((void*)node->m_branch[i].m_child, pfnParam))
						{
							return hitCount;
						}
					}
				}
			}
		}
#pragma warning(pop)
	}
	return hitCount;
}

int CRTree::_SearchIncludeRect( CRTreeNode *node,CRTreeMBR *rc, pfnRTreeCallbackRect pfnSHCB, void* pfnParam )
{
	assert(node);
	assert(node->m_level >= 0);

	int hitCount = 0;
	if (node->m_level > 0)
	{
		for (int i=0; i<RTREE_MAXCARD; i++)
		{
			if (node->m_branch[i].m_child != NULL)
			{
				if (_IsOverlap(rc, &node->m_branch[i].m_mbr))
				{
					hitCount += _SearchIncludeRect(node->m_branch[i].m_child, rc, pfnSHCB, pfnParam);
				}
			}
		}
	}
	else
	{
#pragma warning(push)
#pragma warning(disable : 4311)
		for (int i=0; i<RTREE_MAXCARD; i++)
		{
			if (node->m_branch[i].m_child != NULL)
			{
				if (_IsInclude(rc, &node->m_branch[i].m_mbr))
				{
					hitCount++;
					if (pfnSHCB != NULL)
					{
						if (!pfnSHCB((void *)node->m_branch[i].m_child, (void *)&node->m_branch[i].m_mbr, pfnParam))
						{
							return hitCount;
						}
					}
				}
			}
		}
#pragma warning(pop)
	}
	return hitCount;
}

void CRTree::_Traversal(CRTreeNode *node, pfnRTreeCallback pfnSHCB, void* pfnParam)
{
	assert(node);
	int i;
	if (node->m_level == 0)
	{
		for (i=0; i<RTREE_MAXCARD; i++)
		{
			if (pfnSHCB != NULL && node->m_branch[i].m_child != NULL)
			{
				pfnSHCB((void*)node->m_branch[i].m_child, pfnParam);
			}
		}
		return;
	}
	for (i=0; i<RTREE_MAXCARD; i++)
	{
		if (node->m_branch[i].m_child != NULL)
		{
			_Traversal(node->m_branch[i].m_child, pfnSHCB, pfnParam);
		}
	}
}

void CRTree::_TraversalRect(CRTreeNode *node, pfnRTreeCallbackRect pfnSHCB, void* pfnParam)
{
	assert(node);
	int i;
	if (node->m_level == 0)
	{
		for (i=0; i<RTREE_MAXCARD; i++)
		{
			if (pfnSHCB != NULL && node->m_branch[i].m_child != NULL)
			{
				pfnSHCB((void*)node->m_branch[i].m_child, (void *)&node->m_branch[i].m_mbr, pfnParam);
			}
		}
		return;
	}
	for (i=0; i<RTREE_MAXCARD; i++)
	{
		if (node->m_branch[i].m_child != NULL)
		{
			_TraversalRect(node->m_branch[i].m_child, pfnSHCB, pfnParam);
		}
	}
}

int CRTree::_GetOverlap(CRTreeNode *node, CRTreeMBR *rc, std::vector<void*> &vID)
{
	assert(node);
	assert(node->m_level >= 0);

	int hitCount = 0;
	if (node->m_level > 0)
	{
		for (int i=0; i<RTREE_MAXCARD; i++)
		{
			if (node->m_branch[i].m_child != NULL)
			{
				if (_IsOverlap(rc, &node->m_branch[i].m_mbr))
				{
					hitCount += _GetOverlap(node->m_branch[i].m_child, rc, vID);
				}
			}
		}
	}
	else
	{
#pragma warning(push)
#pragma warning(disable : 4311)
		for (int i=0; i<RTREE_MAXCARD; i++)
		{
			if (node->m_branch[i].m_child != NULL)
			{
				if (_IsOverlap(rc, &node->m_branch[i].m_mbr))
				{
					hitCount++;
					vID.push_back((void*)node->m_branch[i].m_child);
				}
			}
		}
#pragma warning(pop)
	}
	return hitCount;
}

int CRTree::_GetInclude(CRTreeNode *node, CRTreeMBR *rc, std::vector<void*> &vID)
{
	assert(node);
	assert(node->m_level >= 0);

	int hitCount = 0;
	if (node->m_level > 0)
	{
		for (int i=0; i<RTREE_MAXCARD; i++)
		{
			if (node->m_branch[i].m_child != NULL)
			{
				if (_IsOverlap(rc, &node->m_branch[i].m_mbr))
				{
					hitCount += _GetInclude(node->m_branch[i].m_child, rc, vID);
				}
			}
		}
	}
	else
	{
#pragma warning(push)
#pragma warning(disable : 4311)
		for (int i=0; i<RTREE_MAXCARD; i++)
		{
			if (node->m_branch[i].m_child != NULL)
			{
				if (_IsInclude(rc, &node->m_branch[i].m_mbr))
				{
					hitCount++;
					vID.push_back((void*)node->m_branch[i].m_child);
				}
			}
		}
#pragma warning(pop)
	}
	return hitCount;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//重置树,声明变量后不用调用该函数
void CRTree::Init(void)
{
	if (m_root != NULL)
	{
		Destroy();
	}
	m_root = new CRTreeNode;
	m_root->m_level = 0;
}

//销毁删除树,在析构函数中会进行销毁
void CRTree::Destroy(void)
{
	_DestroyNode(m_root);
	m_root = NULL;
	m_idNum = 0;
	m_branchCount = 0;
	m_coverSplitArea = 0.0;
	m_coverSplit.InitRect();
	for (int i=0; i<RTREE_MAXCARD+1; i++)
	{
		m_branchBuf[i].InitBranch();
	}

	for (int i=0; i<RTREE_METHODS; i++)
	{
		m_partitions[i].Init();
	}
}

bool CRTree::InsertRect(CRTreeMBR *rc, void* tid, int level)
{
	if ((long double)fabs(rc->m_bound[0]) < 0.0001)
	{
		rc->m_bound[0] = 0.0;
	}
	if ((long double)fabs(rc->m_bound[1])  < 0.0001)
	{
		rc->m_bound[1] = 0.0;
	}
	if ((long double)fabs(rc->m_bound[2])  < 0.0001)
	{
		rc->m_bound[2] = rc->m_bound[0] + 1.0;
	}
	if ((long double)fabs(rc->m_bound[3]) < 0.0001)
	{
		rc->m_bound[3] = rc->m_bound[1] + 1.0;
	}

	//////////////////////////////////////////////////////////////////////////

	assert(rc);
	assert(tid);
	assert(level >= 0 && level <= m_root->m_level);

	CRTreeNode *newnode = NULL;//root split
	if (_InsertRect(rc, tid, m_root, &newnode, level))
	{
		CRTreeNode *newroot = new CRTreeNode;//grow a new root, tree taller
		newroot->m_level = m_root->m_level + 1;
		CRTreeBranch b;
		b.m_mbr = _NodeCover(m_root);
		b.m_child = m_root;
		_AddBranch(&b, newroot, NULL);
		b.m_mbr = _NodeCover(newnode);
		b.m_child = newnode;
		_AddBranch(&b, newroot, NULL);
		m_root = newroot;
		return true;
	}
	return false;
}

//获取坐标极值,mat是数组,排列为xmin,ymin,zmin,xmax,ymax,zmax...
void CRTree::GetMinMax(double *mat)
{
	assert(mat);
	int i, j, k;
	CRTreeMBR mbr;

	for (k=0; k<RTREE_MAXCARD; k++)
	{
		if (m_root->m_branch[k].m_child != NULL)
		{
			mbr = m_root->m_branch[k].m_mbr;
			break;
		}
	}
	if (k == RTREE_MAXCARD)
	{
		return;
	}
	for (i=0; i<RTREE_DIMS_NUMB; i++)
	{
		j = i + RTREE_DIMS_NUMB;
		mat[i] = mbr.m_bound[i];
		mat[j] = mbr.m_bound[j];
	}

	for (k=k+1; k<RTREE_MAXCARD; k++)
	{
		if (m_root->m_branch[k].m_child != NULL)
		{
			for (i=0; i<RTREE_DIMS_NUMB; i++)
			{
				j = i + RTREE_DIMS_NUMB;
				mbr = m_root->m_branch[k].m_mbr;
				if (mat[i] > mbr.m_bound[i])
				{
					mat[i] = mbr.m_bound[i];
				}
				if (mat[j] < mbr.m_bound[j])
				{
					mat[j] = mbr.m_bound[j];
				}
			}
		}
	}
}

//遍历树,遍历到的元素需在回调函数里作相应操作
void CRTree::Traversal(pfnRTreeCallback pfnSHCB, void* pfnParam)
{
	_Traversal(m_root, pfnSHCB, pfnParam);
}

void CRTree::TraversalRect(pfnRTreeCallbackRect pfnSHCB, void* pfnParam/*=NULL*/)
{
	_TraversalRect(m_root, pfnSHCB, pfnParam);
}

//查找在矩形rc里的元素,pfnSHCB是回调函数,pfnParam是回调函数的参数
int CRTree::SearchInclude(CRTreeMBR *rc, pfnRTreeCallback pfnSHCB, void* pfnParam)
{
	assert(rc);
	return _SearchInclude(m_root, rc, pfnSHCB, pfnParam);
}

int CRTree::SearchIncludeRect(CRTreeMBR *rc, pfnRTreeCallbackRect pfnSHCB/*=NULL*/, void* pfnParam/*=NULL*/)
{
	assert(rc);
	return _SearchIncludeRect(m_root, rc, pfnSHCB, pfnParam);
}

//查找与矩形rc相交和重叠的元素,pfnSHCB是回调函数,pfnParam是回调函数的参数
int CRTree::SearchOverlap(CRTreeMBR *rc, pfnRTreeCallback pfnSHCB, void* pfnParam)
{
	assert(rc);
	return _SearchOverlap(m_root, rc, pfnSHCB, pfnParam);
}

int CRTree::SearchOverlapRect(CRTreeMBR *rc, pfnRTreeCallbackRect pfnSHCB/*=NULL*/, void* pfnParam/*=NULL*/)
{
	assert(rc);
	return _SearchOverlapRect(m_root, rc, pfnSHCB, pfnParam);
}

//获取与矩形相交的元素,该函数里会先清空vID
int CRTree::GetOverlap(CRTreeMBR *rc, std::vector<void*> &vID)
{
	assert(rc);
	vID.clear();
	return _GetOverlap(m_root, rc, vID);
}

//获取在矩形范围内的元素,该函数里会先清空vID
int CRTree::GetInclude(CRTreeMBR *rc, std::vector<void*> &vID)
{
	assert(rc);
	vID.clear();
	return _GetInclude(m_root, rc, vID);
}


// void CRTreeMBR::CRTreeMBR(void);//初始化边界
// {
// 	for (int i = 0; i < RTREE_SIDES_NUMB; i++)
// 	{
// 		m_bound[i] = 0.0;
// 	}
// }


CRTreeNode::CRTreeNode(void)
{
	m_count = 0;
	m_level = -1;
}
void CRTreeNode::InitNode()
{
	m_count = 0;
	m_level = -1;
	for (int i = 0; i < RTREE_MAXCARD; i++)
	{
		m_branch[i].InitBranch();
	}
}


CRTreePartition::CRTreePartition(void)
{
	m_total = 0;
	m_minfill = 0;
	m_count[0] = m_count[1] = 0;
	m_area[0] = m_area[1] = 0.0;
	for (int i = 0; i < RTREE_MAXCARD + 1; i++)
	{
		m_partition[i] = 0;
		m_taken[i] = false;
	}
}
void CRTreePartition::Init(void)
{
	m_total = 0;
	m_minfill = 0;
	m_count[0] = m_count[1] = 0;
	m_area[0] = m_area[1] = 0.0;
	for (int i = 0; i < RTREE_MAXCARD + 1; i++)
	{
		m_partition[i] = 0;
		m_taken[i] = false;
	}
}

