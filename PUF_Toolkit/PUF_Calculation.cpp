#include "PUF_Calculation.h"

/*
 * PUF-ToolKit - Calculation
 *
 * Author: Sebastian Schurig
 *
 * Calculation Functions for the PUF-ToolKit
 *
 */


int Median_AVG(struct Item *item)
/*
 * Calculate the Median and arithmetic average of a data file
 *
 * Inputs:
 * item = pointer to the struct to get and store the necessary informations
 */
 {
    fstream input_file;
    vector<double> values;
    vector<double>::iterator it;
    double tmp = 0;
    unsigned int i = 0;
    unsigned int error = 0;

    it = values.begin();

    input_file.open(item->input_file_name, std::ios_base::in);

    if(input_file.fail()) return 11;

    while (!input_file.eof()){

        // Check if input is only numeric (integer or floating point) and separated by blanks
        if(!(input_file >> tmp)){
            error = 24;
        }
        else values.push_back(tmp);
        if(error) break;
    }

    // Close the file
    input_file.close();

    // Proceed only if no error occurred
    if(!error){

        // Sort the values
        sort(values.begin(), values.end());

        // Check is the list is not empty
        if(values.empty()) return 23;

        // Check that at least 2 values in the vector
        if(values.size() == 1){
            // The element is the result
            item->median = values.at(0);
            item->average = values.at(0);
        }
        else if(values.size() >= 2){

           // Get the median
           if(values.size() % 2 == 0)
                    item->median = (double(values[values.size()/2 - 1]) + double(values[values.size()/2])) / double(2);
            else
                    item->median = values[values.size()/2];

            // Get the arithmetical average
            for(i = 0; i < values.size(); i++){
                    item->average = item->average + values.at(i);
            }
            item->average = item->average / double(values.size());
        }
        return 0;
    }
    return error;
 }


