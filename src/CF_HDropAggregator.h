#pragma once
#include "IClipAggregator.h"
#include "CF_HDropBuilder.h"

class CCF_HDropAggregator : public IClipAggregator
{
public:
	CCF_HDropAggregator(void);
	~CCF_HDropAggregator(void);

	virtual bool AddClip(LPVOID lpData, int nDataSize, int nPos, int nCount, UINT cfType);
	virtual HGLOBAL GetHGlobal();
	virtual HGLOBAL GetHGlobalAsString();

protected:
	CCF_HDropBuilder m_DropFiles;
};
