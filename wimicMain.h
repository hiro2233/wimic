
#ifndef WIMICMAIN_H
#define WIMICMAIN_H

//(*Headers(wimicDialog)
#include <wx/dialog.h>
//*)

class wimicDialog: public wxDialog
{
    public:

        wimicDialog(wxWindow* parent,wxWindowID id = -1);
        virtual ~wimicDialog();

    private:

        //(*Handlers(wimicDialog)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        //*)

        //(*Identifiers(wimicDialog)
        //*)

        //(*Declarations(wimicDialog)
        //*)

        DECLARE_EVENT_TABLE()
};

#endif // WIMICMAIN_H