int HammingWeight(struct Item *item, int option)
/*
 * Calculate the Hamming Weight of a data file
 *
 * Inputs:
 * item = pointer to the struct to get and store the necessary informations
 * option = definition if a single file or complete directory will be processed
 *
 * Options:
 * 0 = single file
 * 1 = complete directory
 */
{
    FILE *fd;
    unsigned char * inputData;
    unsigned long filesize;
    unsigned int i;
    unsigned int error = 0;

    // Reset the values
    item->ones = 0;
    item->zeros = 0;

    if(option == 0){
		// Open the specified file
        if((fd = fopen(item->input_file_name, "rb")) == NULL) return 12;

        // Get the filesize
        fseek(fd, 0, SEEK_END);
        filesize = ftell(fd);
        rewind(fd);

        // Go to the offset position in the file
        fseek(fd, item->offSet, SEEK_SET);

        //set the length to be read as the filesize - 2*offset
        item->input_length = filesize - item->offSet - item->offset_end;
        // Check if the chosen part of the PUF-Response is valid
        if((item->offSet+item->input_length) > filesize) return 13;

        // Get space to read in the input file
        inputData = (unsigned char *) malloc(sizeof(char) * item->input_length);

        if (fread(&inputData[0], sizeof(char), item->input_length, fd) != item->input_length) return 3;

        // Check each bit of a byte, if it is set or not an calculate the Hamming Weight
        // We want to have the # of zero's and the # of one's
        for(i = 0; i < item->input_length;i++){

            // Calculation of the Hamming Weight
            if((inputData[i] & 0x80) == 0x00) item->zeros++;                       // bit at position 8 == 0
            else item->ones++;
            if((inputData[i] & 0x40) == 0x00) item->zeros++;                       // bit at position 7 == 0
            else item->ones++;
            if((inputData[i] & 0x20) == 0x00) item->zeros++;                       // bit at position 6 == 0
            else item->ones++;
            if((inputData[i] & 0x10) == 0x00) item->zeros++;                       // bit at position 5 == 0
            else item->ones++;
            if((inputData[i] & 0x08) == 0x00) item->zeros++;                       // bit at position 4 == 0
            else item->ones++;
            if((inputData[i] & 0x04) == 0x00) item->zeros++;                       // bit at position 3 == 0
            else item->ones++;
            if((inputData[i] & 0x02) == 0x00) item->zeros++;                       // bit at position 2 == 0
            else item->ones++;
            if((inputData[i] & 0x01) == 0x00) item->zeros++;                       // bit at position 1 == 0
            else item->ones++;
        }
        fclose(fd);
        free(inputData);
    }
    else if(option == 1){

        DIR *dir;
        struct dirent *ent;
        string path_and_file_name;
        string check_output_name(item->output_file_name);
        const char * path_file_name;
        vector<string> list_of_files;
        vector<string>::iterator it;
        ofstream result_file;

        it = list_of_files.begin();

        // Check if Input directory is a directory
        if(!IsDir(item->input_path_name.at(0).c_str())){
            return 19;
        }

        string path(item->input_path_name.at(0).c_str());

        // Check if a output filename is set
        if(check_output_name == "none") return 18;

        // Get all the filenames included in the specified directory
        if ((dir = opendir (item->input_path_name.at(0).c_str())) != NULL) {
            while ((ent = readdir (dir)) != NULL) {
                // Skip '.' and '..'
                if( strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0 ) {
                    // Generate complete path+filename
                    path_and_file_name = "";
                    path_and_file_name += path;
                    path_and_file_name += "/";
                    path_and_file_name += ent->d_name;
                    path_file_name = path_and_file_name.c_str();
                    // Only take existing files
                    if(IsFile(path_file_name)) {
                        // Add filenames in a List
                        list_of_files.push_back(path_file_name);
                    }
                }
            }
            closedir (dir);
        } else {
        /* Could not open directory */
            perror ("");
            return EXIT_FAILURE;
        }

        // Check if more the one file is in the folder
        if(list_of_files.size() < 1) return 17;

        for(it = list_of_files.begin();it != list_of_files.end(); ++it){

            // Set the input_file_name for each entry of the list_of_files
            strcpy(item->input_file_name, (*it).c_str());

            //Calculate the (Shannon) Entropy of the defined file
            error = HammingWeight(item, 0);

            if(error == 0){

                // Open the output file to store the results (append)
                result_file.open (item->output_file_name, ios::out | ios::app );

                if (result_file.is_open()){

                    // Minimal output of plain result values, separated with a " "
                    result_file << item->ones;

                    // Remove the last " " in the end of the file (might be problematic with new calculation due to appending)
                    if(!((it+1) == list_of_files.end())) {
                        result_file << " ";
                    }
                    result_file.close();
                }
                else return 16;

                // Reset the values
                item->ones = 0;
                item->zeros = 0;
                if(error !=0) break;
            }
        }
    }
return error;
}

