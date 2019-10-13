#include <iostream>

#include "ex_common.h"
#include "example1.h"
#include "CmdLineParser.h"

using namespace Audioneex;

void PrintUsage()
{
    std::cout << "\nUSAGE: example1 [-u <db_url>] [-m <match_type>] "
                                    "<audio_files_dir>\n\n";
}

int main(int argc, char** argv) {
    CmdLineParser cmdLine;
    CmdLineOptions_t opts;
    
    try
    {
        cmdLine.Parse(argv, argc, opts);

        AudioIndexingTask itask (opts.apath);

        std::shared_ptr<KVDataStore>
        dstore ( new DATASTORE_T (opts.db_url) );

        dstore->Open( opts.db_op, true, true );

        std::shared_ptr<Indexer> indexer ( Indexer::Create() );
        indexer->SetDataStore( dstore.get() );
        indexer->SetAudioProvider( &itask );
        indexer->SetMatchType( opts.mtype );        
        indexer->SetCacheLimit( 256 );
        
        indexer->Start();       

        itask.SetFID( opts.FID_base );
        /*itask.SetDataStore( dstore );
        itask.SetIndexer( indexer );
        itask.Run();*/
        
        indexer->End();

        std::cout << opts.apath << std::endl;
        
        dstore->Close();
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
        
    std::cout << "Hello from my project!" << std::endl;
    return 0;    
}
