#include "history.h"
#include "ui_history.h"
#include<QtSql/QSqlDatabase>
#include<QDebug>
#include<QMessageBox>

History *History::theHistory = nullptr;

QSqlDatabase db=QSqlDatabase::addDatabase("QSQLITE","con_database");

bool DBconnect(const QString& db_name){
    db.setDatabaseName(db_name);
    db.setUserName("ROOT");
    db.setPassword("123456");
    if(!db.open())
        return false;
    return true;
}

void CreateTables(){

}

History::History(QWidget *parent) : QWidget(parent), ui(new Ui::History) {
  ui->setupUi(this);
  if(!DBconnect("database.db"))
      QMessageBox::critical(nullptr, QObject::tr("Connect Error"),
                            QObject::tr("Failed to Connect to Database!"));
}

History::~History()
{
    delete ui;
}

OutputToString::OutputToString(const OutClass &output)
{
    spec_reflect=QString::number(output.out->spec_reflect,'f',5);
    abs_prob=QString::number(output.out->abs_prob,'f',5);
    total_trans=QString::number(output.out->total_trans,'f',5);

    diff_reflect_agl=LinkDataFromVector(output.out->diff_reflect_agl);
    diff_reflect_rdl=LinkDataFromVector(output.out->diff_reflect_rdl);
    abs_prob_z=LinkDataFromVector(output.out->abs_prob_z);
    abs_prob_layer=LinkDataFromVector(output.out->abs_prob_layer);
    total_trans_agl=LinkDataFromVector(output.out->total_trans_agl);
    total_trans_rdl=LinkDataFromVector(output.out->total_trans_rdl);

    diff_reflect_2d=abs_prob_rz=total_trans_2d="";
    for(const auto &item:output.out->diff_reflect_2d){
        diff_reflect_2d+=LinkDataFromVector(item);
    }
    for(const auto &item:output.out->abs_prob_rz){
        abs_prob_rz+=LinkDataFromVector(item);
    }
    for(const auto &item:output.out->total_trans_2d){
        total_trans_2d+=LinkDataFromVector(item);
    }

    setUpAll();
}

void OutputToString::setUpAll()
{
    all="specular reflectance:\n"+spec_reflect+"\n\n"
            +"total absorption probability:\n"+abs_prob+"\n\n"
            +"total transmittance:\n"+total_trans+"\n\n"
            +"1D radial distribution of diffuse reflectance:\n"+diff_reflect_rdl+"\n\n"
            +"1D angular distribution of diffuse reflectance:\n"+diff_reflect_agl+"\n\n"
            +"1D probability density over z:\n"+abs_prob_z+"\n\n"
            +"each layer's absorption probability:\n"+abs_prob_layer+"\n\n"
            +"1D radial distribution of transmittance:\n"+total_trans_rdl+"\n\n"
            +"1D angular distribution of transmittance:\n"+total_trans_agl+"\n\n"
            +"2D distribution of diffuse reflectance:\n"+diff_reflect_2d+"\n\n"
            +"2D probability density in turbid media over r & z:\n"+abs_prob_rz+"\n\n"
            +"2D distribution of total transmittance"+total_trans_2d;
}

InputToString::InputToString(const InputClass &input)
{
    num_photon=QString::number(input.input->num_photons);
    num_layers=QString::number(input.input->num_layers);
    dz=QString::number(input.input->dz,'f',6);
    dr=QString::number(input.input->dr,'f',6);
    da=QString::number(input.input->da,'f',6);
    nz=QString::number(input.input->nz,'f',6);
    nr=QString::number(input.input->nr,'f',6);
    na=QString::number(input.input->na,'f',6);
    layerspecs="";
    for(size_t i=1;i<input.input->num_layers-1;++i){
        layerspecs+=(QString::number(input.input->layerspecs[i].layer->rfct_index,'f',6)+",")
                +(QString::number(input.input->layerspecs[i].layer->abs_coef,'f',6)+",")
                +(QString::number(input.input->layerspecs[i].layer->scat_coef,'f',6)+",")
                +(QString::number(input.input->layerspecs[i].layer->anisotropy,'f',6)+",")
                +"\n";
    }
    setUpAll();

}

void InputToString::setUpAll()
{
    all="number of photons:\n"+num_photon+"\n\n"
            +"delta z/delta r/delta alpha:\n"+dz+","+dr+","+da+"\n\n"
            +"number of z/r/a grid separation:\n"+nz+","+nr+","+na+"\n\n"
            +"number of layers:\n"+num_layers+"\n\n"
            +"refractive index/absorption coefficient/scattering coefficient/anisotropy of each layer:\n"
            +layerspecs;
}
