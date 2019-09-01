#pragma once
#include "Wnd.h"


class CSceneMenuUI : public mini::ui::CWnd
{

public:
	CSceneMenuUI();
	virtual ~CSceneMenuUI();
	void bind();

private:
	bool oNewScen();
	bool onOpenScene();
	bool onSave();
	bool onSaveAs();
	bool onNewProject();
	bool onOpenProject();
	bool onSaveProject();
	bool onBuildSetting();
	bool onBuildAndRun();
	bool onExit();
private:
	bool init()
	{
		return true;
	}
};

