#include "stdafx.h"
#include "SceneMenuUI.h"

CSceneMenuUI::CSceneMenuUI()
{
}


CSceneMenuUI::~CSceneMenuUI()
{
}

void CSceneMenuUI::bind()
{
	BindChildMessage(mini::LEFTDOWN, "menu", "NewScen", &CSceneMenuUI::oNewScen);
	BindChildMessage(mini::LEFTDOWN, "menu", "OpenScene", &CSceneMenuUI::onOpenScene);
	BindChildMessage(mini::LEFTDOWN, "menu", "Save", &CSceneMenuUI::onSave);
	BindChildMessage(mini::LEFTDOWN, "menu", "SaveAs", &CSceneMenuUI::onSaveAs);
	BindChildMessage(mini::LEFTDOWN, "menu", "NewProject", &CSceneMenuUI::onNewProject);
	BindChildMessage(mini::LEFTDOWN, "menu", "OpenProject", &CSceneMenuUI::onOpenProject);
	BindChildMessage(mini::LEFTDOWN, "menu", "SaveProject", &CSceneMenuUI::onSaveProject);
	BindChildMessage(mini::LEFTDOWN, "menu", "BuildSetting", &CSceneMenuUI::onBuildSetting);
	BindChildMessage(mini::LEFTDOWN, "menu", "BuildAndRun", &CSceneMenuUI::onBuildAndRun);
	BindChildMessage(mini::LEFTDOWN, "menu", "Exit", &CSceneMenuUI::onExit);
}

bool CSceneMenuUI::oNewScen()
{
	return true;
}

bool CSceneMenuUI::onOpenScene()
{
	return true;
}

bool CSceneMenuUI::onSave()
{
	return true;
}

bool CSceneMenuUI::onSaveAs()
{
	return true;
}

bool CSceneMenuUI::onNewProject()
{
	return true;

}

bool CSceneMenuUI::onOpenProject()
{
	return true;
}

bool CSceneMenuUI::onSaveProject()
{
	return true;
}

bool CSceneMenuUI::onBuildSetting()
{
	return true;
}

bool CSceneMenuUI::onBuildAndRun()
{
	return true;
}

bool CSceneMenuUI::onExit()
{
	return true;
}

