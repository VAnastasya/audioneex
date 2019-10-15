/*
  Copyright (c) 2014, Alberto Gramaglia

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

*/

///
/// example3
///
/// This example shows how to perform the identification of audio from files.
/// To run the program, use the following command
///
/// example3 [-u <db_url>] [-m <match_type>] [-i <id_type>] [-d <id_mode>] 
///          [-b <b_thresh>] [-s <offset>] <audio_path>
///
/// where <db_url> specifies the location of the database, <match_type> is one 
/// of [MSCALE | XSCALE], <id_type> one of [BINARY | FUZZY], <id_mode> one of 
/// [STRICT | EASY], <b_thresh> the binary identification threshold, <offset> 
/// the starting point within the audio at which to perform the identification 
/// and <audio_path> the path to the directory containing the audio file(s).
///


#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>

#include "ex_common.h"
#include "example3.h"
#include "CmdLineParser.h"

using namespace Audioneex;


void PrintUsage()
{
    std::cout << "\nUSAGE: example3 [-u <db_url>] [-m <match_type>] "
                 "[-i <id_type>] [-d <id_mode>] "
                 "[-b <b_thresh>] [-s <offset>] <audio_path>\n\n";
}


int main(int argc, char** argv)
{
    CmdLineParser cmdLine;
    CmdLineOptions_t opts;

    try
    {
        cmdLine.Parse(argv, argc, opts);

        IdentificationTask itask (opts.apath);

        // Create the identification results parser
        FileIdentificationResultsParser idparser;

        // Get a connection instance to the datastore
        std::shared_ptr<KVDataStore> 
        dstore ( new DATASTORE_T (opts.db_url) );

        dstore->Open( KVDataStore::GET, true, true );

        // Create and set up the recognizer
        std::shared_ptr<Recognizer> 
        recognizer ( Recognizer::Create() );
        recognizer->SetDataStore( dstore.get() );
        recognizer->SetMatchType( opts.mtype );
        recognizer->SetMMS( opts.mms );
        recognizer->SetIdentificationType( opts.id_type );
        recognizer->SetIdentificationMode( opts.id_mode );
        recognizer->SetBinaryIdThreshold( opts.b_thresh );
      
        /*std::cout<<"mtype="<<opts.mtype<<std::endl;
        std::cout<<"mms="<<opts.mms<<std::endl;
        std::cout<<"id_type="<<opts.id_type<<std::endl;
        std::cout<<"id_mode="<<opts.id_mode<<std::endl;
        std::cout<<"b_thresh="<<opts.b_thresh<<std::endl;
        std::cout<<"offset="<<opts.offset<<std::endl;  */    

        idparser.SetDatastore( dstore );
        idparser.SetRecognizer( recognizer );

        itask.SetDataStore( dstore );
        itask.SetRecognizer( recognizer );
        itask.Connect( &idparser );
        itask.GetAudioSource()->SetPosition( opts.offset );
        itask.Run();

        std::cout << "Done" << std::endl;
    }
    catch(const bad_cmd_line_exception &ex)
    {
        std::cerr << "ERROR: " << ex.what() << std::endl;
        PrintUsage();
        return -1;
    }
    catch(const std::exception &ex)
    {
        std::cerr << "ERROR: " << ex.what() << std::endl;
        return -1;
    }

    return 0;
}

