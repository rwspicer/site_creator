// -----------------------------------------------------------------------------
// site_creator.cpp
// Rawser Spicer
// created: 2014/03/26
// modified: 2014/03/27
//
//      this is the source file for the site creator app
//  it cotains the code needed for the app to run. Created using Qt Creator
//  using the Qt framwork for the Gui
//
// -----------------------------------------------------------------------------
#include "site_creator.h"
#include "ui_site_creator.h"
#include "csvrow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QString>
#include <QDir>
#include <QFile>
#include <QChar>

#include "paramfile.h"
#include <stdexcept>
#include <sstream>
using std::istringstream;


// constructor
//  builds the widegt and its interface
site_creator::site_creator(QWidget *parent) :
     QWidget(parent), ui(new Ui::site_creator), _siteName(""),
    _badVal("6999"), _therm1("null"), _therm2("null"), _therm3("null"),
    _params(nullptr),_currRow(nullptr),_dataTypes(nullptr), _datReloadIp(false)
{
    ui->setupUi(this);
    ui->badVal_text->setText(_badVal);
    QFile typeFile("data_types.txt");
    if (typeFile.open(QIODevice::ReadOnly))
    {
        QString typeStr(typeFile.readAll());
        typeFile.close();
        istringstream iss(typeStr.toStdString());
        _dataTypes = new StrMap(iss);
    }
    else
    {
        istringstream iss("0::ignore\n1::num\n"
                                       "2::net\n3::precip\n4::therm\n"
                                       "5::thermF\n6::poly\n7::flux\n"
                                       "8::netrad\n9::rt_sensor\n"
                                       "10::tmstmpcol\n11::datey\n"
                                       "12::dated\n13::dateh\n");
        _dataTypes = new StrMap(iss);
    }

    set_up_dt_list();
}

// destructor
site_creator::~site_creator()
{
    delete _params;
    delete ui;
    delete _dataTypes;
}



void site_creator::on_siteDir_button_clicked()
{
    // line to open the file promte
    _rootDir = QFileDialog::getExistingDirectory(this,
                                   tr("Find Files"), QDir::currentPath());


    //code to put it in a varible
    if(! _rootDir.isEmpty())
    {
       if (_rootDir == "") return;
       setRootDir();
    }


}

void site_creator::on_siteDir_text_editingFinished()
{

    _rootDir = ui->siteDir_text->text();
        if (_rootDir == "") return;
    setRootDir();
}



void site_creator::on_createSite_clicked()
{
   /* if (_rootDir == ""){
        alert("Invalid Root Directory");
        return;
    }
    if (_siteName == "")
    {
        alert("please Enter a site Name");
        return;
    }
    if (_logType == "")
    {
        alert("please select a logger type");
        return;
    }
    if (_arrays == ""){
        alert("please enter a number for arrays");
        return;
    }
    if (_inDFile == ""){
        alert("Invalid Input Data File");
        return;
    }
    if (_keyFile == ""){
        alert("Invalid key file");
        return;
    }*/


   // add root dir checking
    QDir rootDir(_rootDir);
    rootDir.mkpath("config");
    rootDir.mkpath("error");
    rootDir.mkpath("outputs");
    rootDir.mkpath("png");
    rootDir.mkpath("qc");
    rootDir.mkpath("raw");
    rootDir.mkpath("web");

    create_key_txt_file();
    _params->setOutName(_csvName);
    _params->write();

    close();
}

void site_creator::on_name_text_editingFinished()
{
    _siteName = ui->name_text->text();
    refresh_file_names();
}




void site_creator::on_arrays_text_editingFinished()
{
    bool ok;
    ui->arrays_text->text().toInt(&ok);

    if (ok){
        _arrays = ui->arrays_text->text();
    } else {
        alert("Arrrays must be a iterger");
    }
}


void site_creator::on_badVal_text_editingFinished()
{
    bool ok;
    ui->badVal_text->text().toInt(&ok);

    if (ok){
        _badVal = ui->badVal_text->text();
    } else {
        alert("Bad Value must be a iterger");
    }
}

void site_creator::on_therm_text_1_editingFinished()
{
    _therm1 = ui->therm_text_1->text();
}

