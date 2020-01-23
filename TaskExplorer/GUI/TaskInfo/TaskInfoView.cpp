#include "stdafx.h"
#include "TaskInfoView.h"
#include "../TaskExplorer.h"
#include "ProcessView.h"
#include "HandlesView.h"
#include "SocketsView.h"
#include "ThreadsView.h"
#include "ModulesView.h"
#include "MemoryView.h"
#ifdef WIN32
#include "JobView.h"
#include "TokenView.h"
#include "DotNetView.h"
#include "GDIView.h"
#endif
#include "WindowsView.h"
#include "EnvironmentView.h"
#include "../SystemInfo/ServicesView.h"
#include "../SystemInfo/DnsCacheView.h"


CTaskInfoView::CTaskInfoView(bool bAsWindow, QWidget* patent)
: CTabPanel(patent)
{
	setObjectName(bAsWindow ? "TaskWindow" : "TaskPanel");

	InitializeTabs();

	int ActiveTab = theConf->GetValue(objectName() + "/Tabs_Active").toInt();
	QStringList VisibleTabs = theConf->GetStringList(objectName() + "/Tabs_Visible");
	RebuildTabs(ActiveTab, VisibleTabs);

	connect(m_pTabs, SIGNAL(currentChanged(int)), this, SLOT(OnTab(int)));
}


CTaskInfoView::~CTaskInfoView()
{
	int ActiveTab = 0;
	QStringList VisibleTabs;
	SaveTabs(ActiveTab, VisibleTabs);
	theConf->SetValue(objectName() + "/Tabs_Active", ActiveTab);
	theConf->SetValue(objectName() + "/Tabs_Visible", VisibleTabs);
}

void CTaskInfoView::InitializeTabs()
{
	m_pProcessView = new CProcessView(this);
	AddTab(m_pProcessView, tr("General"));

	m_pFilesView = new CHandlesView(3, this);
	AddTab(m_pFilesView, tr("Files"));

	m_pHandlesView = new CHandlesView(0, this);
	AddTab(m_pHandlesView, tr("Handles"));

	m_pSocketsView = new CSocketsView(false, this);
	AddTab(m_pSocketsView, tr("Sockets"));

	m_pThreadsView = new CThreadsView(this);
	AddTab(m_pThreadsView, tr("Threads"));

	m_pModulesView = new CModulesView(false, this);
	AddTab(m_pModulesView, tr("Modules"));

	m_pWindowsView = new CWindowsView(this);
	AddTab(m_pWindowsView, tr("Windows"));

	m_pMemoryView = new CMemoryView(this);
	AddTab(m_pMemoryView, tr("Memory"));

#ifdef WIN32
	m_pTokenView = new CTokenView(this);
	AddTab(m_pTokenView, tr("Token"));

	m_pJobView = new CJobView(this);
	AddTab(m_pJobView, tr("Job"));

	m_pServiceView = new CServicesView(false, this);
	AddTab(m_pServiceView, tr("Service"));

	m_pDotNetView = new CDotNetView(this);
	AddTab(m_pDotNetView, tr(".NET"));

	m_pGDIView = new CGDIView(this);
	AddTab(m_pGDIView, tr("GDI"));
#endif

	//m_pDnsCacheView = new CDnsCacheView(false, this);
	//AddTab(m_pDnsCacheView, tr("Dns Cache"));

	m_pEnvironmentView = new CEnvironmentView(this);
	AddTab(m_pEnvironmentView, tr("Environment"));
}

/*void CTaskInfoView::ShowProcess(const CProcessPtr& pProcess)
{

}*/

void CTaskInfoView::ShowProcesses(const QList<CProcessPtr>& Processes)
{
	if (m_Processes == Processes)
		return;

	m_Processes = Processes;

	OnTab(m_pTabs->currentIndex());
}

void CTaskInfoView::SellectThread(quint64 ThreadId)
{
	m_pTabs->setCurrentWidget(m_pThreadsView);
	m_pThreadsView->SellectThread(ThreadId);
}

void CTaskInfoView::OnTab(int tabIndex)
{
	if (!m_Processes.isEmpty())
	{
		//QMetaObject::invokeMethod(m_pTabs->currentWidget(), "ShowProcess", Qt::AutoConnection, Q_ARG(const CProcessPtr&, m_Processes.first()));
		QMetaObject::invokeMethod(m_pTabs->currentWidget(), "ShowProcesses", Qt::AutoConnection, Q_ARG(const QList<CProcessPtr>&, m_Processes));
	}
}

void CTaskInfoView::Refresh()
{
	QMetaObject::invokeMethod(m_pTabs->currentWidget(), "Refresh", Qt::AutoConnection);
}