int Entropy(struct Item *item, int option)
/*
 * Calculate the entropy of a data file
 *
 * Inputs:
 * item = pointer to the struct to get and store the necessary informations
 * option = definition if a single file or complete directory will be processed
 *
 * Options:
 * 0 = single file
 * 1 = complete directory
 */
{
    int error = 0;

    // Reset the values
    item->ones = 0;
    item->zeros = 0;
    item->entrp = 0;

    if(option == 0){
        double p_zeros;
        double p_ones;

        // Get the amount of 1's and 0's
        error = HammingWeight(item, 0);

        if(!error){
            // Calculate the probabilities
            p_zeros = (double(1) / double(item->input_length*8)) * double(item->zeros);
            p_ones = (double(1) / double(item->input_length*8)) * double(item->ones);

            // Calculate and store the entropy
            item->entrp = -((p_zeros * log2(p_zeros)) + (p_ones * log2(p_ones)));
        }

    }
    else if(option == 1){

        DIR *dir;
        struct dirent *ent;
        string path_and_file_name;
        string check_output_name(item->output_file_name);
        const char * path_file_name;
        vector<string> list_of_files;
        vector<string>::iterator it;
        ofstream result_file;

        it = list_of_files.begin();

        // Check if Input directory is a directory
        if(!IsDir(item->input_path_name.at(0).c_str())){
            return 19;
        }

        string path(item->input_path_name.at(0).c_str());

        // Check if a output filename is set
        if(check_output_name == "none") return 18;

        // Get all the filenames included in the specified directory
        if ((dir = opendir (item->input_path_name.at(0).c_str())) != NULL) {
            while ((ent = readdir (dir)) != NULL) {
                // Skip '.' and '..'
                if( strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0 ) {
                    // Generate complete path+filename
                    path_and_file_name = "";
                    path_and_file_name += path;
                    path_and_file_name += "/";
                    path_and_file_name += ent->d_name;
                    path_file_name = path_and_file_name.c_str();
                    // Only take existing files
                    if(IsFile(path_file_name)) {
                        // Add filenames in a List
                        list_of_files.push_back(path_file_name);
                    }
                }
            }
            closedir (dir);
        } else {
        /* Could not open directory */
            perror ("");
            return EXIT_FAILURE;
        }

        // Check if more the one file is in the folder
        if(list_of_files.size() < 1) return 17;

        for(it = list_of_files.begin();it != list_of_files.end(); ++it){

            // Set the input_file_name for each entry of the list_of_files
            strcpy(item->input_file_name, (*it).c_str());

            //Calculate the (Shannon) Entropy of the defined file
            error = Entropy(item, 0);

            if(error == 0){

                // Open the output file to store the results (append)
                result_file.open (item->output_file_name, ios::out | ios::app );

                if (result_file.is_open()){

                    // Minimal output of plain result values, separated with a " "
                    result_file << item->entrp;

                    // Remove the last " " in the end of the file (might be problematic with new calculation due to appending)
                    if(!((it+1) == list_of_files.end())) {
                        result_file << " ";
                    }
                    result_file.close();
                }
                else return 16;
            }
            // Reset the values
            item->ones = 0;
            item->zeros = 0;
            item->entrp = 0;

            if(error !=0) break;
        }
    }
    return error;
}