void site_creator::on_therm_button_1_clicked()
{
    _therm1 = QFileDialog::getOpenFileName(this,
                                   tr("Find Files"), QDir::currentPath());

    //code to put it in a varible
    if(! _therm1.isEmpty())
    {
        ui->therm_text_1->setText(_therm1);
    }
}



void site_creator::on_therm_text_2_editingFinished()
{
    _therm2 = ui->therm_text_2->text();
}

void site_creator::on_therm_button_2_clicked()
{
    _therm2 = QFileDialog::getOpenFileName(this,
                                   tr("Find Files"), QDir::currentPath());

    //code to put it in a varible
    if(! _therm2.isEmpty())
    {
        ui->therm_text_2->setText(_therm2);
    }
}

void site_creator::on_therm_text_3_editingFinished()
{
    _therm3 = ui->therm_text_3->text();
}

void site_creator::on_therm_button_3_clicked()
{
    _therm3 = QFileDialog::getOpenFileName(this,
                                   tr("Find Files"), QDir::currentPath());

    //code to put it in a varible
    if(! _therm3.isEmpty())
    {
        ui->therm_text_3->setText(_therm3);
    }
}


void site_creator::on_inDFile_text_editingFinished()
{
    _inDFile = ui->inDFile_text->text();
    readDatFile();
}

void site_creator::on_inDFile_button_clicked()
{
    _inDFile = QFileDialog::getOpenFileName(this,
                                   tr("Find Files"), QDir::currentPath());

    readDatFile();
    //code to put it in a varible
    if(! _inDFile.isEmpty())
    {
        ui->inDFile_text->setText(_inDFile);
    }
}

void site_creator::on_keyFile_text_editingFinished()
{
    _keyFile = ui->keyFile_text->text();
}

void site_creator::on_keyFile_button_clicked()
{
    _keyFile = QFileDialog::getOpenFileName(this,
                                   tr("Find Files"), QDir::currentPath());

    //code to put it in a varible
    if(! _keyFile.isEmpty())
    {
        ui->keyFile_text->setText(_keyFile);
    }
}

void site_creator::on_outDir_text_editingFinished()
{
    _outDir = ui->outDir_text->text();
}

void site_creator::on_outDir_button_clicked()
{
    // line to open the file promte
    _outDir = QFileDialog::getExistingDirectory(this,
                                   tr("Find Files"), QDir::currentPath());

    //code to put it in a varible
    if(! _outDir.isEmpty())
    {
        ui->outDir_text->setText(_outDir);
    }

}

void site_creator::on_qCDir_text_editingFinished()
{
    _qCDir = ui->qCDir_text->text();
}

void site_creator::on_qCDir_button_clicked()
{
    // line to open the file promte
    _qCDir = QFileDialog::getExistingDirectory(this,
                                   tr("Find Files"), QDir::currentPath());

    //code to put it in a varible
    if(! _qCDir.isEmpty())
    {
        ui->qCDir_text->setText(_qCDir);
    }

}

void site_creator::on_errorDir_text_editingFinished()
{
    _errorDir = ui->errorDir_text->text();
}


void site_creator::on_errorDir_button_clicked()
{
    // line to open the file promte
    _errorDir = QFileDialog::getExistingDirectory(this,
                                   tr("Find Files"), QDir::currentPath());

    //code to put it in a varible
    if(! _errorDir.isEmpty())
    {
        ui->errorDir_text->setText(_errorDir);
    }

}


void site_creator::on_tableName_text_editingFinished()
{
    _tableName = ui->tableName_text->text();
    refresh_file_names();
}


void site_creator::on_csvSel_box_currentIndexChanged(int index)
{
    if (index == 0 || _datReloadIp)
    {
        toggle_csv_edit(false);
        return;
    }
    toggle_csv_edit(true);
    int pos = index-1;
    _currRow = &((*_params)[pos]);
   // auto temp = _currRow->array_pos.toInt();

    ui->pos_edit->setText(_currRow->array_pos);
    ui->row_text->setText(_currRow->d_element);
    ui->coef_text->setText(_currRow->Coef1);
    ui->coef_text_2->setText(_currRow->Coef2);
    ui->coef_text_3->setText(_currRow->Coef3);
    ui->coef_text_4->setText(_currRow->Coef4);
    ui->coef_text_5->setText(_currRow->Coef5);
    ui->coef_text_6->setText(_currRow->Coef6);
    ui->coef_text_7->setText(_currRow->Coef7);
    try {
        ui->dataType_box->setCurrentIndex(QString((_dataTypes->
                    keyOf(_currRow->Data_type.toStdString())).c_str()).toInt());

    } catch (...){
        ui->dataType_box->setCurrentIndex(0);
        _currRow->Data_type = _dataTypes->at("0").c_str();
    }

    ui->hMeas_text->setText(_currRow->out_tpye);
    ui->hName_text->setText(_currRow->out_name);
    ui->hUnits_text->setText(_currRow->out_units);
    ui->qcHigh_text->setText(_currRow->QC_high);
    ui->qcLow_text->setText(_currRow->QC_low);
    ui->qcStep_Edit->setText(_currRow->QC_step);

}

