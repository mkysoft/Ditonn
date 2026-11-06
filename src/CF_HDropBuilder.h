#pragma once

#include <afx.h>

class CCF_HDropBuilder
{
public:
	void AddFile(const CString& file) { m_files.Add(file); }
	HGLOBAL CreateCF_HDROPBuffer() const;
	HGLOBAL CreateCF_HDROPBufferAsString() const;

private:
	CStringArray m_files;
};