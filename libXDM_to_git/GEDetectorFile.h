//-------------------------------------------------------------
//
//  GEDetectorFile.h
//
//
//
//-------------------------------------------------------------

#ifndef _GE_DETECTOR_FILE_H
#define _GE_DETECTOR_FILE_H

#include "Raster.h"
#include "boost/multi_array.hpp"
#include <string>

namespace GeneralLib
{
  //-------------------------------------
  //
  // BinarizedGEFile
  //
  //-------------------------------------
  class BinarizedGEFile
  {
  public:
    typedef CRaster<IntensityT, BitMatrix > BinRasterT; 
    typedef float                           GEFloatT;
    typedef boost::multi_array<GEFloatT, 2> FloatMatrix;
    
  private:

    int NumFrames;
    int SizeX;
    int SizeY;


    //------------------------------
    //  Somehow magically each GE detectors have
    //  a different flip order....
    //------------------------------
    bool VerticalFlip;     
    bool HorizontalFlip; 

    std::vector<std::string> FilenameList;
    
    
    
    //-------------------------------------
    //  ReadHeader
    //  Read header of GE files.
    //-------------------------------------
    void ReadHeader();
    
    //-------------------------------------
    // SetFilename
    //-------------------------------------
    void SetFilename();
      
    //-------------------------------------
    //  CreateBackground
    //-------------------------------------
    FloatMatrix CreateBackground();
    
    //-------------------------------------
    //  ReadSingleFrame
    //-------------------------------------
    bool ReadSingleFrame( BinRasterT & Frame  );
    
  };
    

  
} // end GeneralLib

#endif