int IntraHD(struct Item *item, int option)
/*
 * Calculate the Intra-Hamming Distance of all files of a defined directory
 *
 * Inputs:
 * item = pointer to the struct to get and store the necessary informations
 * option = define style of the output format
 *
 * Options:
 * 0 = compact - no duplicate calculation
 * 1 = minimal - plain results sequence
 */
{
    DIR *dir;
    struct dirent *ent;
    string path_and_file_name;
    string check_output_name(item->output_file_name);
    string white_spaces;
    const char * path_file_name;
    vector<string> list_of_files;
    vector<string>::iterator it;
    vector<string>::iterator it2;
    ofstream result_file;
    unsigned int counter = 0;
    unsigned int max_file_length = 0;
    bool header = true;
    bool next_table = true;

    it = list_of_files.begin();

    // Check if Input directory is a directory
    if(!IsDir(item->input_path_name.at(0).c_str())){
        return 19;
    }

    string path(item->input_path_name.at(0).c_str());

    // Check if a output filename is set
	if(check_output_name == "none") return 18;

    // Get all the filenames included in the specified directory
    if ((dir = opendir (item->input_path_name.at(0).c_str())) != NULL) {
        while ((ent = readdir (dir)) != NULL) {
            // Skip '.' and '..'
            if( strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0 ) {
                // Generate complete path+filename
                path_and_file_name = "";
                path_and_file_name += path;
                path_and_file_name += "/";
                path_and_file_name += ent->d_name;
                path_file_name = path_and_file_name.c_str();
                // Only take existing files
                if(IsFile(path_file_name)) {
                    // Add filenames in a List
                    list_of_files.push_back(path_file_name);

                    // Check the length of the file and find the maximum
                    if(strlen(path_file_name) > max_file_length) max_file_length = strlen(path_file_name);
                }
            }
        }
        closedir (dir);
    } else {
    /* Could not open directory */
        perror ("");
        return EXIT_FAILURE;
    }

    // Check if more the one file is in the folder
    if(list_of_files.size() <= 1) return 17;

    FILE *fd;
    FILE *fd2;
    unsigned char * file1;
    unsigned char * file2;
    unsigned char inputData;
    unsigned long file1_size;
    unsigned long file2_size;
    unsigned int i = 0;

    for(it = list_of_files.begin();it != list_of_files.end(); ++it){

        // Try to open the file
        if((fd = fopen((*it).c_str(), "rb")) == NULL) return 12;

        // Get the filesize
        fseek(fd, 0, SEEK_END);
        file1_size = ftell(fd);
        rewind(fd);

        // Get to the correct position in the file (defined by the offset)
        fseek(fd, item->offSet, SEEK_SET);

        // Check if the chosen part of the PUF-Response is valid
        if((item->offSet+item->input_length) > file1_size) return 13;

        // Get space to read in the input file of length input_length
        file1 = (unsigned char *) malloc(sizeof(char) * item->input_length);

        // Read in the file into a buffer
        if (fread(&file1[0], sizeof(char), item->input_length, fd) != item->input_length) return 14;

        for(it2 = (it+1);it2 != list_of_files.end(); ++it2){

            // Try to open the file
            if((fd2 = fopen((*it2).c_str(), "rb")) == NULL) return 12;

            // Get the filesize
            fseek(fd2, 0, SEEK_END);
            file2_size = ftell(fd2);
            rewind(fd2);

            // Get to the correct position in the file (defined by the offset)
            fseek(fd2, item->offSet, SEEK_SET);

            // Check if the chosen part of the PUF-Response is valid
            if((item->offSet+item->input_length) > file2_size) return 13;

            // Get space to read in the input file of length input_length
            file2 = (unsigned char *) malloc(sizeof(char) * item->input_length);

            // Read in the file into a buffer
            if (fread(&file2[0], sizeof(char), item->input_length, fd2) != item->input_length) return 14;

            // Calculate Intra-Hamming Distance
            // Check each bit of a byte, if it is set or not an calculate the Hamming Distance
            for(i = 0; i < item->input_length;i++){
                    inputData = file1[i] ^ file2[i];

                // Check each byte separate
                if((inputData & 0x80) != 0x00) counter++;                    // bit at position 8 == 1
				if((inputData & 0x40) != 0x00) counter++;                    // bit at position 7 == 1
				if((inputData & 0x20) != 0x00) counter++;                    // bit at position 6 == 1
				if((inputData & 0x10) != 0x00) counter++;                    // bit at position 5 == 1
                if((inputData & 0x08) != 0x00) counter++;                    // bit at position 4 == 1
                if((inputData & 0x04) != 0x00) counter++;                    // bit at position 3 == 1
                if((inputData & 0x02) != 0x00) counter++;                    // bit at position 2 == 1
                if((inputData & 0x01) != 0x00) counter++;                    // bit at position 1 == 1
            }

            free(file2);
            fclose(fd2);
            // Open the output file to store the results (append)
            result_file.open (item->output_file_name, ios::out | ios::app );

            if (result_file.is_open()){
                if(option == 0){
                    if(header){
                            result_file << "*******************************************************************************" << endl;
                            result_file << "                                                                               " << endl;
                            result_file << "                             Intra-Hamming Distance                            " << endl;
                            result_file << "                                                                               " << endl;
                            result_file << "                          Used Settings:                                       " << endl;
                            result_file << "                                                                               " << endl;
                            result_file << "                          Device folder: '" << item->input_path_name.at(0).c_str() <<"'" << endl;
                            result_file << "                          OffSet:         " << item->offSet                      << endl;
                            result_file << "                          Length:         " << item->input_length << " byte (" << item->input_length*8 << " bit)" << endl;
                            result_file << "                                                                               " << endl;
                            result_file << "*******************************************************************************" << endl << endl;
                            header = false;
                    }
                   if(next_table){
                            if(max_file_length < 79) white_spaces = string(67 - strlen((*it).c_str()), ' ');
                            else white_spaces = string(4, ' ');
                    result_file << endl << "-------------------------------------------------------------------------------" << endl;
                            result_file << "    Filename" << white_spaces << *it << endl;
                            result_file << "-------------------------------------------------------------------------------" << endl;
                            next_table = false;
                    }
                    if(max_file_length < 79) white_spaces = string(((79 - strlen((*it2).c_str())) - ((79 - max_file_length)/2)), ' ');
                    else white_spaces = string(4, ' ');
                    result_file << *it2 << white_spaces << counter << endl;
                    if((it2+1) == list_of_files.end()){
                            result_file << "-------------------------------------------------------------------------------" << endl << endl;
                    }
                    result_file.close();
                }
                if(option == 1){
                    // Minimal output of plain result values, separated with a " "
                    result_file << counter;

                    // Remove the last " " in the end of the file (might be problematic with new calculation due to appending)
                    if(!((it2+1) == list_of_files.end() && (it+2) == list_of_files.end())) {
                        result_file << " ";
                    }
                    result_file.close();
                }
            }
            else return 16;
            counter = 0;
        }
    next_table = true;
    free(file1);
    fclose(fd);
    }
return 0;
}

