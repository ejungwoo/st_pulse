#include "STPulse.cc"
#include "STPulse.hh"
#include "STSamplePoint.cc"
#include "STSamplePoint.hh"

void draw()
{
  auto test_amp = 50;
  auto test_pos = 10;

  auto cvs = new TCanvas("cvs","",750,700);
  auto hist = new TH2D("hist",";tb;amp;",100,0,50,100,0,test_amp*1.1);
  hist -> Draw();

  auto line = new TLine(test_pos,0,test_pos,test_amp*1.1);
  line -> SetLineStyle(2);
  line -> SetLineColor(kGray+2);
  line -> Draw("samel");

  const char *titles[] = {"cocktail, 232 ns", "collision, 232 ns", "collision, 117 ns", "collision, 117 ns, longer tail", };

  std::ifstream list_files("list_pulser");
  TString file_name;
  auto legend = new TLegend(0.55,0.5,0.9,0.9);
  Int_t idx = 0;
  while (list_files >> file_name) {
    cout << file_name << endl;
    STPulse *pulse = new STPulse(file_name);
    auto f1 = pulse -> GetPulseFunction();
    f1 -> SetParameters(test_amp,test_pos);
    f1 -> SetNpx(1000);
    f1 -> Draw("samel");
    f1 -> SetLineColor(idx+1);
    legend -> AddEntry(f1,titles[idx++],"l");
  }
  legend -> Draw();
}
