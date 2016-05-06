#include "selector/Selector.h"

using namespace std;



/*
 * Prints out usage and exits using exit 1
 */
void printUsage(char* name)
{
	cout << "Usage: " << name << " [file1] [file2] [file3] [file4]" << endl;
	cout << "The follows names are respected if found" << endl;
	cout << "\t_m for metal channel and will always take the first channel" << endl;
	cout << "\t_s for specular channel and will always appear after metal" << endl;
	cout << "\t_r for roughness channel and will always appear after specular" << endl;
	cout << "\t_a for arbitrary channel and will always appear after roughness" << endl;
	exit(1);
}

/*
 * Entry point
 */
int main(int argc, char **argv)
{
    // Okay so lets stitch some images aye
	vector<boost::filesystem::path> f_paths;

	// Reserve
	f_paths.reserve(4);

    // _m is metal
    // _s is specular
    // _r is roughness
	// _a is arbitrary
	if (argc < 3 || argc > 5) printUsage(argv[0]);


    // Iterate over input looking for matchs
	for (int i = 1; i < argc; i++)
    {
        boost::filesystem::path argPath(argv[i]);
        if (boost::filesystem::exists(argPath)) // Does the file exist?
        {
            if (boost::filesystem::is_regular_file(argPath)) f_paths.push_back(argPath);
			else printUsage(argv[0]);
        }
    }

	// Vector has files
	// Check if any of f_paths begins with _m, if it doesn't add it list and break
	Selector *orderer = new Selector();
	orderer->processInput(f_paths);
	orderer->packImage();
	orderer->saveImage("test.png");


	// Fallback to dumb stitch

    return 0;
}