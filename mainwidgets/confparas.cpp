#include "confparas.h"
#include "../mcml/mcml.h"
#include "../mcml/utility_fwd.h"
#include "runresults.h"
#include "ui_confparas.h"
#include <QDebug>
#include <QDialog>
#include <QDoubleValidator>
#include <QIntValidator>
#include <QPainter>
#include <QStyleOption>
#include <QVBoxLayout>
#include <vector>
//#include<thread>
//#include<functional>

ConfParas *ConfParas::theConfParas = nullptr;

ConfParas::ConfParas(QWidget *parent) : QWidget(parent), ui(new Ui::ConfParas) {
  ui->setupUi(this);
  ui->PhoNumSpinBox->setMaximum(9999999);
  ui->PhoNumSlider->setMaximum(9999999);
  ui->PhoNumSlider->setStyleSheet("QSlider::groove:horizontal {  \
                                    border: 1px solid #bbb;  \
                                    background: white;  \
                                    height: 10px;  \
                                    }  \
                                      \
                                    QSlider::sub-page:horizontal {  \
                                    background: qlineargradient(x1: 0, y1: 0,    x2: 0, y2: 1, \
                                        stop: 0 #66e, stop: 1 #bbf);  \
                                    background: qlineargradient(x1: 0, y1: 0.2, x2: 1, y2: 1,  \
                                        stop: 0 #bbf, stop: 1 #55f);  \
                                    border: 1px solid #777;  \
                                    height: 10px;  \
                                    }  \
                                      \
                                    QSlider::add-page:horizontal {  \
                                    background: #fff;  \
                                    border: 1px solid #777;  \
                                    height: 10px;  \
                                    }  \
                                      \
                                    QSlider::handle:horizontal {  \
                                    background: qlineargradient(x1:0, y1:0, x2:1, y2:1, \
                                        stop:0 #eee, stop:1 #ccc);  \
                                    border: 1px solid #777;  \
                                    width: 13px;  \
                                    margin-top: -2px;  \
                                    margin-bottom: -2px;  \
                                    }  \
                                      \
                                    QSlider::handle:horizontal:hover {  \
                                    background: qlineargradient(x1:0, y1:0, x2:1, y2:1,  \
                                        stop:0 #fff, stop:1 #ddd);  \
                                    border: 1px solid #444;  \
                                    }  \
                                      \
                                    QSlider::sub-page:horizontal:disabled {  \
                                    background: #bbb;  \
                                    border-color: #999;  \
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
  QString bground_path = ":/image/image/Photon.jpg";
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
  ui->label->setPalette(font_color);

  ui->Instructor->setAutoFillBackground(true);
  ui->Instructor->setAlignment(Qt::AlignCenter);
  setInstructor();

  ui->progressBar->setFont(QFont("Consolas"));
  ui->progressBar->setValue(0);
  ui->progressBar->setPalette(font_color);
}

ConfParas::~ConfParas() { delete ui; }

void ConfParas::paintEvent(QPaintEvent *) {
  QStyleOption opt;
  opt.init(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void ConfParas::on_SpeLayerButton_clicked() {
  QDialog *SpecifyLayer = new QDialog();
  SpecifyLayer->setWindowTitle("Specify parameters for layers");
  SpecifyLayer->setFixedSize(QSize(220, 150));
  SpecifyLayer->setFont(QFont("Consolas"));
  SpecifyLayer->setWindowModality(Qt::WindowModal);
  SpecifyLayer->setWindowIcon(QIcon(":/image/logo"));
  QPlainTextEdit *LayerEdit = new QPlainTextEdit();
  // QDoubleValidator *doublevalid=new QDoubleValidator();
  // doublevalid->setBottom(0.0);
  QPushButton *Confrim = new QPushButton("Confirm");
  Confrim->setFont(QFont("Consolas"));
  QVBoxLayout *layout = new QVBoxLayout();
  layout->addWidget(LayerEdit);
  layout->addWidget(Confrim);

  connect(Confrim, &QPushButton::clicked, [LayerEdit, this] {
    // use lambda instead of slots
    // lambda syntax is amazing.
    *LayerDatas = LayerEdit->toPlainText().split("\n", QString::SkipEmptyParts);
  });
  connect(Confrim, &QPushButton::clicked, this, &ConfParas::setInstructor);
  connect(Confrim, &QPushButton::clicked, SpecifyLayer, &QDialog::close);

  SpecifyLayer->setLayout(layout);
  SpecifyLayer->show();
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
  LayerDatas->clear();
}

// to check the Layer Datas is valid or not.
bool ConfParas::checkLayerDatas() {
  bool flag;
  for (auto &it : (*LayerDatas)) {
    QStringList tmp = it.split(" ");
    if (tmp.size() != 5)
      return false;
    for (auto &i : tmp) {
      if (i.contains(".")) {
        i = i.toDouble(&flag);
      } else {
        i = i.toInt(&flag, 10);
      }
    }
  }
  return flag;
}

// set up the instructor for showing the layer data status.
void ConfParas::setInstructor() {
  QPalette pa;
  if (LayerDatas->isEmpty()) {
    pa.setColor(QPalette::Background, Qt::red);
    ui->Instructor->setPalette(pa);
    ui->Instructor->setText("Layer paras is empty");
  } else {
    if (checkLayerDatas()) {
      pa.setColor(QPalette::Background, Qt::green);
      ui->Instructor->setPalette(pa);
      ui->Instructor->setText("Valid paras");
    } else {
      pa.setColor(QPalette::Background, Qt::red);
      ui->Instructor->setPalette(pa);
      ui->Instructor->setText("Invalid paras");
    }
  }
}

bool ConfParas::judgeParamsNotEmpty() {
  if (ui->zGridEdit->text().isEmpty() || ui->rGridEdit->text().isEmpty() ||
      ui->No_aGridEdit->text().isEmpty() ||
      ui->No_rGridEdit->text().isEmpty() ||
      ui->No_zGridEdit->text().isEmpty() ||
      ui->MedAboveEdit->text().isEmpty() ||
      ui->MedBelowEdit->text().isEmpty() || LayerDatas->isEmpty()) {
    QDialog *error = new QDialog();
    error->setWindowTitle(QString("error msg"));
    error->setWindowModality(Qt::WindowModal);
    error->setFont(QFont("Consolas"));
    error->setFixedSize(QSize(200, 100));
    error->setWindowIcon(QIcon(":/image/logo"));
    QPlainTextEdit *msg = new QPlainTextEdit();
    msg->setPlainText("Empty param exists!\nPlease re-input!");
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
void ConfParas::readDatas(InputClass &In_Ptr) {
  In_Ptr.input->Wth = WEIGHT;

  In_Ptr.input->num_photons = ui->PhoNumSpinBox->value();
  ui->progressBar->setMinimum(0);
  ui->progressBar->setMaximum(In_Ptr.input->num_photons);
  In_Ptr.input->dz = ui->zGridEdit->text().toDouble();
  In_Ptr.input->dr = ui->rGridEdit->text().toDouble();

  bool ok;
  In_Ptr.input->nz = ui->No_zGridEdit->text().toInt(&ok, 10);
  In_Ptr.input->nr = ui->No_rGridEdit->text().toInt(&ok, 10);
  In_Ptr.input->na = ui->No_aGridEdit->text().toInt(&ok, 10);
  In_Ptr.input->da = 0.5 * PI / In_Ptr.input->na; // indispensable.

  In_Ptr.input->num_layers = LayerDatas->size();

  In_Ptr.input->layerspecs.resize(In_Ptr.input->num_layers + 2);
  // Read the refractive index of the ambient and parameters of layers.
  In_Ptr.input->layerspecs[0].layer->rfct_index =
      ui->MedAboveEdit->text().toDouble();
  double z = 0.0; /* z coordinate of the current layer. */
  for (size_t i = 1; i <= In_Ptr.input->num_layers; ++i) {
    QStringList data = LayerDatas->at(i - 1).split(" ");
    In_Ptr.input->layerspecs[i].layer->rfct_index = data[0].toDouble();
    In_Ptr.input->layerspecs[i].layer->abs_coef = data[1].toDouble();
    In_Ptr.input->layerspecs[i].layer->scat_coef = data[2].toDouble();
    In_Ptr.input->layerspecs[i].layer->anisotropy = data[3].toDouble();
    In_Ptr.input->layerspecs[i].layer->z0 = z;
    z += data[4].toDouble(); // data[4] is thickness
    In_Ptr.input->layerspecs[i].layer->z1 = z;
  }
  In_Ptr.input->layerspecs[In_Ptr.input->num_layers + 1].layer->rfct_index =
      ui->MedBelowEdit->text().toDouble();
  CriticalAngle(In_Ptr.input->num_layers, In_Ptr.input->layerspecs);
}

void ThrowUselessData(QVector<double> &vec) {
  size_t max_valid_index{0};
  size_t size = vec.size();
  for (size_t i = 0; i < size; ++i) {
    if (vec[i] > 0.0001) {
      max_valid_index = i;
    }
  }
  if (max_valid_index < (size - 3))
    vec.resize(max_valid_index + 3);
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
  const long int photon_num = In_Ptr.input->num_photons;
  InitOutputData(In_Ptr, out_parm);
  out_parm.out->spec_reflect = GetSpecularReflection(In_Ptr.input->layerspecs);
  // const size_t thread_count=std::thread::hardware_concurrency()-1;
  const size_t thread_count =
      QThread::idealThreadCount(); // return the ideal number of sub-threads.
  std::atomic<int> flag{0};
  std::vector<Workers *> threads;
  /*
  auto sub_running=[&out_parm,&In_Ptr](const int& len,std::atomic<int>& flag){
      PhotonClass photon;
      for(int i=0;i<len;++i){
          photon.launch(out_parm.out->spec_reflect, In_Ptr.input->layerspecs);
          do {
            photon.hopDropSpin(In_Ptr, out_parm);
          } while (!photon.photon->dead);
          QCoreApplication::processEvents();
          ++flag;
          emit flagChanged();
      }
  };
  */
  size_t each_len = photon_num / thread_count;

  for (size_t i = 0; i < thread_count; ++i) {
    if (i == (thread_count - 1)) {
      Workers *one_photon_run =
          new Workers(out_parm, In_Ptr, flag, (thread_count - i * each_len));
      connect(one_photon_run, &Workers::flagChanged, this,
              &ConfParas::setProgressValue);
      connect(one_photon_run, &Workers::finished, one_photon_run,
              &Workers::deleteLater);
      one_photon_run->start();
      threads.push_back(one_photon_run);
    } else {
      Workers *one_photon_run = new Workers(out_parm, In_Ptr, flag, each_len);
      connect(one_photon_run, &Workers::flagChanged, this,
              &ConfParas::setProgressValue);
      connect(one_photon_run, &Workers::finished, one_photon_run,
              &Workers::deleteLater);
      one_photon_run->start();
      threads.push_back(one_photon_run);
    }
  }
  for (auto thread : threads) {
    thread->wait(); // same as std::thread::join()
  }
  SumScaleResult(In_Ptr, out_parm); // indispensable.
  out_temp = out_parm; // out_temp,in_temp (extern) is declared in runresults.h
  ApplyThrowUselessData(out_temp);
  in_temp = In_Ptr;
  ui->progressBar->setValue(photon_num);
  emit isDone(); // send signal to triggle to open run-results page.
}

void ConfParas::on_RunButton_clicked() {
  if (!judgeParamsNotEmpty())
    return;
  InputClass in_parm;
  readDatas(in_parm);
  doOneRun(in_parm);
}

void ConfParas::setProgressValue(const int &flag) {
  ui->progressBar->setValue(flag);
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
  LayerDatas->push_back(QString("1.37 1 100 0.9 0.1"));
  LayerDatas->push_back(QString("1.37 1 10 0.0 0.1"));
  LayerDatas->push_back(QString("1.37 2 10 0.7 0.2"));
  setInstructor();
  ui->progressBar->setValue(0);
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
  LayerDatas->push_back(QString("1.5 4.3 107 0.79 0.01"));
  LayerDatas->push_back(QString("1.4 2.7 187 0.82 0.02"));
  LayerDatas->push_back(QString("1.4 3.3 192 0.82 0.02"));
  LayerDatas->push_back(QString("1.4 2.7 187 0.82 0.09"));
  LayerDatas->push_back(QString("1.4 2.4 194 0.82 0.06"));
  setInstructor();
  ui->progressBar->setValue(0);
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
  LayerDatas->push_back(QString("1.37 0.2 100 0.9 1.5"));
  LayerDatas->push_back(QString("1.37 0.2 100 0.9 2.0"));
  LayerDatas->push_back(QString("1.37 0.2 100 0.9 0.5"));
  setInstructor();
  ui->progressBar->setValue(0);
}

void Workers::run() {
  PhotonClass photon;
  for (int i = 0; i < len_; ++i) {
    photon.launch(out_.out->spec_reflect, in_.input->layerspecs);
    do {
      photon.hopDropSpin(in_, out_);
    } while (!photon.photon->dead);
    QCoreApplication::processEvents();
    ++flag_;
    emit flagChanged(flag_);
  }
}