void site_creator::on_row_text_editingFinished()
{
    _currRow->d_element = ui->row_text->text();
}

void site_creator::on_dataType_box_currentIndexChanged(const QString &arg1)
{
    if (_currRow != nullptr)
    {
        toggle_csv_edit(true);
        if("ignore" == arg1 || "tmstmpcol" == arg1 || "datey" == arg1
                || "dated" == arg1 || "dateh" == arg1)
        {
            ui->coef_text->setEnabled(false);
            ui->coef_text_2->setEnabled(false);
            ui->coef_text_3->setEnabled(false);
            ui->coef_text_4->setEnabled(false);
            ui->coef_text_5->setEnabled(false);
            ui->coef_text_6->setEnabled(false);
            ui->coef_text_7->setEnabled(false);
            ui->qcHigh_text->setEnabled(false);
            ui->qcLow_text->setEnabled(false);
            ui->qcStep_Edit->setEnabled(false);
            ui->hMeas_text->setText("ignore");
            ui->hUnits_text->setText("ignore");
            ui->hMeas_text->setEnabled(false);
            ui->hUnits_text->setEnabled(false);

        }
        else if("num" == arg1 || "net" == arg1 || "precip" == arg1)
        {
            ui->coef_text->setEnabled(false);
            ui->coef_text_2->setEnabled(false);
            ui->coef_text_3->setEnabled(false);
            ui->coef_text_4->setEnabled(false);
            ui->coef_text_5->setEnabled(false);
            ui->coef_text_6->setEnabled(false);
            ui->coef_text_7->setEnabled(false);
        }
        else if("flux" == arg1)
        {
            ui->coef_text_3->setEnabled(false);
            ui->coef_text_4->setEnabled(false);
            ui->coef_text_5->setEnabled(false);
            ui->coef_text_6->setEnabled(false);
            ui->coef_text_7->setEnabled(false);
        }
        else if ("netrad" == arg1)
        {
            ui->coef_text_4->setEnabled(false);
            ui->coef_text_5->setEnabled(false);
            ui->coef_text_6->setEnabled(false);
            ui->coef_text_7->setEnabled(false);
        }
        else if ("therm" == arg1 || "thermF" == arg1)
        {
            ui->coef_text_5->setEnabled(false);
            ui->coef_text_6->setEnabled(false);
            ui->coef_text_7->setEnabled(false);
        }
        // else enable all



        _currRow->Data_type = arg1;
    }
}



void site_creator::on_hName_text_editingFinished()
{
    _currRow->out_name = ui->hName_text->text();
}

void site_creator::on_hUnits_text_editingFinished()
{
    _currRow->out_units = ui->hUnits_text->text();
}

void site_creator::on_hMeas_text_editingFinished()
{
    _currRow->out_tpye = ui->hMeas_text->text();
}

void site_creator::on_coef_text_editingFinished()
{
    auto temp = ui->coef_text->text();
    bool b;
    temp.toFloat(&b);
    if (!b){
        temp = "0";
        ui->coef_text->setText(temp);
    }
    _currRow->Coef1 = temp;
}

void site_creator::on_coef_text_2_editingFinished()
{
    auto temp = ui->coef_text_2->text();
    bool b;
    temp.toFloat(&b);
    if (!b){
        temp = "0";
        ui->coef_text_2->setText(temp);
    }
    _currRow->Coef2 = temp;
}

