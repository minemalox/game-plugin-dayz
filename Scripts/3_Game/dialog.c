/*
	Example Implementation:
	//Notice: the dialog won't close it self unless you have the option destroyOnResult set to 'true'
				  OR you call delete on the instance you are holding which will call its deconstructor
	--
	//Store dialog in-case we want to change it later based on inputs from user, otherwise its strong referenced so you won't need to worry

	ref CFTDialog dialog = CFTDialog.CreateDialog("This is title", "This is text", "OnCallBack", this);
	
	void OnCallBack(int option)
	{
		Print("Got Callback from CFTDialog -> option: " + typename.EnumToString(CFT_DIAGRESULT, option));
		switch(option)
		{
			case CFT_DIAGRESULT.AGREE:
			NotificationSystem.AddNotificationExtended(3.0, "Title:", "You clicked on AGREE", "set:ccgui_enforce image:Icon40Emergency");
			break;

			case CFT_DIAGRESULT.DISAGREE:
			NotificationSystem.AddNotificationExtended(3.0, "Title:", "You clicked on DISAGREE", "set:ccgui_enforce image:Icon40Emergency");
			break;

			case CFT_DIAGRESULT.LICENSE:
			GetGame().OpenURL("https://www.google.com");
			break;
		}
		delete dialog;
	}
*/

enum GL_DIAGRESULT {
	AGREE 	 = 2,
	DISAGREE = 4,
	LICENSE  = 8,
};

class GameLabsDialog: ScriptedWidgetEventHandler {
	ref Widget 	     	m_Root;
	TextWidget 			m_Caption;
	MultilineTextWidget m_Text;
	ButtonWidget 		m_bAgree;
	ButtonWidget 		m_bDisagree;
	ButtonWidget 		m_bLicense;
	bool 				m_destroyOnResult;

	protected Class 	m_CallBackClass;
	protected string  	m_CallBackFunc;
	private bool 		m_Init;
	private int         m_ResultBitMask;
	
	void OnWidgetScriptInit(Widget w) {
		m_Root 		 = w;
		m_Caption    = TextWidget.Cast(m_Root.FindAnyWidget("Caption"));
		m_Text       = MultilineTextWidget.Cast(m_Root.FindAnyWidget("Text"));
		m_bAgree     = ButtonWidget.Cast(m_Root.FindAnyWidget("bAgree"));
		m_bDisagree  = ButtonWidget.Cast(m_Root.FindAnyWidget("bDisagree"));
		m_bLicense	 = ButtonWidget.Cast(m_Root.FindAnyWidget("bLicense"));
		
		m_Root.SetHandler(this);
		m_Root.SetSort(1023,true); //show-up at the very front
		Print(m_Root);
		m_Init = true;
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(this.DoUpdate);
	}
	
	void ~GameLabsDialog() {
		if (m_Root != NULL) {
			m_Root.Unlink();
		}
		GetGame().GetUIManager().ShowUICursor(false);
		GetUApi().ActivateGroup("menu");
		GetUApi().UpdateControls();
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(this.DoUpdate);
	}

	/*
		Creates dialog on screen, takes its priority to the upmost front of the screen above all UI elements
		Returns instance of CFTDialog
		---
		if callBackClass is left NULL, the function call be will global
	*/
	static GameLabsDialog CreateDialog(string caption, string text, string callBack_fnc, Class callBackClass, bool destroyOnResult = false) {
		if (callBack_fnc == string.Empty || !callBackClass){
			Print("[GameLabsDialog] unable to create dialog! Callback class NULL or function empty!");
			return NULL;
		}

        GameLabsDialog instance;
		Widget dialog = GetGame().GetWorkspace().CreateWidgets("GameLabs/GUI/Assets/cf_license_dialog.layout");
		dialog.GetScript(instance);

		if (instance && instance.IsInitialized()) {
			instance.m_Caption.SetText(caption);
			instance.m_Text.SetText(text);
			instance.SetCallBackFunc(callBack_fnc);
			instance.SetCallBackClass(callBackClass);
			instance.m_destroyOnResult = destroyOnResult;

			Print(string.Format("[GameLabsDialog] CreateDialog(): %1 %2 %3 %4", caption, text, callBack_fnc, callBackClass));
		}
		return instance; 
	}

	void DoUpdate(float tDelta) {
		GetGame().GetUIManager().ShowUICursor(true); //Always force to show mouse cursor
		//Lock controls if player exists
		if (GetGame().GetPlayer()) {
			GetUApi().ActivateExclude("menu");
			GetUApi().UpdateControls();
		}
	}

	bool IsInitialized()
	{
		return m_Init;
	}

	void SetCallBackFunc(string text)
	{
		m_CallBackFunc = text;
	}

	void SetCallBackClass(Class cb)
	{
		m_CallBackClass = cb;
	}

	int GetResultBitMask()
	{
		return m_ResultBitMask;
	}

	void TriggerCallback()
	{
		//Filter options
		typename e = GL_DIAGRESULT;
		for (int bit = 1, i = 0; i < e.GetVariableCount(); i++)
		{
			bit <<= 1;
			if (GetResultBitMask() & bit)
			{
				GetGame().GameScript.CallFunction(m_CallBackClass, m_CallBackFunc, NULL, bit);
			}
		}

		if (m_destroyOnResult){
			delete this; //destroy
		}
	}

	/*
		Notice, this function only works for things like buttons (and checkboxes i think)
		OnMouseButtonDown is for more general "trigger" check for clicks
	*/
	override bool OnClick(Widget w, int x, int y, int button) {
		switch(w)
		{
			case m_bAgree:
				m_ResultBitMask = GL_DIAGRESULT.AGREE;
			break;

			case m_bDisagree:
				m_ResultBitMask = GL_DIAGRESULT.DISAGREE;
			break;

			case m_bLicense:
				m_ResultBitMask = GL_DIAGRESULT.LICENSE;
			break;
		}
		TriggerCallback();
		return true;
	}
};