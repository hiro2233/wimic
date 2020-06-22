
#ifndef WIMICMAIN_H
#define WIMICMAIN_H


//(*Headers(wimicDialog)
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/gbsizer.h>
#include <wx/led.h>
#include <wx/notebook.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/timer.h>
//*)

class wimicDialog: public wxDialog
{
    public:

        wimicDialog(wxWindow* parent,wxWindowID id = -1);
        virtual ~wimicDialog();

    private:

        static bool _started;
        static bool _server_started;

        static void *_main_start(void *arg);
        static void *_server_main_start(void *arg);
        void _start_server();
        void _start_client();

        //(*Handlers(wimicDialog)
        void OnQuit(wxCommandEvent& event);
        void Onstart_clientClick(wxCommandEvent& event);
        void Onstop_clientClick(wxCommandEvent& event);
        void Ontimer_connect_statusTrigger(wxTimerEvent& event);
        //*)

        //(*Identifiers(wimicDialog)
        static const long ID_STATICTEXT1;
        static const long ID_LED1;
        static const long ID_BUTTON1;
        static const long ID_BUTTON2;
        static const long ID_BUTTON3;
        static const long ID_PANEL1;
        static const long ID_NOTEBOOK1;
        static const long ID_TIMER1;
        //*)

        //(*Declarations(wimicDialog)
        wxBoxSizer* BoxSizer1;
        wxBoxSizer* BoxSizer2;
        wxBoxSizer* BoxSizer3;
        wxBoxSizer* BoxSizer4;
        wxButton* close;
        wxButton* start_client;
        wxButton* stop_server;
        wxGridBagSizer* GridBagSizer1;
        wxLed* Led1;
        wxNotebook* Notebook1;
        wxPanel* Panel1;
        wxStaticText* lblstatconnection;
        wxTimer timer_connect_status;
        //*)

        DECLARE_EVENT_TABLE()
};

#endif // WIMICMAIN_H
