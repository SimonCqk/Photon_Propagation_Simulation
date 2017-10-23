#include "confparas.h"
#include "../mcml/mcml.h"
#include "../mcml/utility_fwd.h"
#include "runresults.h"
#include "ui_confparas.h"
#include <QDebug>
#include <QDialog>
#include <QDoubleValidator>
#include <QHBoxLayout>
#include <QIntValidator>
#include <QPainter>
#include <QStyleOption>
#include <QVBoxLayout>
#include <QMessageBox>


ConfParas *ConfParas::theConfParas = nullptr;

ConfParas::ConfParas(QWidget *parent) : QWidget(parent), ui(new Ui::ConfParas) {
  ui->setupUi(this);
  ui->PhoNumSpinBox->setMaximum(999999);
  ui->PhoNumSlider->setMaximum(999999);
  ui->PhoNumSlider->setStyleSheet("QSlider::groove:horizontal {  \
                                                         border: 1px solid #4A708B; \
                                                         height: 5px;  \
                                                         border-radius: 1px;  \
                                                         padding-left:-1px;  \
                                                         padding-right:-1px;  \
                                    }  \
                                    QSlider::sub-page:horizontal {  \
                                                          background: qlineargradient(x1:0, y1:0, x2:0, y2:1,\
                                                              stop:0 #B1B1B1, stop:1 #c4c4c4);  \
                                                          background: qlineargradient(x1: 0, y1: 0.2, x2: 1, y2: 1,  \
                                                              stop: 0 #5DCCFF, stop: 1 #1874CD);  \
                                                          border: 1px solid #4A708B;  \
                                                          height: 10px;  \
                                                          border-radius: 2px;  \
                                    }  \
                                      \
                                    QSlider::add-page:horizontal {  \
                                                          background: #999d9c;\
                                                          border: 0px solid #777;\
                                                          height: 10px;  \
                                                          border-radius: 2px;\
                                    }  \
                                      \
                                    QSlider::handle:horizontal {  \
                                                          background: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5,   \
                                                              stop:0.6 #45ADED, stop:0.778409 rgba(255, 255, 255, 255));  \
                                                          width: 11px;  \
                                                          margin-top: -3px; \
                                                          margin-bottom: -3px;\
                                                          border-radius: 5px;  \
                                    }  \
                                      \
                                    QSlider::handle:horizontal:hover {  \
                                                          background: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5, stop:0.6 #2A8BDA,   \
                                                              stop:0.778409 rgba(255, 255, 255, 255));  \
                                                          width: 11px;  \
                                                          margin-top: -3px;\
                                                          margin-bottom: -3px;\
                                                          border-radius: 5px;  \
                                    }  \
                                      \
                                    QSlider::sub-page:horizontal:disabled {  \
                                                          background: #00009C;  \
                                                          border-color: #999;\
                                    }  \
                                      \
                                    QSlider::add-page:horizontal:disabled {  \
                                                          background: #eee;  \
                                                          border-color: #999;  \
                                    }  \
                                      \
                                    QSlider::handle:horizontal:disabled {  \
                                                          background: #eee;  \
                                                          border: 1px solid #aaa;  \
                                                          border-radius: 4px; \
                                    }");
  // set line edits `int` or 'double' input only
  QIntValidator *intvalid = new QIntValidator();
  QDoubleValidator *doublevalid = new QDoubleValidator();
  intvalid->setBottom(1);
  doublevalid->setBottom(0.0);
  ui->zGridEdit->setValidator(doublevalid);
  ui->rGridEdit->setValidator(doublevalid);
  ui->No_aGridEdit->setValidator(intvalid);
  ui->No_rGridEdit->setValidator(intvalid);
  ui->No_zGridEdit->setValidator(intvalid);
  ui->MedAboveEdit->setValidator(doublevalid);
  ui->MedBelowEdit->setValidator(doublevalid);

  this->setAutoFillBackground(true);
  QString bground_path = ":/image/image/bg.jpg";
  QPixmap bground; // set background image
  bground.load(bground_path);
  QPalette palette;
  // set image scaled
  palette.setBrush(this->backgroundRole(),
                   QBrush(bground.scaled(this->size(), Qt::IgnoreAspectRatio,
                                         Qt::SmoothTransformation)));
  this->setPalette(palette);
  QPalette font_color;
  font_color.setColor(QPalette::WindowText, Qt::white);
  ui->PhoNumLabel->setPalette(font_color);
  ui->zrGridLabel->setPalette(font_color);
  ui->No_zra_Label->setPalette(font_color);
  ui->MedLabel->setPalette(font_color);
  font_color.setColor(QPalette::WindowText, QColor(211, 215, 212));
  ui->label->setPalette(font_color);

  ui->progressBar->setFont(QFont("Consolas"));
  ui->progressBar->setValue(0);
  ui->progressBar->setPalette(font_color);

  ui->plainTextEdit->hide();
}

