#include <Riostream.h>
#include "collision.h"
#include "TRandom3.h"
#include "TMath.h"
#include "TString.h"
#include "hit.h"
#include "particle.h"

ClassImp(collision)

collision::collision() : TObject() {}
collision::collision(const collision &source) : TObject(source) {}

collision::~collision() {} // Deconstructor

collision &collision::operator=(const collision &source)
{
   this->~collision();
   new (this) collision(source);
   return *this;
}

void collision::generateCollision(const uint8_t distType)
{
   // generate coordinates
   _ptc[0]       = gRandom->Gaus(_mx, _sx); // x
   _ptc[1]       = gRandom->Gaus(_my, _sy); // y
   _ptc[2]       = gRandom->Gaus(_mz, _sz); // z
   uint16_t mult = 0;
   switch (distType) {
   case FIXED: mult = 40; break;
   case UNIFORM: mult = (uint16_t)(1 + 49 * gRandom->Rndm()); break;
   case GAUSSIAN: mult = gRandom->Gaus(_mmDist, _smDist); break;
   }
   _ptc[3] = mult;
}

// getters
void collision::getCoordinates(double *ptc)
{
   ptc[0] = _ptc[0];
   ptc[1] = _ptc[1];
   ptc[2] = _ptc[2];
   ptc[3] = _ptc[3];
}

void collision::getDir(particle *dir)
{
   double phi   = 2. * TMath::Pi() * gRandom->Rndm();
   double theta = 2. * TMath::ATan(TMath::Exp(-(gRandom->Gaus(-1, 2))));
   dir->setTheta(theta);
   dir->setPhi(phi);
}
