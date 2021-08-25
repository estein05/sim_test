/* COLLIDER EVENT MONTECARLO SIMULATION
 *  Generating particles collisions and intersecting with two
 *  layers of vertex detectors and saving hits to file
 */

 // c++ standard libraries
#include <iostream>
#include <iomanip>

// ROOT libraries
#include "TClonesArray.h"
#include "TStopwatch.h"
#include "TFile.h"
#include "TTree.h"
#include "TMath.h"

// custum classes
#include "lib/detector.h"
#include "lib/collision.h"
#include "lib/hit.h"

#ifndef N_EVENTS
#define N_EVENTS 1e6
#endif

void simulation(bool multScat = false, int randomNoise = 0)
{
   //int        debug = 4;
   TStopwatch watch;
   // creating TTree for simulation
   TFile *simulation = new TFile("simulation.root", "RECREATE");
   TTree *hitTree    = new TTree("hits", "Hits tree");

   detector *det = new detector();

   collision *vtx = new collision();
   double     ptc[4]; // vertex coordinates array
   // instantiating TClonesArrays to store hits
   TClonesArray *hitsL1 = new TClonesArray("hit", 100);
   TClonesArray *hitsL2 = new TClonesArray("hit", 100);
   TClonesArray &L1HITS = *hitsL1;
   TClonesArray &L2HITS = *hitsL2;
   // referring tree branches to arrays
   hitTree->Branch("Vertex", &ptc[0], "x0/D:y0:z0:m");
   hitTree->Branch("L1hit", &hitsL1);
   hitTree->Branch("L2hit", &hitsL2);

   
   //----------------------------- MONTECARLO ---------------------------
   for (int i = 0; i < N_EVENTS; i++) {
      vtx->generateCollision(GAUSSIAN);
      vtx->getCoordinates(ptc);
      //if (debug > 2) printf("\rx: %f\t y: %f\t z: %f\t m: %f \n", ptc[0], ptc[1], ptc[2], ptc[3]);
      // generating particles with a given multiplicity distribution
      particle *part = new particle(); // direction holder
      uint8_t   L1 = 0, L2 = 0;        // number of hits
      for (uint8_t mult = 0; mult < ptc[3]; mult++) {
         vtx->getDir(part); // update the direction
         double beampt[3];  // hit coordinates in cartesian
         det->intersect(ptc, part, beampt, 1000., 30.0, 0, 0, multScat, 0.008);
         double l1pt[3];
         hit *  l1hit = det->intersect(beampt, part, l1pt, 270., 40., 0.012, 0.003, multScat, 0.008);
         if (l1hit != NULL) {
            L1HITS[L1++] = l1hit;
         }
         double l2pt[3];
         hit *  l2hit = det->intersect(l1pt, part, l2pt, 270., 70., 0.012, 0.003, false, 0);
         if (l2hit != NULL) {
            L2HITS[L2++] = l2hit;
         }
         if (l2hit != NULL) {
         //printf("%f,%f  %f,%f,%f\n",l2hit->getTheta(), TMath::ATan(beampt[1] / beampt[0]), (beampt[1] / beampt[0]), (l1pt[1] / l1pt[0]), l2pt[1] / l2pt[0]);
         }
      }
      delete part;
      hitTree->Fill();
      hitsL1->Clear();
      hitsL2->Clear();
      //printf("\nL1 hits: %2d\t L2 hits: %2d\n", L1, L2);

      if (i%10000 == 0) {
        std::system("clear");
        std::cout.width(42);
        std::cout << "MONTECARLO:\nMult. Scattering:\tCollisions:\tRandom Noise" << std::endl;
        std::cout << setiosflags(ios::left)
                  << setw(20) << multScat
                  << resetiosflags(ios::left)
                  << setfill(' ')
                  << setw(10) << i
                  << setw(12) << randomNoise
                  << std::endl;
      }
   }
   std::cout << "\nmontecarlo finished" << std::endl;
   //----------------------------- END MONTECARLO ---------------------------


   // saving simulation file
   simulation->Write("", TObject::kOverwrite);
   simulation->Close();
   hitsL1->Delete();
   hitsL2->Delete();
   delete vtx;
   delete det;

   std::cout  << "\nSimulation run succesful, cpu time: "
              << watch.CpuTime() << "s"
              << std::endl;
   return;
}
