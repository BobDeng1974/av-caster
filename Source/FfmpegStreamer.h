/*
  ==============================================================================

    FfmpegStreamer.h
    Created: 12 Sep 2015 10:26:17am
    Author:  bill

  ==============================================================================
*/

#ifndef FFMPEGSTREAMER_H_INCLUDED
#define FFMPEGSTREAMER_H_INCLUDED

#include "Constants.h"
#include "MainContent.h"


class FfmpegStream : public Thread
{
  friend class FfmpegStreamer ;


public:
  ~FfmpegStream() { this->proc->kill() ; delete this->proc ; }


private:

  FfmpegStream(const String& thread_name) : Thread(thread_name)
  {
    this->proc           = new ChildProcess() ;
    this->proc_out[0]    = '\0' ;
    this->proc_out_lines = StringArray() ;
    this->proc_out_tail  = String::empty ;

    // stream configuration
    this->captureDevices = StringArray() ;
    this->captureDevice  = APP::DEFAULT_CAPTURE_DEVICE ;
    // status display
    this->desktopW     = 0 ;   this->desktopH       = 0 ;
    this->currentFrame = 0 ;   this->currentFps     = 0 ;
    this->currentQ     = 0.0 ; this->currentSize    = String::empty ;
    this->currentTime  = 0.0 ; this->currentBitrate = String::empty ;
  }

  void   run() override ;
  bool   readProcOutput() ;
  int    readProcOutputLines() ;
  bool   isSystemSane() ;
  String findSubstring(  StringArray haystack , String needle) ;
  bool   hasSubstring(   StringArray haystack , String needle) ;
  bool   hasAnySubstring(StringArray haystack , StringArray needles) ;
  bool   areRuntimeErrors() ;


private:

  ChildProcess* proc ;
  char          proc_out[APP::PROC_BUFFER_SIZE] ;
  StringArray   proc_out_lines ;
  String        proc_out_tail ;
  uint16        desktopW ;
  uint16        desktopH ;
  StringArray   captureDevices ;
  String        captureDevice ;
  uint32        currentFrame ;
  uint8         currentFps ;
  float         currentQ ;
  String        currentSize ;
  float         currentTime ;
  String        currentBitrate ;
} ;


class FfmpegStreamer
{
  friend class FfmpegStreamerApplication ;

public:

  // GUI dispatchers
  static void Warning(String message_text) ;
  static void Error(  String message_text) ;


private:

  // setup
  static bool Initialize(MainContent* main_content , const String& args) ;
//   static bool Initialize(JUCEApplication* main_app , MainContent* main_content ,
//                          const String&    args                                 ) ;
  static void Shutdown() ;

  // callbacks and event handlers
  static void HandleTimer(int timer_id) ;
  static void UpdateStatusGUI() ;

//   static JUCEApplication* App ;
  static MainContent*                Gui ;
  static ScopedPointer<FfmpegStream> MuxStream ;
} ;

#endif  // FFMPEGSTREAMER_H_INCLUDED
