
#ifndef WIMICMAIN_H
#define WIMICMAIN_H


//(*Headers(wimicDialog)
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/gbsizer.h>
#include <wx/notebook.h>
#include <wx/panel.h>
#include <wx/sizer.h>
//*)

class wimicDialog: public wxDialog
{
    public:

        wimicDialog(wxWindow* parent,wxWindowID id = -1);
        virtual ~wimicDialog();

    private:

        static bool _started;


        static void *_main_start(void *arg);

        //(*Handlers(wimicDialog)
        void OnQuit(wxCommandEvent& event);
        void Onstart_serverClick(wxCommandEvent& event);
        void Onstop_serverClick(wxCommandEvent& event);
        //*)

        //(*Identifiers(wimicDialog)
        static const long ID_PANEL3;
        static const long ID_PANEL2;
        static const long ID_BUTTON1;
        static const long ID_BUTTON2;
        static const long ID_BUTTON3;
        static const long ID_PANEL1;
        static const long ID_NOTEBOOK1;
        //*)

        //(*Declarations(wimicDialog)
        wxBoxSizer* BoxSizer1;
        wxBoxSizer* BoxSizer2;
        wxBoxSizer* BoxSizer3;
        wxBoxSizer* BoxSizer4;
        wxButton* close;
        wxButton* start_server;
        wxButton* stop_server;
        wxGridBagSizer* GridBagSizer1;
        wxPanel* Panel1;
        wxPanel* Panel2;
        wxPanel* Panel3;
        //*)

        DECLARE_EVENT_TABLE()
};

#endif // WIMICMAIN_H