void site_creator::on_coef_text_3_editingFinished()
{
    auto temp = ui->coef_text_3->text();
    bool b;
    temp.toFloat(&b);
    if (!b){
        temp = "0";
        ui->coef_text_3->setText(temp);
    }
    _currRow->Coef3= temp;
}

void site_creator::on_coef_text_4_editingFinished()
{
    auto temp = ui->coef_text_4->text();
    bool b;
    temp.toFloat(&b);
    if (!b){
        temp = "0";
        ui->coef_text_4->setText(temp);
    }
    _currRow->Coef4 = temp;
}

void site_creator::on_coef_text_5_editingFinished()
{
    auto temp = ui->coef_text_5->text();
    bool b;
    temp.toFloat(&b);
    if (!b){
        temp = "0";
        ui->coef_text_5->setText(temp);
    }
    _currRow->Coef5 = temp;
}

void site_creator::on_coef_text_6_editingFinished()
{
    auto temp = ui->coef_text_6->text();
    bool b;
    temp.toFloat(&b);
    if (!b){
        temp = "0";
        ui->coef_text_6->setText(temp);
    }
    _currRow->Coef6 = temp;
}

void site_creator::on_coef_text_7_editingFinished()
{
    auto temp = ui->coef_text_7->text();
    bool b;
    temp.toFloat(&b);
    if (!b){
        temp = "0";
        ui->coef_text_7->setText(temp);
    }
    _currRow->Coef7 = temp;
}

void site_creator::on_qcLow_text_editingFinished()
{
    auto temp = ui->qcLow_text->text();
    bool b;
    temp.toFloat(&b);
    if (!b){
        temp = "0";
        ui->qcLow_text->setText(temp);
    }
    _currRow->QC_low = temp;
}

void site_creator::on_qcHigh_text_editingFinished()
{
    auto temp = ui->qcHigh_text->text();
    bool b;
    temp.toFloat(&b);
    if (!b){
        temp = "0";
        ui->qcHigh_text->setText(temp);
    }
    _currRow->QC_high = temp;
}

void site_creator::on_qcStep_Edit_editingFinished()
{
    auto temp = ui->qcStep_Edit->text();
    bool b;
    temp.toFloat(&b);
    if (!b){
        temp = "0";
        ui->qcStep_Edit->setText(temp);
    }
    _currRow->QC_step = temp;
}

void site_creator::on_ArrayId_box_currentIndexChanged(const QString &arg1)
{
    _params->setParamSet(arg1.toInt());
    set_up_csv_list();
    _arrays = std::to_string(_params->size()).c_str();
    ui->arrays_text->setText(_arrays);
    _arrayId = _tableName = _params->getTag();
    ui->tableName_text->setText(_tableName);
    refresh_file_names();
}

void site_creator::on_siteDir_lock_toggled(bool checked)
{
    ui->siteDir_button->setEnabled(!checked);
    ui->siteDir_text->setEnabled(!checked);
}

void site_creator::on_inDfile_lock_toggled(bool checked)
{
    ui->inDFile_button->setEnabled(!checked);
    ui->inDFile_text->setEnabled(!checked);
}

void site_creator::on_arrayId_lock_toggled(bool checked)
{
    ui->ArrayId_box->setEnabled(!checked);
}


//setRootDir
//  sets the root and sub directories also enables feilds that require these
//  precoditions
//      none
// postconditions
//      the rootDir as well as sub dir values will be set, and the feilds that
//  require them will be enabled
void site_creator::setRootDir()
{
    ui->siteDir_text->setText(_rootDir);
    ui->siteDir_lock->setEnabled(true);
    ui->siteDir_lock->setChecked(true);

    _outDir = _rootDir + "/outputs/";
    ui->outDir_text->setText(_outDir);
    _qCDir = _rootDir + "/qc/";
    ui->qCDir_text->setText(_qCDir);
    _errorDir = _rootDir + "/error/";
    ui->errorDir_text->setText(_errorDir);

    ui->outDir_button->setEnabled(true);
    ui->outDir_text->setEnabled(true);

    ui->qCDir_button->setEnabled(true);
    ui->qCDir_text->setEnabled(true);

    ui->errorDir_button->setEnabled(true);
    ui->errorDir_text->setEnabled(true);

    ui->inDFile_text->setEnabled(true);
    ui->inDFile_button->setEnabled(true);

    ui->therm_text_1->setEnabled(true);
    ui->therm_button_1->setEnabled(true);

    ui->therm_text_2->setEnabled(true);
    ui->therm_button_2->setEnabled(true);

    ui->therm_text_3->setEnabled(true);
    ui->therm_button_3->setEnabled(true);

    refresh_file_names();
}

