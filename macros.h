#pragma once

#define MAX_ATTRSIZE				2000
#define INVALID_VALUE				99999

//数据库字典
#define DICT_PROJECTINFO			_T("Dict_ProjectInfo")

//图层
#define TEMP_LAYER					_T("FCS_TEMP")			//工程临时图层

//cass编码
#define CASSCODE_HUTS			_T("141500")		//棚房编码
#define CASSCODE_480410			_T("480410")		//图廊注记编码
#define CASSCODE_999999			_T("999999")

//扩展属性
#define XDATA_TXTZ					_T("TXTZ")			//图廊图形性质
#define XDATA_FRAME_TYPE		_T("FRAME_TYPE")	//		//1 单独生成图框 2 批量生成的图框
#define XDATA_ENTITY_TYPE		_T("ENTITY_TYPE")		//图框要素类型  1 面积图左下角图例  2 面积图右下角汇总表
																				//3 面积图表头表格(图名) 4 总平图建筑实体

#define XDATA_CALCULATE_AREA			_T("CALCULATE_AREA")		//
#define XDATA_FloorSummary				_T("FloorSummary")

#ifndef SPOINT
#define SPOINT(p, x, y, z) (*(p) = (x), (p)[1] = (y), (p)[2] = (z))
#endif