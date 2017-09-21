#pragma once
#ifndef R_MCML_H
#define R_MCML_H

#include "../mainwidgets/confparas.h"
#include "mcml_fwd.h"
#include "mcml_impl.h"
#include "utility_fwd.h"
#include <QVector>
#include <iostream>
#include <memory>

// using std::vector;
// using std::string;
using std::shared_ptr;
using std::weak_ptr;
using std::make_shared;

/* Critical weight for roulette. [Specify when read Datas] */
const double WEIGHT = 1E-4;

const int STANDARDTEST = 0;
/* testing program using fixed rnd seed. */

const double COSZERO = (1.0 - 1.0E-12);
/* cosine of about 1e-6 rad. */

const double COS90D = 1.0E-6;
/* cosine of about 1.57 - 1e-6 rad. */

const double CHANCE = 0.1; /* Chance of roulette survival. */
const double PI = 3.1415926;

// Class used to describe a photon packet.

class PhotonClass {
  friend class ConfParas;

public:
  PhotonClass() : photon(new PhotonStruct) {}
  explicit PhotonClass(const PhotonStruct &item)
      : photon(make_shared<PhotonStruct>(item)) {}
  // forbid some constructors
  PhotonClass(const PhotonClass &) = delete;
  PhotonClass &operator=(const PhotonClass &) = delete;
  ~PhotonClass() = default;

  void
  launch(const double &GetSpecularReflection,
         const QVector<LayerClass> &LayerVec); // Initialize a photon packet.
  void spin(const double &g); // Choose a new direction for photon propagation
  void hop(); // Move the photon s away in the current layer of medium.
  void
  stepSizeInGlass(const InputClass &In); // return the photon step size in glass
  void stepSizeInTissue(const InputClass &In); // Pick a step size for a photon
                                               // packet when it is in tissue.
  bool
  hitBoundary(const InputClass &In); // Check if the step will hit the boundary.
  void drop(const InputClass &In, OutClass &Out); // Drop photon(assumed not
                                                  // dead) weight inside the
                                                  // tissue (not glass).
  void roulette(); // tries to survive a roulette.
  void recordWeightFirstLayer(const double &Refl, const InputClass &In,
                              OutClass &Out); // Record the photon weight
                                              // exiting the first
                                              // layer(dcos_z<0)
  void recordWeightLastLayer(const double &Refl, const InputClass &In,
                             OutClass &Out); // Record the photon weight exiting
                                             // the first layer(dcos_z>0)
  void crossUpOrNot(const InputClass &In,
                    OutClass &Out); // Decide whether the photon will be
                                    // transmitted or reflected on the upper
                                    // boundary(dcos_z<0) of the current layer.
  void crossDownOrNot(const InputClass &In,
                      OutClass &Out); // Decide whether the photon will be
                                      // transmitted or reflected on the bottom
                                      // boundary(dcos_z>0) of the current
                                      // layer.
  void crossOrNot(const InputClass &In, OutClass &Out);
  void hopInGlass(const InputClass &In,
                  OutClass &Out); // Move the photon packet in glass layer.
  void hopDropSpinInTissue(const InputClass &In,
                           OutClass &Out); // Set a step size, move the photon,
                                           // drop some weight, choose a new
                                           // photon direction for propagation.
  void hopDropSpin(const InputClass &In, OutClass &Out);

private:
  shared_ptr<PhotonStruct> photon;
};

void CriticalAngle(const size_t &Layer_num, QVector<LayerClass> &layerspecs);

class LayerClass {
  friend double GetSpecularReflection(const QVector<LayerClass> &LayerVec);
  friend void CriticalAngle(const size_t &Layer_num,
                            QVector<LayerClass> &layerspecs);
  friend size_t IndexToLayer(const size_t &Iz, const InputClass &In_Parm);
  friend class PhotonClass;
  friend class ConfParas;
  friend class InputToString;

public:
  LayerClass() : layer(new LayerStruct) {}
  explicit LayerClass(const LayerStruct &item)
      : layer(make_shared<LayerStruct>(item)) {}
  ~LayerClass() = default;

private:
  shared_ptr<LayerStruct> layer;
};

enum class Types;
class InputClass {
  friend class PhotonClass;
  friend class InputToString;
  friend void Sum2D_DiffReflect(const InputClass &In, OutClass &Out);
  friend size_t IndexToLayer(const size_t &Iz, const InputClass &In_Parm);
  friend void Sum2D_AbsProb(const InputClass &In_Parm, OutClass &Out_Ptr);
  friend void Sum2D_TotalTrans(const InputClass &In_Parm, OutClass &Out_Ptr);
  friend void Scale_DiffReflect_TotalTrans(const InputClass &In_Parm,
                                           OutClass &Out_Ptr);
  friend void Scale_AbsProb(const InputClass &In_Parm, OutClass &Out_Ptr);
  friend void SumScaleResult(const InputClass &In_Parm, OutClass &Out_Ptr);
  // friend void DoOneRun(InputClass* In_Ptr,OutClass& out_parm);
  friend void InitOutputData(const InputClass &In_Parm, OutClass &Out_Ptr);
  friend void Draw1DScatterChart(const QVector<double> &vec,
                                 const QString &name, const Types &type);
  friend void Draw1DSpLineChart(const QVector<double> &vec, const QString &name,
                                const Types &type);
  friend class ConfParas;

public:
  InputClass() : input(new InputStruct) {}
  explicit InputClass(const InputStruct &item)
      : input(make_shared<InputStruct>(item)) {}
  ~InputClass() = default;
  void resetData();

private:
  shared_ptr<InputStruct> input;
};

class OutClass {
  friend class PhotonClass;
  friend class ConfParas;
  friend class RunResults;
  friend class OutputToString;
  friend void Sum2D_DiffReflect(const InputClass &In, OutClass &Out);
  friend void Sum2D_AbsProb(const InputClass &In_Parm, OutClass &Out_Ptr);
  friend void Sum2D_TotalTrans(const InputClass &In_Parm, OutClass &Out_Ptr);
  friend void Scale_DiffReflect_TotalTrans(const InputClass &In_Parm,
                                           OutClass &Out_Ptr);
  friend void Scale_AbsProb(const InputClass &In_Parm, OutClass &Out_Ptr);
  friend void SumScaleResult(const InputClass &In_Parm, OutClass &Out_Ptr);
  // friend void DoOneRun(InputClass* In_Ptr,OutClass& out_parm);
  friend void InitOutputData(const InputClass &In_Parm, OutClass &Out_Ptr);
  friend void ApplyThrowUselessData(OutClass& out);
public:
  OutClass() : out(new OutStruct) {}
  explicit OutClass(const OutStruct &item)
      : out(make_shared<OutStruct>(item)) {}

  OutClass(const OutClass &) = default;
  OutClass &operator=(const OutClass &) = default;
  ~OutClass() = default;

private:
  shared_ptr<OutStruct> out;
};

#endif // !R_MCML_H
