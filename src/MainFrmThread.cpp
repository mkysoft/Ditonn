#include "stdafx.h"
#include "MainFrmThread.h"
#include "DatabaseUtilities.h"
#include "Options.h"
#include "Misc.h"
#include "cp_main.h"

	}
}

void CMainFrmThread::OnDeleteEntries()
{
    RemoveOldEntries(true);
}

void CMainFrmThread::OnRemoveTempFiles()
{
	DeleteDittoTempFiles(TRUE);
}

void CMainFrmThread::OnSaveClips()
{
	CClipList *pLocalClips = new CClipList();

	CopyReasonEnum::CopyReason copyReason = CopyReasonEnum::COPY_TO_UNKOWN;

	//Save the clips locally
	{
		ATL::CCritSecLock csLock(m_cs.m_sect);

		POSITION pos;
		CClip* pClip;

		pos = m_saveClips.GetHeadPosition();
		while(pos)
		{
			pClip = m_saveClips.GetNext(pos);
			copyReason = pClip->m_copyReason;
			pLocalClips->AddTail(pClip);
		}

		//pLocalClips now own, the clips
		m_saveClips.RemoveAll();
	}

	Log(_T("SaveCopyClips Before AddToDb")); 

	int count = pLocalClips->AddToDB(true);

	Log(StrF(_T("SaveCopyclips After AddToDb, Count: %d"), count));

	if(count > 0)
	{
		int Id = pLocalClips->GetTail()->m_id;

		Log(StrF(_T("SaveCopyclips After AddToDb, Id: %d Before OnCopyCopyCompleted"), Id));

		theApp.OnCopyCompleted(Id, count, copyReason);

		Log(StrF(_T("SaveCopyclips After AddToDb, Id: %d After OnCopyCopyCompleted"), Id));

		if (pLocalClips->GetTail()->m_copyReason == CopyReasonEnum::COPY_TO_GROUP &&
			CGetSetOptions::GetShowMsgWndOnCopyToGroup())
		{
			CString groupName;
			CppSQLite3Query q = theApp.m_db.execQueryEx(_T("SELECT mText FROM Main WHERE lID = %d"), pLocalClips->GetTail()->m_parentId);
			if (q.eof() == false)
			{
				groupName = q.getStringField(0);
			}

			CString *pMsg = new CString();
			pMsg->Format(_T("Saved new clip \"%s\"\r\ndirectly to the group \"%s\""), pLocalClips->GetTail()->m_Desc.Left(35), groupName);

			theApp.m_pMainFrame->PostMessageW(WM_SHOW_MSG_WINDOW, (WPARAM) pMsg, pLocalClips->GetTail()->m_parentId);
		}
	
	}

	delete pLocalClips;
}

