#include "STPulse.cc"
#include "STPulse.hh"
#include "STSamplePoint.cc"
#include "STSamplePoint.hh"

void draw()
{
  std::ifstream list_files("list_pulser");
  //TString file_name;
  //while (list_files >> file_name)
  //for (auto file_name : {"pulser_117ns.dat"})
  for (auto file_name : {"pulser_117ns_50tb.dat"})
  {
    cout << file_name << endl;
    auto pulse = new STPulse(file_name);
    auto f1 = pulse -> GetPulseFunction();
    f1 -> Draw();
  }
}
