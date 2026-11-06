#include "stdafx.h"
#include "CF_HDropBuilder.h"
#include "Misc.h"

HGLOBAL CCF_HDropBuilder::CreateCF_HDROPBufferAsString() const
{
	CString data;
	for (INT_PTR i = 0; i < m_files.GetSize(); ++i)
	{
		data += m_files[i];
		data += _T("\r\n");
	}

	return NewGlobalP(data.GetBuffer(), (data.GetLength() + 1) * sizeof(TCHAR));
}

HGLOBAL CCF_HDropBuilder::CreateCF_HDROPBuffer() const
{
	if (m_files.GetSize() <= 0)
	{
		return NULL;
	}

	SIZE_T characterCount = 1;
	for (INT_PTR i = 0; i < m_files.GetSize(); ++i)
	{
		characterCount += static_cast<SIZE_T>(m_files[i].GetLength()) + 1;
	}

	const SIZE_T bufferSize = sizeof(DROPFILES) + characterCount * sizeof(TCHAR);
	TCHAR* buffer = new TCHAR[bufferSize / sizeof(TCHAR)];
	ZeroMemory(buffer, bufferSize);

	DROPFILES* dropFiles = reinterpret_cast<DROPFILES*>(buffer);
	dropFiles->pFiles = sizeof(DROPFILES);
	dropFiles->fWide = TRUE;

	TCHAR* current = reinterpret_cast<TCHAR*>(reinterpret_cast<BYTE*>(buffer) + sizeof(DROPFILES));
	for (INT_PTR i = 0; i < m_files.GetSize(); ++i)
	{
		const CString& file = m_files[i];
		STRCPY(current, static_cast<LPCTSTR>(file));
		current += file.GetLength() + 1;
	}

	HGLOBAL result = NewGlobalP(buffer, bufferSize);
	delete[] buffer;
	return result;
}