ConfParas::~ConfParas() { delete ui; }

void ConfParas::paintEvent(QPaintEvent *) {
  QStyleOption opt;
  opt.init(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void ConfParas::on_SpeLayerButton_clicked() {
  ui->plainTextEdit->show();
  // clear items added before.
  while (ui->LayersParamsLayout->count() > 1) {
    ui->LayersParamsLayout->removeItem(ui->LayersParamsLayout->takeAt(1));
  }
  int num_layers = ui->num_layer_spin->value();
  QDoubleValidator *doublevalid = new QDoubleValidator();
  QPalette font_color;
  font_color.setColor(QPalette::WindowText, Qt::white);
  for (int i = 1; i <= num_layers; ++i) {
    QVector<QLineEdit *> one_layer_params;
    QHBoxLayout *one_layer_lay = new QHBoxLayout;
    QLabel *layer_i = new QLabel;
    layer_i->setText(QString("Layer#%1:").arg(i));
    layer_i->setFont(QFont("Consolas"));
    layer_i->setPalette(font_color);
    one_layer_lay->addWidget(layer_i);
    for (int j = 0; j < 5; ++j) {
      QLineEdit *param = new QLineEdit;
      param->setValidator(doublevalid);
      param->setFont(QFont("Consolas"));
      one_layer_lay->addWidget(param);
      one_layer_params.append(param);
    }
    ui->LayersParamsLayout->addLayout(one_layer_lay);
    layers_params.append(one_layer_params);
  }
}

void ConfParas::on_ClearButton_clicked() {
  ui->PhoNumSpinBox->clear();
  ui->MedAboveEdit->clear();
  ui->MedBelowEdit->clear();
  ui->No_aGridEdit->clear();
  ui->No_rGridEdit->clear();
  ui->No_zGridEdit->clear();
  ui->rGridEdit->clear();
  ui->zGridEdit->clear();
  for (auto &vec : layers_params) {
    for (auto item : vec) {
      item->clear();
    }
  }
  layers_params.clear();
}

bool ConfParas::judgeParamsNotEmpty() {
  if (ui->zGridEdit->text().isEmpty() || ui->rGridEdit->text().isEmpty() ||
      ui->No_aGridEdit->text().isEmpty() ||
      ui->No_rGridEdit->text().isEmpty() ||
      ui->No_zGridEdit->text().isEmpty() ||
      ui->MedAboveEdit->text().isEmpty() ||
      ui->MedBelowEdit->text().isEmpty() || layers_params.isEmpty()) {
    QDialog *error = new QDialog();
    error->setWindowTitle(QString("error msg"));
    error->setWindowModality(Qt::WindowModal);
    error->setFont(QFont("Consolas"));
    error->setFixedSize(QSize(250, 150));
    error->setWindowIcon(QIcon(":/image/logo"));
    QPlainTextEdit *msg = new QPlainTextEdit();
    msg->setPlainText(" Empty param exists! \n Please re-input!");
    msg->setReadOnly(true);
    QPushButton *Confrim = new QPushButton("Confirm");
    Confrim->setFont(QFont("Consolas"));
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(msg);
    layout->addWidget(Confrim);
    error->setLayout(layout);
    connect(Confrim, &QPushButton::clicked, error, &QDialog::close);
    error->show();
    return false;
  }
  return true;
}

/*
  read data from Configure page to Input Class for running
*/
bool ConfParas::readDatas(InputClass &In_Ptr) {
  In_Ptr.input->Wth = WEIGHT;

  In_Ptr.input->num_photons = ui->PhoNumSpinBox->value();
  ui->progressBar->setMinimum(0);
  bool ok;
  ui->progressBar->setMaximum(In_Ptr.input->num_photons);
  In_Ptr.input->dz = ui->zGridEdit->text().toDouble(&ok);
  In_Ptr.input->dr = ui->rGridEdit->text().toDouble(&ok);

  In_Ptr.input->nz = ui->No_zGridEdit->text().toInt(&ok, 10);
  In_Ptr.input->nr = ui->No_rGridEdit->text().toInt(&ok, 10);
  In_Ptr.input->na = ui->No_aGridEdit->text().toInt(&ok, 10);
  In_Ptr.input->da = 0.5 * PI / In_Ptr.input->na; // indispensable.

  In_Ptr.input->num_layers = ui->num_layer_spin->value();
  if(In_Ptr.input->num_layers > 5){
      QMessageBox::critical(0, QObject::tr("Too many layers"),
                            QString("Number of layers should be less than 5."));
      return false;
  }
  In_Ptr.input->layerspecs.resize(In_Ptr.input->num_layers + 2);
  // Read the refractive index of the ambient and parameters of layers.
  In_Ptr.input->layerspecs[0].layer->rfct_index =
      ui->MedAboveEdit->text().toDouble(&ok);
  double z = 0.0; /* z coordinate of the current layer. */
  for (size_t i = 1; i <= In_Ptr.input->num_layers; ++i) {
    In_Ptr.input->layerspecs[i].layer->rfct_index =
        layers_params[i - 1][0]->text().toDouble(&ok);
    In_Ptr.input->layerspecs[i].layer->abs_coef =
        layers_params[i - 1][1]->text().toDouble(&ok);
    In_Ptr.input->layerspecs[i].layer->scat_coef =
        layers_params[i - 1][2]->text().toDouble(&ok);
    In_Ptr.input->layerspecs[i].layer->anisotropy =
        layers_params[i - 1][3]->text().toDouble(&ok);
    In_Ptr.input->layerspecs[i].layer->z0 = z;
    z += layers_params[i - 1][4]->text().toDouble(&ok);
    ;
    ; // thickness
    In_Ptr.input->layerspecs[i].layer->z1 = z;
  }
  In_Ptr.input->layerspecs[In_Ptr.input->num_layers + 1].layer->rfct_index =
      ui->MedBelowEdit->text().toDouble(&ok);
  CriticalAngle(In_Ptr.input->num_layers, In_Ptr.input->layerspecs);
  return ok;
}

void ThrowUselessData(QVector<double> &vec) {
  size_t max_valid_index{0};
  size_t size = vec.size();
  for (size_t i = 0; i < size; ++i) {
    if (vec[i] > 0.0001) {
      max_valid_index = i;
    }
  }
  if (max_valid_index < (size - 1))
    vec.resize(max_valid_index + 1);
}

void ApplyThrowUselessData(OutClass &out) {
  ThrowUselessData(out.out->abs_prob_z);
  ThrowUselessData(out.out->diff_reflect_agl);
  ThrowUselessData(out.out->diff_reflect_rdl);
  ThrowUselessData(out.out->total_trans_agl);
  ThrowUselessData(out.out->total_trans_rdl);
}

/*
  main running-control function
*/
void ConfParas::doOneRun(InputClass &In_Ptr) {
  OutClass out_parm;
  // total photon packet number.
  long int photons = In_Ptr.input->num_photons;
  InitOutputData(In_Ptr, out_parm);
  PhotonClass photon;
  out_parm.out->spec_reflect = GetSpecularReflection(In_Ptr.input->layerspecs);
  for (int i = 0; i < photons; ++i) {
    photon.launch(out_parm.out->spec_reflect, In_Ptr.input->layerspecs);
    do {
      photon.hopDropSpin(In_Ptr, out_parm);
    } while (!photon.photon->dead);

    ui->progressBar->setValue(i);
    QCoreApplication::processEvents();
  }
  SumScaleResult(In_Ptr, out_parm); // indispensable.
  out_temp = out_parm; // out_temp,in_temp (extern) is declared in runresults.h
  ApplyThrowUselessData(out_temp);
  in_temp = In_Ptr;
  ui->progressBar->setValue(photons);
  emit isDone(); // send signal to triggle to open run-results page.
}

void ConfParas::on_RunButton_clicked() {
  if (!judgeParamsNotEmpty())
    return;
  InputClass in_parm;
  bool ok = readDatas(in_parm);
  if (!ok)
    return;
  doOneRun(in_parm);
}

void ConfParas::setSampleTemplate(
    const int &num_layer, const std::vector<std::vector<double>> &data) {
  ui->plainTextEdit->show();
  // clear items added before.
  while (ui->LayersParamsLayout->count() > 1) {
    ui->LayersParamsLayout->removeItem(ui->LayersParamsLayout->takeAt(1));
  }
  ui->num_layer_spin->setValue(num_layer);
  QPalette font_color;
  font_color.setColor(QPalette::WindowText, Qt::white);
  for (int i = 1; i <= num_layer; ++i) {
    QVector<QLineEdit *> one_layer_params;
    QHBoxLayout *one_layer_lay = new QHBoxLayout;
    QLabel *layer_i = new QLabel;
    layer_i->setText(QString("Layer#%1:").arg(i));
    layer_i->setFont(QFont("Consolas"));
    layer_i->setPalette(font_color);
    one_layer_lay->addWidget(layer_i);
    for (int j = 0; j < 5; ++j) {
      QLineEdit *param = new QLineEdit(QString::number(data[i - 1][j]));
      param->setFont(QFont("Consolas"));
      one_layer_lay->addWidget(param);
      one_layer_params.append(param);
    }
    ui->LayersParamsLayout->addLayout(one_layer_lay);
    layers_params.append(one_layer_params);
  }
  ui->progressBar->setValue(0);
}

void ConfParas::setSampleOneDatas() {
  on_ClearButton_clicked();
  ui->PhoNumSpinBox->setValue(10000);
  ui->zGridEdit->setText(QString("0.01"));
  ui->rGridEdit->setText(QString("0.01"));
  ui->No_zGridEdit->setText(QString("400"));
  ui->No_rGridEdit->setText(QString("500"));
  ui->No_aGridEdit->setText(QString("100"));
  ui->MedAboveEdit->setText(QString("1"));
  ui->MedBelowEdit->setText(QString("1"));

  std::vector<std::vector<double>> data{{1.37, 1, 100, 0.9, 0.1},
                                        {1.37, 1, 10, 0.0, 0.1},
                                        {1.37, 2, 10, 0.7, 0.2}};
  setSampleTemplate(3, data);
}

void ConfParas::setSampleTwoDatas() {
  on_ClearButton_clicked();
  ui->PhoNumSpinBox->setValue(10000);
  ui->zGridEdit->setText(QString("0.01"));
  ui->rGridEdit->setText(QString("0.02"));
  ui->No_zGridEdit->setText(QString("200"));
  ui->No_rGridEdit->setText(QString("500"));
  ui->No_aGridEdit->setText(QString("100"));
  ui->MedAboveEdit->setText(QString("1"));
  ui->MedBelowEdit->setText(QString("1"));

  std::vector<std::vector<double>> data{{1.5, 4.3, 107, 0.79, 0.01},
                                        {1.4, 2.7, 187, 0.82, 0.02},
                                        {1.4, 3.3, 192, 0.82, 0.02},
                                        {1.4, 2.7, 187, 0.82, 0.09},
                                        {1.4, 2.4, 194, 0.82, 0.06}};
  setSampleTemplate(5, data);
}

void ConfParas::setSampleThreeDatas() {
  on_ClearButton_clicked();
  ui->PhoNumSpinBox->setValue(5000);
  ui->zGridEdit->setText(QString("0.01"));
  ui->rGridEdit->setText(QString("0.02"));
  ui->No_zGridEdit->setText(QString("200"));
  ui->No_rGridEdit->setText(QString("300"));
  ui->No_aGridEdit->setText(QString("100"));
  ui->MedAboveEdit->setText(QString("1"));
  ui->MedBelowEdit->setText(QString("1"));

  std::vector<std::vector<double>> data{{1.37, 0.2, 100, 0.9, 1.5},
                                        {1.37, 0.2, 100, 0.9, 2.0},
                                        {1.37, 0.2, 100, 0.9, 0.5}};
  setSampleTemplate(3, data);
}
