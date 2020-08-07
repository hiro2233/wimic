
#ifndef WIMICAPP_H
#define WIMICAPP_H

#include <wx/app.h>
#include <wx/cmdline.h>
#include <wx/msgdlg.h>

class wimicDialog;

class wimicApp : public wxApp
{
    public:
        virtual bool OnInit();
        virtual int OnExit();

        wimicDialog *Dlg;

};

DECLARE_APP(wimicApp)

#endif // WIMICAPP_H
