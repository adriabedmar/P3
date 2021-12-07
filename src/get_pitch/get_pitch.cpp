/// @file

#include <iostream>
#include <fstream>
#include <string.h>
#include <errno.h>

#include "wavfile_mono.h"
#include "pitch_analyzer.h"

#include "docopt.h"

#define FRAME_LEN   0.030 /* 30 ms. */
#define FRAME_SHIFT 0.015 /* 15 ms. */

using namespace std;
using namespace upc;

static const char USAGE[] = R"(
get_pitch - Pitch Detector 

Usage:
    get_pitch [options] <input-wav> <output-txt> [-p <pot>][-r <r1>][-m <rmax>][-c <clip>]
    get_pitch (-h | --help)
    get_pitch --version

Options:
    -h, --help  Show this screen
    --version   Show the version of the project
    -p <pot>       Threshold to classify voiced and unvoiced with power level [default: 42.5]
    -r <r1>        Threshold to classify voiced and unvoiced with r1norm level [default: 0.9]
    -m <rmax>      Threshold to classify voiced and unvoiced with rmaxnorm level [default: 0.45]
    -c <clip>      Threshold for the central clipping [default: 0.015]

Arguments:
    input-wav   Wave file with the audio signal
    output-txt  Output file: ASCII file with the result of the detection:
                    - One line per frame with the estimated f0
                    - If considered unvoiced, f0 must be set to f0 = 0
)";

int main(int argc, const char *argv[]) {
	/// \TODO 
	///  Modify the program syntax and the call to **docopt()** in order to
	///  add options and arguments to the program.
  /// \HECHO
  /// Updated CLI with options for power, r1, rmax and clipping thresholds
    std::map<std::string, docopt::value> args = docopt::docopt(USAGE,
        {argv + 1, argv + argc},	// array of arguments, without the program name
        true,    // show help if requested
        "2.0");  // version string
  
	std::string input_wav = args["<input-wav>"].asString();
	std::string output_txt = args["<output-txt>"].asString();
  float pot_th = std::stof(args["-p"].asString());
  float r1_th =  std::stof(args["-r"].asString());
  float rmax_th = std::stof(args["-m"].asString());
  float clip = std::stof(args["-c"].asString());

  // Read input sound file
  unsigned int rate;
  vector<float> x;
  if (readwav_mono(input_wav, rate, x) != 0) {
    cerr << "Error reading input file " << input_wav << " (" << strerror(errno) << ")\n";
    return -2;
  }

  int n_len = rate * FRAME_LEN;
  int n_shift = rate * FRAME_SHIFT;

  // Define analyzer
  PitchAnalyzer analyzer(n_len, rate, pot_th, r1_th, rmax_th, PitchAnalyzer::HAMMING, 50, 500);

  /// \TODO
  /// Preprocess the input signal in order to ease pitch estimation. For instance,
  /// central-clipping or low pass filtering may be used.
  /// \HECHO
  /// Central clipping
  float max = 0, min = 0, range = 0, abs = 0;

  //Iterate through the whole audio to get the minimum and maximum value to clip the file relative to their values
  vector<float>::iterator iX;
  for(iX = x.begin(); iX < x.end(); iX++){    
    if(*iX > max){
      max = *iX;
    }   
    if(*iX < min){      
      min = *iX;
    }  
  }

  if(-1*(min) > max){  
    range = -1*(min);  
  }else{
    range = max;
  } 
  //Set the clipping range relative to the maximum, with a value of clip form 0 to 1. This value is 0 by default and can be set through the CLI
  clip = range * clip;

  //Eliminate all values lower than the threshold and substract the threshold to the higher values
  for(iX = x.begin(); iX<x.end(); iX++){
    if(*iX<0){
      abs = -1*(*iX);
    }
    else{
      abs = *iX;
    }
    if(abs<clip){      
      *iX = 0;
    }    
    else{
      if(*iX > 0)        
        *iX = *iX - clip;
      else        
        *iX = *iX + clip;    
    } 
  }
  
  vector<float> f0;
  for (iX = x.begin(); iX + n_len < x.end(); iX = iX + n_shift) {
    float f = analyzer(iX, iX + n_len);
    f0.push_back(f);
  }

  /// \TODO
  /// Postprocess the estimation in order to supress errors. For instance, a median filter
  /// or time-warping may be used.

  // Write f0 contour into the output file
  ofstream os(output_txt);
  if (!os.good()) {
    cerr << "Error reading output file " << output_txt << " (" << strerror(errno) << ")\n";
    return -3;
  }

  os << 0 << '\n'; //pitch at t=0
  for (iX = f0.begin(); iX != f0.end(); ++iX) 
    os << *iX << "\n";
  os << 0 << '\n';//pitch at t=Dur

  return 0;
}
