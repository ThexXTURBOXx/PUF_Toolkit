#include "PUF_File.h"

/*
 * PUF-ToolKit - File
 *
 * Author: Sebastian Schurig
 *
 * File-System Functions for the PUF-ToolKit
 *
 */


bool IsFile(const char* input)
/*
 * Check if the input is a file */
{
    struct stat test;
    stat(input, &test);
    return S_ISREG(test.st_mode);
}

bool IsDir(const char* input)
/*
 * Check if the input is a directory */
{
    struct stat test;
    stat(input, &test);
    return S_ISDIR(test.st_mode);
}

int ViewFile(struct Item *item, int option)
/*
 * View a text file */
{
    string text;
    ifstream view;
    char buf[2];
    unsigned int ch;
    char *p;

    view.open(item->output_file_name);

    if(view.fail()) return 11;

		// Output of the file content
        while(!view.eof())
        {
            getline(view,text);
            cout << "" << text << endl;
        }

    view.close();

	// Show output until 'return' is pressed, workaround to skip all other input
    cout << "To continue press 'return'";
    if (fgets(buf, sizeof(buf), stdin)) {
        /* fgets succeeds, scan for newline character */
        p = strchr(buf, '\n');
        if (p) {
        *p = '\0';
        return 0;
        }
        else {
            /* newline not found, flush stdin to end of line */
            while (((ch = getchar()) != '\n')
            && !feof(stdin)
            && !ferror(stdin)
            );
            return 0;
        }
    }
    else {
        cin.clear();
        return 0;
    }
	return 0;
}

int SaveFile(struct Item *item, int option)
/*
 * Save the result of the calculation into a text file
 *
 * Inputs:
 * item = pointer to the struct with necessary informations
 * option = definition of the output format
 *
 * Options:
 * 1 = output for the Hamming Weight
 * 2 = output for the (Shannon) Entropy
 * 3 = output for the Min-Entropy (CTW)
 */
{
    ofstream file;

    DefineFilename(item, 2);

    file.open (item->output_file_name);

    if(file.fail()) return 11;

    if (file.is_open())
      {
                        file << "*******************************************************************************" << endl;
                        file << "                                                                               " << endl;
        if(option == 1) file << "                                 Hamming Weight                                " << endl;
        if(option == 2) file << "                               (Shannon) Entropy                               " << endl;
        if(option == 3) file << "                               Min-Entropy (CTW)                               " << endl;
                        file << "                                                                               " << endl;
                        file << "                          Used Settings:                                       " << endl;
                        file << "                                                                               " << endl;
        if(option == 3) file << "                          Input-Path:     '" << item->input_path_name.at(0).c_str() <<"'"      << endl;
        else            file << "                          Input file:     '" << item->input_file_name <<"'"      << endl;
                        file << "                          OffSet:         " << item->offset_begin                      << endl;
                        file << "                          Length:         " << item->input_length << " byte (" << item->input_length*8 << " bit)" << endl;
                        file << "                                                                               " << endl;
        if(option == 1) file << "                          Result:         " << item->ones                        << endl;
        if(option == 2) file << "                          Result:         " << item->entrp                       << endl;
        if(option == 3){file << "                          Result:                                              " << endl;
                        file << "                          Min-Entropy (CTW)      = " << item->entrp              << endl;
                        file << "                          Fractional Min-Entropy = " << ((item->entrp) / double(item->input_length*8)) << " (" << (((item->entrp) / double(item->input_length*8)) * 100) << "%)" << endl;}
                        file << "                                                                               " << endl;
                        file << "*******************************************************************************" << endl << endl;
        file.close();
      }
    else return 14;

    file.close();

	return 0;
 }