int InterHD(struct Item *item, int option)
/*
 * Calculate the Inter-Hamming Distance of all files of the defined directories
 *
 * Inputs:
 * item = pointer to the struct to get and store the necessary informations
 * option = define of the calculation and output format
 *
 * Options:
 * 0 = compact - no duplicate calculation - symmetry
 * 1 = detailed - all with all - symmetries will be ignored
 * 2 = minimal - plain results sequence
 */
{
    DIR *dir;
    struct dirent *ent;
    string path_and_file_name;
    string path;
    string white_spaces;
    string check_output_name(item->output_file_name);
    const char * path_file_name;
    vector<string> list_of_all_files;
    vector<string> list_of_files;
    vector<string>::iterator it;
    vector<string>::iterator it2;
    vector<unsigned int> number_of_files;
    ofstream result_file;
    unsigned int counter = 0;
    unsigned int i = 0;
    unsigned int count_files = 0;
    unsigned int count_offset = 0;
    unsigned int max_file_length = 0;
    bool header = true;
    bool next_table = true;

    it = list_of_files.begin();

    // Check if Input directories are really directories
    for(i = 0; i < item->input_path_name.size(); i++){
        if(!IsDir(item->input_path_name.at(i).c_str())){
            item->HD_error_pos = i+1;
            return 20;
        }
    }

    // Check if Output is set
    if(check_output_name == "none") return 18;

    // Get all the filenames included in the specified directory
    for(i = 0; i < item->input_path_name.size(); i++){
        path = item->input_path_name.at(i).c_str();

        // Reset the file counter
        count_files = 0;

        if ((dir = opendir (item->input_path_name.at(i).c_str())) != NULL) {
            while ((ent = readdir (dir)) != NULL) {
                // Skip '.' and '..'
                if( strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0 ) {
                    // Generate complete path+filename
                    path_and_file_name = "";
                    path_and_file_name += path;
                    path_and_file_name += "/";
                    path_and_file_name += ent->d_name;
                    path_file_name = path_and_file_name.c_str();
                    // Only take existing files
                    if(IsFile(path_file_name)) {
                        // Add filenames in a List depending on option
                        if(option == 0 || option == 2){
                             if(i < item->input_path_name.size()-1)
                                list_of_all_files.push_back(path_file_name);
                        }
                        else if(option == 1){
                                list_of_all_files.push_back(path_file_name);
                        }

                        // Increment file counter
                        count_files++;

                        // Comparators list, skip first folder
                        if(i > 0) list_of_files.push_back(path_file_name);

                        // Check the length of the file and find the maximum
                        if(strlen(path_file_name) > max_file_length) max_file_length = strlen(path_file_name);
                    }
                }
			}
			number_of_files.push_back(count_files);
			closedir (dir);
        } else {
        /* Could not open directory */
            perror ("");
            return EXIT_FAILURE;
        }

    }

    // Check if in each folder is at least one file
    for(i = 0; i < number_of_files.size();i++){
        if(number_of_files.at(i) < 1) {
            item->HD_error_pos = i+1;
            return 21;
        }
    }

    FILE *fd;
    FILE *fd2;
    unsigned char * file1;
    unsigned char * file2;
    unsigned char inputData;
    unsigned long file1_size;
    unsigned long file2_size;
    unsigned int j = 0;
    count_files = 0;
    unsigned int c = 0;

    for(it = list_of_all_files.begin();it != list_of_all_files.end(); ++it){

        // Count the # of precessed files
        count_files++;
        count_offset++;

        // Try to open the file
        if((fd = fopen((*it).c_str(), "rb")) == NULL) return 12;

        // Get the filesize
        fseek(fd, 0, SEEK_END);
        file1_size = ftell(fd);
        rewind(fd);

        // Get to the correct position in the file (defined by the offset)
        fseek(fd, item->offSet, SEEK_SET);

        // Check if the chosen part of the PUF-Response is valid
        if((item->offSet+item->input_length) > file1_size) return 13;

        // Get space to read in the input file of length input_length
        file1 = (unsigned char *) malloc(sizeof(char) * item->input_length);

        // Read in the file into a buffer
        if (fread(&file1[0], sizeof(char), item->input_length, fd) != item->input_length) return 14;

        for(it2 = list_of_files.begin();it2 != list_of_files.end(); ++it2){

            // Try to open the file
            if((fd2 = fopen((*it2).c_str(), "rb")) == NULL) return 12;

            // Get the filesize
            fseek(fd2, 0, SEEK_END);
            file2_size = ftell(fd2);
            rewind(fd2);

            // Get to the correct position in the file (defined by the offset)
            fseek(fd2, item->offSet, SEEK_SET);

            // Check if the chosen part of the PUF-Response is valid
            if((item->offSet+item->input_length) > file2_size) return 13;

            // Get space to read in the input file of length input_length
            file2 = (unsigned char *) malloc(sizeof(char) * item->input_length);

            // read in the file into a buffer
            if (fread(&file2[0], sizeof(char), item->input_length, fd2) != item->input_length) return 14;

            // Calculate Inter-Hamming Distance
            // Check each bit of a byte, if it is set or not an calculate the Hamming Distance
            for(i = 0; i < item->input_length;i++){
                inputData = file1[i] ^ file2[i];

                // Check each byte separate
                if((inputData & 0x80) != 0x00) counter++;                    // bit at position 8 == 1
				if((inputData & 0x40) != 0x00) counter++;                    // bit at position 7 == 1
				if((inputData & 0x20) != 0x00) counter++;                    // bit at position 6 == 1
				if((inputData & 0x10) != 0x00) counter++;                    // bit at position 5 == 1
                if((inputData & 0x08) != 0x00) counter++;                    // bit at position 4 == 1
                if((inputData & 0x04) != 0x00) counter++;                    // bit at position 3 == 1
                if((inputData & 0x02) != 0x00) counter++;                    // bit at position 2 == 1
                if((inputData & 0x01) != 0x00) counter++;                    // bit at position 1 == 1
            }

            free(file2);
            fclose(fd2);

            // Open the output file to store the results (append)
            result_file.open (item->output_file_name, ios::out | ios::app );

            // Generate output content
            if (result_file.is_open()) {
                if(option == 0 || option == 1){
                    if(header){
												result_file << "*******************************************************************************" << endl;
												result_file << "                                                                               " << endl;
												result_file << "                             Inter-Hamming Distance                            " << endl;
												result_file << "                                                                               " << endl;
												result_file << "                          Used Settings:                                       " << endl;
												result_file << "                                                                               " << endl;
						for(j = 0; j < item->input_path_name.size(); j++){
							if(j == 0)  		result_file << "                          Device folder: '" << item->input_path_name.at(j).c_str() <<"'" << endl;
							else        		result_file << "                                         '" << item->input_path_name.at(j).c_str() <<"'" << endl;
						}
												result_file << "                          OffSet:         " << item->offSet                      << endl;
												result_file << "                          Length:         " << item->input_length << " byte (" << item->input_length*8 << " bit)" << endl;
						if(option == 0)			result_file << "                          Output Style:   compact                              " << endl;
						else if(option == 1) 	result_file << "                          Output Style:   detailed                             " << endl;
												result_file << "                                                                               " << endl;
												result_file << "*******************************************************************************" << endl << endl;
						header = false;
					}
					if(next_table){
						if(max_file_length < 79) white_spaces = string(67 - strlen((*it).c_str()), ' ');
						else white_spaces = string(4, ' ');
                        result_file << endl << "-------------------------------------------------------------------------------" << endl;
                                result_file << "    Filename" << white_spaces << *it << endl;
                                result_file << "-------------------------------------------------------------------------------" << endl;
						next_table = false;
					}
                    if(max_file_length < 79) white_spaces = string(((79 - strlen((*it2).c_str())) - ((79 - max_file_length)/2)), ' ');
                    else white_spaces = string(4, ' ');
                    result_file << *it2 << white_spaces << counter << endl;
                    if((it2+1) == list_of_files.end()){
                                result_file << "-------------------------------------------------------------------------------" << endl << endl;
                    }
                    result_file.close();
                }
                if (option == 2){
                    // Minimal output of plain result values, separated with a " "
                    result_file << counter;

                    // Remove the last " " in the end of the file (might be problematic with new calculation due to appending)
                    if(!((it2+1) == list_of_files.end() && (it+1) == list_of_all_files.end())) {
                        result_file << " ";
                    }
                    result_file.close();
                }
            }
            else return 16;
            counter = 0;
        }

        // Handle next table and EOF
        if(count_files == number_of_files.at(c)){
            c++;

            if(option == 0 || option == 2) list_of_files.erase(list_of_files.begin(), list_of_files.begin() + number_of_files.at(c));
            else if(option == 1) {
                    if(c < number_of_files.size()){
                        list_of_files = list_of_all_files;
                        list_of_files.erase(list_of_files.begin() + count_offset, list_of_files.begin() + count_offset + number_of_files.at(c));
                    }

            }
            count_files = 0;
        }
		next_table = true;
		free(file1);
		fclose(fd);
    }
return 0;
}

