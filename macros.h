#pragma once

#define MAX_ATTRSIZE				2000
#define INVALID_VALUE				99999

//���ݿ��ֵ�
#define DICT_PROJECTINFO			_T("Dict_ProjectInfo")

//ͼ��
#define TEMP_LAYER					_T("FCS_TEMP")			//������ʱͼ��

//cass����
#define CASSCODE_HUTS			_T("141500")		//�﷿����
#define CASSCODE_480410			_T("480410")		//ͼ��ע�Ǳ���
#define CASSCODE_999999			_T("999999")

//��չ����
#define XDATA_TXTZ					_T("TXTZ")			//ͼ��ͼ������
#define XDATA_FRAME_TYPE		_T("FRAME_TYPE")	//		//1 ��������ͼ�� 2 �������ɵ�ͼ��
#define XDATA_ENTITY_TYPE		_T("ENTITY_TYPE")		//ͼ��Ҫ������  1 ���ͼ���½�ͼ��  2 ���ͼ���½ǻ��ܱ�
																				//3 ���ͼ��ͷ���(ͼ��) 4 ��ƽͼ����ʵ��

#define XDATA_CALCULATE_AREA			_T("CALCULATE_AREA")		//
#define XDATA_FloorSummary				_T("FloorSummary")

#ifndef SPOINT
#define SPOINT(p, x, y, z) (*(p) = (x), (p)[1] = (y), (p)[2] = (z))
#endif