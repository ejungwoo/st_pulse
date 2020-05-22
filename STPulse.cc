#include "STPulse.hh"
#include "TSystem.h"
#include <fstream>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

STPulse::STPulse(TString fileName)
{
  Initialize(fileName);
}

void STPulse::Initialize(TString fileName)
{
  TString spiritroot = gSystem -> Getenv("VMCWORKDIR");

  fileName = fileName;

  ifstream file(fileName);
  string line;

  while (getline(file, line) && line.find("#") == 0) {}
  istringstream ss(line);
  ss >> fShapingTime;
  ss >> fNumDataPoints >> fStepSize >> fNumAscending >> fNDFTbs;

  if (fNumDataPoints < 20 || fStepSize > 1)
  {
    cout << "*** Error occured while initializing the pulse!" << endl;
    cout << "*** Check file: " << fileName << endl;
    return;
  }

  fPulseData = new STSamplePoint[fNumDataPoints];

  Double_t max = 0;
  for (Int_t iData = 0; iData < fNumDataPoints; iData++)
  {
    getline(file, line);
    if (line.find("#") == 0) {
      iData--;
      continue;
    }

    fPulseData[iData].Init(line);
    Double_t value = fPulseData[iData].fValue;

    if (value > max) {
      max = value;
      fTbAtMax = iData * fStepSize;
    }
  }

  Double_t c = 1/max;
  Double_t valuePre = 0, valueCur = 0;
  fTbAtThreshold = 0;
  fTbAtTail = 0;

  for (Int_t iData = 0; iData < fNumDataPoints; iData++)
  {
    fPulseData[iData].fValue = c * fPulseData[iData].fValue;

    valuePre = valueCur;
    valueCur = fPulseData[iData].fValue;

    if (fTbAtThreshold == 0 && valueCur > fThresholdRatio)
    {
      fTbAtThreshold = iData * fStepSize;
      Int_t next = iData + 1/fStepSize;
      fThresholdTbStep = fPulseData[next].fValue - fPulseData[iData].fValue;
    }

    if (fTbAtTail == 0 && valueCur < valuePre && valueCur < 0.1)
      fTbAtTail = iData * fStepSize;
  }

  file.close();
}

Double_t 
STPulse::PulseF1(Double_t *x, Double_t *par)
{
  Double_t tb = x[0] - par[1];
  if (tb < 0) 
    return 0;

  Int_t tbInStep = tb / fStepSize;
  if (tbInStep > fNumDataPoints - 2)
    return 0;

  Double_t r = (tb / fStepSize) - tbInStep;
  Double_t val = r * fPulseData[tbInStep + 1].fValue + (1 - r) * fPulseData[tbInStep].fValue;

  return par[0] * val;
}

TF1*
STPulse::GetPulseFunction(TString name)
{
  if (name.IsNull()) 
    name = Form("STPulse_%d", fNumF1++);
  TF1* f1 = new TF1(name, this, &STPulse::PulseF1, 0, 512, 2, "STPulse", "PulseF1");
  return f1;
}

   Int_t  STPulse::GetShapingTime()     { return fShapingTime;     }
Double_t  STPulse::GetTbAtThreshold()   { return fTbAtThreshold;   }
Double_t  STPulse::GetTbAtTail()        { return fTbAtTail;        }
Double_t  STPulse::GetTbAtMax()         { return fTbAtMax;         }
   Int_t  STPulse::GetNumAscending()    { return fNumAscending;    }
Double_t  STPulse::GetThresholdTbStep() { return fThresholdTbStep; }
   Int_t  STPulse::GetNumDataPoints()   { return fNumDataPoints;   }
Double_t  STPulse::GetStepSize()        { return fStepSize;        }
