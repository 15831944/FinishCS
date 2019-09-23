#pragma once
class LineParallel
{
public:
	LineParallel();
	~LineParallel();

	AcDbObjectId polyOffset(const AcDbObjectId &, double);

	AcDbObjectIdArray getExplodes(const AcDbObjectId &);

};

