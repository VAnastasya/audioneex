/*
  Copyright (c) 2014, Alberto Gramaglia

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

*/

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
#include "example1.h"
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
        
        AudioIndexingTask itask (opts.apath);
        IdentificationTask rtask (opts.apath);

        // Get a connection instance to the datastore
        std::shared_ptr<KVDataStore> 
        dstore ( new DATASTORE_T (opts.db_url) );
        dstore->Open( KVDataStore::GET, true, true );
      
        // Create and set up the indexer      
        std::shared_ptr<Indexer> indexer ( Indexer::Create() );
        indexer->SetDataStore( dstore.get() );
        indexer->SetAudioProvider( &itask );
        indexer->SetMatchType( opts.mtype );    
      
        itask.SetFID( opts.FID_base );
        itask.SetDataStore( dstore );
        itask.SetIndexer( indexer );
        itask.Run();      
      
        // Create the identification results parser
        FileIdentificationResultsParser idparser;
        // Create and set up the recognizer
        std::shared_ptr<Recognizer> recognizer ( Recognizer::Create() );
        recognizer->SetDataStore( dstore.get() );
        recognizer->SetMatchType( opts.mtype );
        recognizer->SetMMS( opts.mms );
        recognizer->SetIdentificationType( opts.id_type );
        recognizer->SetIdentificationMode( opts.id_mode );
        recognizer->SetBinaryIdThreshold( opts.b_thresh );
      
        idparser.SetDatastore( dstore );
        idparser.SetRecognizer( recognizer );

        rtask.SetDataStore( dstore );
        rtask.SetRecognizer( recognizer );
        rtask.Connect( &idparser );
        rtask.GetAudioSource()->SetPosition( opts.offset );
        rtask.Run();

        dstore->Close();
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