int MinEntropy(struct Item *item)
/*
 * Calculate the Min-Entropy of a device (out of all files in the defined directory)
 *
 * Inputs:
 * item = pointer to the struct to get and store the necessary informations
 */
{
    DIR *dir;
    struct dirent *ent;
    string path_and_file_name;
    const char * path_file_name;
    vector<string> list_of_files;
    vector<string>::iterator it;
    vector<unsigned int>::iterator it2;
    vector<unsigned int> p_i;
    unsigned int m = 0;
    double Hmin = 0;

    it = list_of_files.begin();
    it2 = p_i.begin();

    // Check if Input directory is a directory
    if(!IsDir(item->input_path_name.at(0).c_str())){
        return 19;
    }

    string path(item->input_path_name.at(0).c_str());

    // Get all the filenames included in the specified directory
    if ((dir = opendir (item->input_path_name.at(0).c_str())) != NULL) {
        while ((ent = readdir (dir)) != NULL) {
            // Skip '.' and '..'
            if( strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0 ) {
                // Generate complete path+filename
                path_and_file_name = "";
                path_and_file_name += path;
                path_and_file_name += "/";
                path_and_file_name += ent->d_name;
                path_file_name = path_and_file_name.c_str();
                // Only take existing files
                if(IsFile(path_file_name)) {
                    // Add filenames in a List
                    list_of_files.push_back(path_file_name);

                    // Get the # of files
                    m++;
                }
            }
        }
        closedir (dir);
    } else {
    /* Could not open directory */
        perror ("");
        return EXIT_FAILURE;
    }

    // Check if more the one file is in the folder
    if(list_of_files.size() <= 1) return 17;

    FILE *fd;
    unsigned char * inputData;
    unsigned long file1_size;
    unsigned int i = 0;
    unsigned int j = 0;
    bool initial = true;
    unsigned long bit_size = 0;
    double a;
    double b;

    for(it = list_of_files.begin();it != list_of_files.end(); ++it){

        // Try to open the file
        if((fd = fopen((*it).c_str(), "rb")) == NULL) return 12;

        // Get the filesize
        fseek(fd, 0, SEEK_END);
        file1_size = ftell(fd);
        rewind(fd);

        // Get to the correct position in the file (defined by the offset)
        fseek(fd, item->offSet, SEEK_SET);

        // Check if the chosen part of the PUF-Response is valid
        if((item->offSet+item->input_length) > file1_size) return 13;

        // Get space to read in the input file of length input_length
        inputData = (unsigned char *) malloc(sizeof(char) * item->input_length);

        // Read in the file into a buffer
        if (fread(&inputData[0], sizeof(char), item->input_length, fd) != item->input_length) return 14;

        // Define the amount of bits
        bit_size = 8 * item->input_length;

        // Initialize the vector p_i with the occurrences of zeros at each bit position (bit position == vector.at())
        if(initial){
            for(i = 0; i < bit_size; i++){
                p_i.push_back(0);
            }
            initial = false;
        }

        // Check each bit of a byte, if it is set or not count the occurrences of zeros for each bit position
        for(j = 0; j < item->input_length;j++){

                    // Check each byte separate
                    if((inputData[j] & 0x80) == 0x00) p_i[(8*j)] = p_i[(8*j)] + 1;                        // bit at position 8 == 0
					if((inputData[j] & 0x40) == 0x00) p_i[(8*j)+1] = p_i[(8*j)+1] + 1;                    // bit at position 7 == 0
					if((inputData[j] & 0x20) == 0x00) p_i[(8*j)+2] = p_i[(8*j)+2] + 1;                    // bit at position 6 == 0
					if((inputData[j] & 0x10) == 0x00) p_i[(8*j)+3] = p_i[(8*j)+3] + 1;                    // bit at position 5 == 0
                    if((inputData[j] & 0x08) == 0x00) p_i[(8*j)+4] = p_i[(8*j)+4] + 1;                    // bit at position 4 == 0
                    if((inputData[j] & 0x04) == 0x00) p_i[(8*j)+5] = p_i[(8*j)+5] + 1;                    // bit at position 3 == 0
                    if((inputData[j] & 0x02) == 0x00) p_i[(8*j)+6] = p_i[(8*j)+6] + 1;                    // bit at position 2 == 0
                    if((inputData[j] & 0x01) == 0x00) p_i[(8*j)+7] = p_i[(8*j)+7] + 1;                    // bit at position 1 == 0
        }
    free(inputData);
    fclose(fd);
    }

    // Calculate Min-Entropy: Hmin
    for(i = 0; i < bit_size; i++){

        a = (double(p_i[i]) / double(m));
        b = (double(1) - (double(p_i[i]) / double(m)));

        Hmin = Hmin + (-log2(max(a, b)));
    }
item->entrp = Hmin;
return 0;
}

