#ifndef collision_H
#define collision_H

#include "TObject.h"
#include "TFile.h"
#include "TString.h"
#include "TH1F.h"
#include "TClonesArray.h"
#include "particle.h"

#define UNIFORM 1
#define KINEMATIC 2
#define FIXED 3
#define GAUSSIAN 4

class collision : public TObject {
public:
   collision();
   collision(const collision &source);
   virtual ~collision();
   collision &operator=(const collision &source); // assignment operator
   void generateCollision(const uint8_t distType); // generates vertex coordinates and multiplicity
   // getters
   void getCoordinates(double *ptc); // get generated vertex
   void getDir(particle *dir);    // particle direction generator
  // setters
   ClassDef(collision, 1)

private:
   // point coordinates
   double _ptc[4]; //[4]
   // distrib params
   double _mx = 0.;
   double _my = 0.;
   double _mz = 0.;
   double _sx = 0.1;
   double _sy = 0.1;
   double _sz = 53.0;
   // kinematic file
   TH1F * _distMult;
   TH1F * _distEta;
};

#endif