//setup_dt_list
// the list of datatyps will be set to values in _datatypes
// preconditions:
//   _dataTypes must be initialized
//   the dat type list should be empty
// postconditions:
//  the datatype list will have values
void site_creator::set_up_dt_list()
{
    int index = 0;
    while (true){
        try{
            ui->dataType_box->addItem((_dataTypes->
                                      at(std::to_string(index)).c_str()));
        }
        catch (...){
            break;
        }
        ++index;
    }
}


//readDatFile
//  reads the data file provided
//  preconditions
//      none
// postconditions
//      vairiables will be set based on the type of file read
void site_creator::readDatFile()
{
    if(_inDFile == "")
        return;
    try{
        if (_params != nullptr){
            ui->ArrayId_box->clear();
            ui->ArrayId_box->setEnabled(false);
            ui->csvSel_box->setEnabled(false);
        }
        delete _params;
        _params = new ParamFile("outName",_inDFile);
    }
    catch(const std::invalid_argument & ia)
    {
        alert(ia.what());
        _inDFile = "";
        _params = nullptr;
        ui->inDFile_text->setText(_inDFile);
        return;
    }


    _tableName = _params->getTag();
    _arrays = std::to_string(_params->size()).c_str();
    ui->arrays_text->setText(_arrays);
    refresh_file_names();
    ui->tableName_text->setText(_tableName);
    set_up_csv_list();
    _logType = _params->getType();
    if (_logType == "Table"){
        _arrayId = "-9999";
        ui->ArrayId_box->setEnabled(false);
        ui->arrayId_lock->setEnabled(false);
        ui->arrayId_lock->setChecked(false);
    } else if (_logType == "Array"){
        //_arrayId = "-9999";
        auto ids = _params->getIds();
        for (auto id : ids)
        {
            ui->ArrayId_box->addItem(std::to_string(id).c_str());
        }
        ui->ArrayId_box->setEnabled(true);
        ui->arrayId_lock->setEnabled(true);
        ui->arrayId_lock->setChecked(false);
    } else {
        _logType = "";
        _arrayId = "-9999";
        ui->ArrayId_box->setEnabled(false);
        ui->arrayId_lock->setEnabled(false);
    }
    ui->logType_text->setText(_logType);
    ui->name_text->setEnabled(true);
    ui->tableName_text->setEnabled(true);
    //ui->logTyp_sel->setEnabled(true);
    ui->badVal_text->setEnabled(true);
    ui->keyFile_text->setEnabled(true);
    ui->keyFile_button->setEnabled(true);

    ui->csvSel_box->setEnabled(true);

    ui->createSite->setEnabled(true);
    refresh_file_names();
    ui->inDfile_lock->setEnabled(true);
    ui->inDfile_lock->setChecked(true);

}


// refresh_file_names
//  refreshes the file names based on the current values of class member vaibles
// preconditions
//      none
// postconditions
//      the file name will be uptodate based on current values
void site_creator::refresh_file_names()
{
    QString siteName = "";

    if(!_rootDir.isEmpty()){
       auto strItr = _rootDir.end();
        if (*(strItr - 1) == '/')
            strItr -= 1;
        strItr -= 1;

        while(true)
        {
            if (*(strItr) == '/')
                break;
            siteName = *strItr + siteName;
            strItr--;
        }
    }

    if(_siteName == "")
    {
        _siteName = siteName;
        ui->name_text->setText(_siteName);
    }

    QString tag = _tableName ;



    _txtName = _rootDir + "/config/" + siteName + "_"+ tag +"_key.txt";
    _keyFile = _csvName = _rootDir + "/config/" + siteName + "_"+ tag + "_params_" +
            _arrays + ".csv";

     ui->keyFile_text->setText(_txtName);

}

