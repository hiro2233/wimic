
#ifndef WIMICAPP_H
#define WIMICAPP_H

#include <wx/app.h>
#include <wx/cmdline.h>
#include <wx/msgdlg.h>
#include <wx/taskbar.h>
#include <wx/menu.h>
#include "wimicMain.h"

class wimic_TaskBar: public wxTaskBarIcon
{
public:

    enum {
        TSKBR_SHOW = 10001,
        TSKBR_HIDE,
        TSKBR_EXIT,
        TSKBR_START,
        TSKBR_STOP,
        TSKBR_SUBMAIN
    };

#if defined(__WXCOCOA__)
    wimic_TaskBar(wxTaskBarIconType iconType = DEFAULT_TYPE)
    :   wxTaskBarIcon(iconType)
#else
    wimic_TaskBar()
#endif
    ;

    void OnLeftButtonDClick(wxTaskBarIconEvent& event);
    void OnMenuShow(wxCommandEvent& event);
    void OnMenuHide(wxCommandEvent& event);
    void OnMenuExit(wxCommandEvent& event);
    void OnMenuUIStart(wxUpdateUIEvent& event);
    void OnMenuUIStop(wxUpdateUIEvent& event);
    void OnMenuSubStart(wxCommandEvent& event);
    void OnMenuSubStop(wxCommandEvent& event);
    virtual wxMenu *CreatePopupMenu();

private:

    static bool _start_enabled;
    static bool _stop_enabled;
    static bool _show_enabled;

DECLARE_EVENT_TABLE()
};

class wimicApp : public wxApp
{
    public:
        virtual bool OnInit();
        virtual int OnExit();

        wimicDialog *Dlg;

protected:
    wimic_TaskBar   *m_wimic_taskBarIcon;
#if defined(__WXCOCOA__)
    wimic_TaskBar   *m_wimic_dockIcon;
#endif

};

DECLARE_APP(wimicApp);

#endif // WIMICAPP_H
