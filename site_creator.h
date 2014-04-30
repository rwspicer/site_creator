// -----------------------------------------------------------------------------
// site_creator.h
// Rawser Spicer
// created: 2014/03/26
// modified: 2014/03/27
//
//      this is the header file for the site creator app
//  it cotains the code needed for the app to run. Created using Qt Creator
//  using the Qt framwork for the Gui
//
// -----------------------------------------------------------------------------
#ifndef SITE_CREATOR_H
#define SITE_CREATOR_H

#include <QWidget>
#include <QString>
#include "paramfile.h"
#include "strmap.h"

namespace Ui {
class site_creator;
}



// Class site_creator
//  this is the class for the sit creator program. it controls how all of the
// inputs and outputs are handled
class site_creator : public QWidget
{
    Q_OBJECT

public:
    explicit site_creator(QWidget *parent = 0);
    ~site_creator();

private slots:
    // functions to hadel I/o names gennerated by qt creator
    void on_siteDir_button_clicked();
    void on_siteDir_text_editingFinished();
    void on_createSite_clicked();
    void on_name_text_editingFinished();
    void on_arrays_text_editingFinished();
    void on_badVal_text_editingFinished();
    void on_therm_text_1_editingFinished();
    void on_therm_button_1_clicked();
    void on_therm_text_2_editingFinished();
    void on_therm_button_2_clicked();
    void on_therm_text_3_editingFinished();
    void on_therm_button_3_clicked();
    void on_inDFile_text_editingFinished();
    void on_inDFile_button_clicked();
    void on_keyFile_text_editingFinished();
    void on_keyFile_button_clicked();
    void on_outDir_text_editingFinished();
    void on_outDir_button_clicked();
    void on_qCDir_text_editingFinished();
    void on_qCDir_button_clicked();
    void on_errorDir_button_clicked();
    void on_errorDir_text_editingFinished();
    void on_tableName_text_editingFinished();
    void on_csvSel_box_currentIndexChanged(int index);
    void on_row_text_editingFinished();
    void on_dataType_box_currentIndexChanged(const QString &arg1);
    void on_hName_text_editingFinished();
    void on_hUnits_text_editingFinished();
    void on_hMeas_text_editingFinished();
    void on_coef_text_editingFinished();
    void on_coef_text_2_editingFinished();
    void on_coef_text_3_editingFinished();
    void on_coef_text_4_editingFinished();
    void on_coef_text_5_editingFinished();
    void on_coef_text_6_editingFinished();
    void on_coef_text_7_editingFinished();
    void on_qcLow_text_editingFinished();
    void on_qcHigh_text_editingFinished();
    void on_qcStep_Edit_editingFinished();
    void on_ArrayId_box_currentIndexChanged(const QString &arg1);
    void on_siteDir_lock_toggled(bool checked);
    void on_inDfile_lock_toggled(bool checked);
    void on_arrayId_lock_toggled(bool checked);


private:
    // data members
    Ui::site_creator *ui;

    // data members for file info storage
    QString     _rootDir;
    QString     _siteName;
    QString     _logType;
    QString     _arrays;
    QString     _arrayId;
    QString     _badVal;
    QString     _inDFile;
    QString     _keyFile;
    QString     _outDir;
    QString     _qCDir;
    QString     _errorDir;
    QString     _therm1;
    QString     _therm2;
    QString     _therm3;
    QString     _txtName;
    QString     _csvName;
    QString     _tableName;
    ParamFile * _params;
    CsvRow *    _currRow;
    StrMap *    _dataTypes;
    bool        _datReloadIp;

    // helper functions to set up directory tree and files
    void setRootDir();
    void set_up_dt_list();
    void readDatFile();
    void refresh_file_names();
    void set_up_csv_list();
    void toggle_csv_edit(bool bval);
    bool create_key_txt_file() const;
    void alert(QString Str);

};

#endif // SITE_CREATOR_H