// set_up_csv_list
//  sets up the list of editiable csv rows
//  preconditions:
//      _params must be set
// postconditions
//      the csv list will be populated
void site_creator::set_up_csv_list()
{
    if(ui->csvSel_box->count() > 1){
        _datReloadIp = true;
        ui->csvSel_box->clear();
        ui->csvSel_box->addItem("select");
        _datReloadIp = false;
    }
    for(std::size_t i = 0; i< _params->size(); ++i)
    {
        ui->csvSel_box->addItem((*_params)[i].d_element);
    }
}


//toggle_csv_edit
// sets the csvEdit feilds to endabed or disabeled
//  preconditions
//      bval is a bool
//  postconditions:
//      feilds enabeld if bval is true, disabled otherwise
void site_creator::toggle_csv_edit(bool bval)
{
    ui->coef_text->setEnabled(bval);
    ui->coef_text_2->setEnabled(bval);
    ui->coef_text_3->setEnabled(bval);
    ui->coef_text_4->setEnabled(bval);
    ui->coef_text_5->setEnabled(bval);
    ui->coef_text_6->setEnabled(bval);
    ui->coef_text_7->setEnabled(bval);
    ui->row_text->setEnabled(bval);
    ui->dataType_box->setEnabled(bval);
    ui->qcHigh_text->setEnabled(bval);
    ui->qcLow_text->setEnabled(bval);
    ui->qcStep_Edit->setEnabled(bval);
    ui->hName_text->setEnabled(bval);
    ui->hUnits_text->setEnabled(bval);
    ui->hMeas_text->setEnabled(bval);

}


//Createt_key_text_file
//  writes the key text file from the datain the .dat file
//
// preconditions:
//      all members used in this function must have some value
//  postcoditions
//      writes a .txt file
bool site_creator::create_key_txt_file() const
{


    QFile keyFile(_txtName);
    keyFile.open( QIODevice::WriteOnly);
    keyFile.write("[main]\n");
    keyFile.write("# general stuff like:\n");
    keyFile.write("# 1) the name of the station\n");
    keyFile.write("# 2) the type of data logger choices: CR10X or CR1000\n");
    keyFile.write("# 3) number of arrays on the line (so incomplete lines are");
    keyFile.write("ignored and so program changes, which might involve same");
    keyFile.write(" array ID but different number of columns are ignored\n");
    keyFile.write("# 4) the array ID to look for in the input file\n");


    keyFile.write("station_name = ");
    keyFile.write(_siteName.toStdString().c_str());
    keyFile.write(" -- ");
    keyFile.write(_tableName.toStdString().c_str());

    keyFile.write("\nlogger_type = ");
    keyFile.write(_logType.toStdString().c_str());

    keyFile.write("\narrays = ");
    keyFile.write(_arrays.toStdString().c_str());

    keyFile.write("\narray_id = ");
    keyFile.write(_arrayId.toStdString().c_str());

    keyFile.write("\n\n");
    keyFile.write("# input files\n");
    keyFile.write("# 1) the raw data\n");
    keyFile.write("# 2) another key file containng array based parameters\n\n");

    keyFile.write("input_data_file = file://");
    keyFile.write(_inDFile.toStdString().c_str());
    keyFile.write("\narray_based_params_key_file = ");
    keyFile.write(_keyFile.toStdString().c_str());

    keyFile.write("\n\n# Working Directories\n");
    keyFile.write("output_dir = ");
    keyFile.write(_outDir.toStdString().c_str());
    keyFile.write("\nqc_log_dir = ");
    keyFile.write(_qCDir.toStdString().c_str());
    keyFile.write("\nerror_log_dir = ");
    keyFile.write(_errorDir.toStdString().c_str());

    keyFile.write("\n\ntherm1 = ");
    keyFile.write(_therm1.toStdString().c_str());
    keyFile.write("\ntherm2 = ");
    keyFile.write(_therm2.toStdString().c_str());
    keyFile.write("\ntherm1 = ");
    keyFile.write(_therm3.toStdString().c_str());

    keyFile.write("\n\n# bad data Value\n");
    keyFile.write("bad_data_val = ");
    keyFile.write(_badVal.toStdString().c_str());

    return true;
}




// void alert(QString Str )
//  this funtion posts the string it is passed to a message box
// it was created for testing purposes
// input:
//      Str: a QString
// returns nothing
void  site_creator::alert(QString Str)
{
     QMessageBox msgBox;
     msgBox.setWindowTitle("Alert");
     msgBox.setText( Str );
     msgBox.exec();
}



