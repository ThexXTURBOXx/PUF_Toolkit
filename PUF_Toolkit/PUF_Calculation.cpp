#include "PUF_Calculation.h"
#include <assert.h>

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
    unsigned int i;
    unsigned int error = 0;

    // Reset the values
    item->ones = 0;
    item->zeros = 0;

    if(option == 0){

        error = SetInputLen(item);
	    // Open the specified file
        if((fd = fopen(item->input_file_name, "rb")) == NULL) return 12;
        // Go to the offset_begin position in the file
        fseek(fd, item->offset_begin, SEEK_SET);
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
    unsigned int error = 0;

    for(it = list_of_files.begin();it != list_of_files.end(); ++it){

        // Try to open the file
        if((fd = fopen((*it).c_str(), "rb")) == NULL) return 12;

        // Get the filesize
        fseek(fd, 0, SEEK_END);
        file1_size = ftell(fd);
        rewind(fd);

        strcpy(item->input_file_name, (*it).c_str());
        error = SetInputLen(item);
        // Get to the correct position in the file (defined by the offset_begin)
        fseek(fd, item->offset_begin, SEEK_SET);

        // Check if the chosen part of the PUF-Response is valid
        if((item->offset_begin+item->input_length) > file1_size) return 13;

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

            strcpy(item->input_file_name, (*it2).c_str());
            error = SetInputLen(item);
            // Get to the correct position in the file (defined by the offset_begin)
            fseek(fd2, item->offset_begin, SEEK_SET);

            // Check if the chosen part of the PUF-Response is valid
            if((item->offset_begin+item->input_length) > file2_size) return 13;

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
                            result_file << "                          offset_begin:         " << item->offset_begin                      << endl;
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

        // Get to the correct position in the file (defined by the offset_begin)
        fseek(fd, item->offset_begin, SEEK_SET);
        //set the length to be read as the filesize - sum of offsets
        item->input_length = file1_size - item->offset_begin - item->offset_end;
        printf("file1 input_length: %lu\n", item->input_length);
        // Check if the chosen part of the PUF-Response is valid
        if((item->offset_begin+item->input_length) > file1_size) return 13;

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

            // Get to the correct position in the file (defined by the offset_begin)
            fseek(fd2, item->offset_begin, SEEK_SET);
            //set the length to be read as the filesize - sum of offsets
            item->input_length = file2_size - item->offset_begin - item->offset_end;
            // Check if the chosen part of the PUF-Response is valid
            if((item->offset_begin+item->input_length) > file2_size) return 13;

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
												result_file << "                          offset_begin:         " << item->offset_begin                      << endl;
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
    unsigned int error = 0;

    for(it = list_of_files.begin();it != list_of_files.end(); ++it){

        // Try to open the file
        if((fd = fopen((*it).c_str(), "rb")) == NULL) return 12;

        // Get the filesize
        fseek(fd, 0, SEEK_END);
        file1_size = ftell(fd);
        rewind(fd);

        strcpy(item->input_file_name, (*it).c_str());
        error = SetInputLen(item);
        // Get to the correct position in the file (defined by the offset_begin)
        fseek(fd, item->offset_begin, SEEK_SET);

        // Check if the chosen part of the PUF-Response is valid
        if((item->offset_begin+item->input_length) > file1_size) return 13;

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

/*
 * File:    bch3.c
 * Title:   Encoder/decoder for binary BCH codes in C (Version 3.1)
 * Author:  Robert Morelos-Zaragoza
 * Date:    August 1994
 * Revised: June 13, 1997
 *
 * ===============  Encoder/Decoder for binary BCH codes in C =================
 *
 * Version 1:   Original program. The user provides the generator polynomial
 *              of the code (cumbersome!).
 * Version 2:   Computes the generator polynomial of the code.
 * Version 3:   No need to input the coefficients of a primitive polynomial of
 *              degree m, used to construct the Galois Field GF(2**m). The
 *              program now works for any binary BCH code of length such that:
 *              2**(m-1) - 1 < length <= 2**m - 1
 *
 * Note:        You may have to change the size of the arrays to make it work.
 *
 * The encoding and decoding methods used in this program are based on the
 * book "Error Control Coding: Fundamentals and Applications", by Lin and
 * Costello, Prentice Hall, 1983.
 *
 * Thanks to Patrick Boyle (pboyle@era.com) for his observation that 'bch2.c'
 * did not work for lengths other than 2**m-1 which led to this new version.
 * Portions of this program are from 'rs.c', a Reed-Solomon encoder/decoder
 * in C, written by Simon Rockliff (simon@augean.ua.oz.au) on 21/9/89. The
 * previous version of the BCH encoder/decoder in C, 'bch2.c', was written by
 * Robert Morelos-Zaragoza (robert@spectra.eng.hawaii.edu) on 5/19/92.
 *
 * NOTE:
 *          The author is not responsible for any malfunctioning of
 *          this program, nor for any damage caused by it. Please include the
 *          original program along with these comments in any redistribution.
 *
 *  For more information, suggestions, or other ideas on implementing error
 *  correcting codes, please contact me at:
 *
 *                           Robert Morelos-Zaragoza
 *                           5120 Woodway, Suite 7036
 *                           Houston, Texas 77056
 *
 *                    email: r.morelos-zaragoza@ieee.org
 *
 * COPYRIGHT NOTICE: This computer program is free for non-commercial purposes.
 * You may implement this program for any non-commercial application. You may
 * also implement this program for commercial purposes, provided that you
 * obtain my written permission. Any modification of this program is covered
 * by this copyright.
 *
 * == Copyright (c) 1994-7,  Robert Morelos-Zaragoza. All rights reserved.  ==
 *
 * m = order of the Galois field GF(2**m)
 * n = 2**m - 1 = size of the multiplicative group of GF(2**m)
 * length = length of the BCH code
 * t = error correcting capability (max. no. of errors the code corrects)
 * d = 2*t + 1 = designed min. distance = no. of consecutive roots of g(x) + 1
 * k = n - deg(g(x)) = dimension (no. of information bits/codeword) of the code
 * p[] = coefficients of a primitive polynomial used to generate GF(2**m)
 * g[] = coefficients of the generator polynomial, g(x)
 * alpha_to [] = log table of GF(2**m)
 * index_of[] = antilog table of GF(2**m)
 * data[] = information bits = coefficients of data polynomial, i(x)
 * bb[] = coefficients of redundancy polynomial x^(length-k) i(x) modulo g(x)
 * numerr = number of errors
 * errpos[] = error positions
 * recd[] = coefficients of the received polynomial
 * decerror = number of decoding errors (in _message_ positions)
 *
 * =============================================================================
 *
 *  This is a BCH Encoder for PUF Responses to generate secure HelperData for
 *  the realization of a SecureKeyStorage.
 *
 *  The main BCH encoder and decoder code are from:
 *
 * File:    bch3.c
 * Title:   Encoder/decoder for binary BCH codes in C (Version 3.1)
 * Author:  Robert Morelos-Zaragoza
 * Date:    August 1994
 * Revised: June 13, 1997
 *
 *  The encoder and decoder functionality was fixed, due the base implementation
 *  was not running properly. Additional the functionality for the SecureKeyStorage
 *  and HelperData generation / reconstruction was added.
 *
 *  This code is written / modified by:      Sebastian Schurig
 */


/*
 * BCH-Encoder - Calculation
 *
 * Author: Sebastian Schurig
 *
 * Calculation Functions for the BCH-Encoder
 *
 */


void read_p()
/*
 *	Read m, the degree of a primitive polynomial p(x) used to compute the
 *	Galois field GF(2**m). Get precomputed coefficients p[] of p(x). Read
 *	the code length.
 */
{
	int			    i = 0, ninf;
	char            *h;
    unsigned int    ch;
    char            user_input[7];
    int             error = 0;


	while(true){
        if (error != 0) ClearScreen();
        cout << "*******************************************************************************" << endl;
		cout << "*                                                                             *" << endl;
		cout << "*                  Set the relevant parameter to define the BCH mode:         *" << endl;
		cout << "*                                                                             *" << endl;
		cout << "*                   'm'  = such that the code length is in between            *" << endl;
		cout << "*                                                                             *" << endl;
        cout << "*                        (2^(m-1) - 1) < length <= (2^m - 1)                  *" << endl;
		cout << "*                                                                             *" << endl;
		cout << "*******************************************************************************" << endl;

        if(error) ErrorMessages(error, i, 0);
        error = 0;

        // Get "m" as user input
        cout << endl << "Enter m (between 4 and 14): ";
        if (fgets(user_input, sizeof(user_input), stdin)) {
            /* fgets succeeds, scan for newline character */
            h = strchr(user_input, '\n');
                if (h) {
                    if(user_input[0] == '\n') error = 6;
                    else {
                        *h = '\0';
                        //check input if only digits are used
                        for(i = 0; i < (signed)sizeof(user_input)-1; i++){
                            if(user_input[i] != '\0' && !isdigit(user_input[i])){
                                error = 1;
                                break;
                            }
                            if(user_input[i] == '\0') i = sizeof(user_input);
                        }
                        if(!error){
                            if(atoi(user_input) >= 4 && atoi(user_input) <= 14){
                                m = atoi(user_input);
                                break;
                            }
                            else error = 7;
                        }
                    }
                }
                else {
                    /* newline not found, flush stdin to end of line */
                        while (((ch = getchar()) != '\n')
                                && !feof(stdin)
                                && !ferror(stdin)
                            );
                        error = 2;
                }
        }
        else {
            /* fgets failed, handle error */
            cin.clear();
            error = 3;
        }
    }

    // Generate coefficients of a primitive polynomial used to generate GF(2**m)
    for (i=1; i<m; i++)
            p[i] = 0;

    p[0] = p[m] = 1;

    if 		(m == 4)	p[1] = 1;
    else if (m == 5)	p[2] = 1;
    else if (m == 6)	p[1] = 1;
    else if (m == 7)	p[1] = 1;
    else if (m == 8)	p[4] = p[5] = p[6] = 1;
    else if (m == 9)	p[4] = 1;
    else if (m == 10)	p[3] = 1;
    else if (m == 11)	p[2] = 1;
    else if (m == 12)	p[3] = p[4] = p[7] = 1;
    else if (m == 13)	p[1] = p[3] = p[4] = 1;
    else if (m == 14)	p[1] = p[11] = p[12] = 1;

    // Calculate 'n'
    n = 1;
    for (i = 0; i <= m; i++) {
        n *= 2;
    }

    // Calculate the supported BCH code length, limited by "m"
    n = n / 2 - 1;
    ninf = (n + 1) / 2 - 1;

    // Get the code length from user input
    while(true){
        ClearScreen();
        cout << "*******************************************************************************" << endl;
        cout << "*                                                                             *" << endl;
        cout << "*             Set the relevant parameter to define the BCH mode:              *" << endl;
        cout << "*                                                                             *" << endl;
        cout << "*              length  = the length of each BCH encoded codeword              *" << endl;
        cout << "*                                                                             *" << endl;
        cout << "*******************************************************************************" << endl;

        if(error) ErrorMessages(error, i, n);
        error = 0;

        cout << endl << "Enter the code length between (" << ninf << " < length <= " << n << "): ";
        if (fgets(user_input, sizeof(user_input), stdin)) {
            /* fgets succeeds, scan for newline character */
            h = strchr(user_input, '\n');
            if (h) {
                if(user_input[0] == '\n') error = 6;
                else {
                    *h = '\0';
                    //check input if only digits are used
                    for(i = 0; i < (signed)sizeof(user_input)-1; i++){
                        if(user_input[i] != '\0' && !isdigit(user_input[i])){
                            error = 1;
                            break;
                        }
                        if(user_input[i] == '\0') i = sizeof(user_input);
                    }
                    if(!error){
                        if(atoi(user_input) > ninf && atoi(user_input) <= n){
                            length = atoi(user_input);
                            break;
                        }
                        else {
                            error = 8;
                            i = ninf;
                        }
                    }
                }
            }
            else {
                /* newline not found, flush stdin to end of line */
                while (((ch = getchar()) != '\n')
                                && !feof(stdin)
                                && !ferror(stdin)
                            );
                error = 2;
            }
        }
        else {
            /* fgets failed, handle error */
            cin.clear();
            error = 3;
        }
    }
}

void generate_gf()
/*
 * Generate field GF(2**m) from the irreducible polynomial p(X) with
 * coefficients in p[0]..p[m].
 *
 * Lookup tables:
 *   index->polynomial form: alpha_to[] contains j=alpha^i;
 *   polynomial form -> index form:	index_of[j=alpha^i] = i
 *
 * alpha=2 is the primitive element of GF(2**m)
 */
{
	register int    i, mask;


	mask = 1;
	alpha_to[m] = 0;
	for (i = 0; i < m; i++) {
		alpha_to[i] = mask;
		index_of[alpha_to[i]] = i;
		if (p[i] != 0)
			alpha_to[m] ^= mask;
		mask <<= 1;

	}
	index_of[alpha_to[m]] = m;
	mask >>= 1;
	for (i = m + 1; i < n; i++) {
		if (alpha_to[i - 1] >= mask)
		  alpha_to[i] = alpha_to[m] ^ ((alpha_to[i - 1] ^ mask) << 1);
		else
		  alpha_to[i] = alpha_to[i - 1] << 1;
		index_of[alpha_to[i]] = i;
	}
	index_of[0] = -1;
}

void gen_poly()
/*
 * Compute the generator polynomial of a binary BCH code. Fist generate the
 * cycle sets modulo 2**m - 1, cycle[][] =  (i, 2*i, 4*i, ..., 2^l*i). Then
 * determine those cycle sets that contain integers in the set of (d-1)
 * consecutive integers {1..(d-1)}. The generator polynomial is calculated
 * as the product of linear factors of the form (x+alpha^i), for every i in
 * the above cycle sets.
 */
{
	register int	ii, jj, ll, kaux, zero;
	register int	test, aux, nocycles, root, noterms, rdncy;
	int      cycle[8192][21];
	std::vector<int> size(8192);
    std::vector<int> min(2048);
    std::vector<int> zeros(16385);
    char        *h;
    unsigned int ch;
    char user_input[6];
    int error = 0, i = 0, done = 0;
    char processing[3] = {' ',' ','\0'};
    double max_error = 0;


	/* Generate cycle sets modulo n, n = 2**m - 1 */
	cycle[0][0] = 0;
	size[0] = 1;
	cycle[1][0] = 1;
	size[1] = 1;
	jj = 1;			/* cycle set index */

	if (m > 9)  {

		printf("\nComputing cycle sets modulo %d\n", n);
		printf("(This may take some time)...\n\n");
	}
	do {
		/* Generate the jj-th cycle set */
		ii = 0;
		do {
			ii++;
			cycle[jj][ii] = (cycle[jj][ii - 1] * 2) % n;
			size[jj]++;
			aux = (cycle[jj][ii] * 2) % n;
		} while (aux != cycle[jj][0]);

		/* Next cycle set representative */
		ll = 0;
		do {
			ll++;
			test = 0;
			for (ii = 1; ((ii <= jj) && (!test)); ii++)
			/* Examine previous cycle sets */
			  for (kaux = 0; ((kaux < size[ii]) && (!test)); kaux++)
			     if (ll == cycle[ii][kaux])
			        test = 1;
		} while ((test) && (ll < (n - 1)));
		if (!(test)) {
			jj++;	/* next cycle set index */
			cycle[jj][0] = ll;
			size[jj] = 1;
		}

    zero = ((n-1)/100);

    // Status progress
    if(zero != 0 && (ll/zero) <= 100) cout << "\r" << setw(-20) << StatusProgress((ll/zero)) << " " << (ll/zero) << "% completed." << processing << flush;
        else if(zero != 0 && (ll/zero) >= 100) cout << "\r" << setw(-20) << StatusProgress(100) << " " << 100 << "% completed.  " << flush;

    if(processing[0] == ' ' && processing[1] == ' '){
            processing[0] = '.';
    }
    else if(processing[0] == '.' && processing[1] == ' '){
        processing[1] = '.';
    }
    else if(processing[0] == '.' && processing[1] == '.') {
       processing[0] = ' ';
       processing[1] = ' ';
    }

	} while (ll < (n - 1));
	nocycles = jj;		/* number of cycle sets modulo n */

	while(!done){
        while(true){
            ClearScreen();
            cout << "*******************************************************************************" << endl;
            cout << "*                                                                             *" << endl;
            cout << "*             Set the relevant parameter to define the BCH mode:              *" << endl;
            cout << "*                                                                             *" << endl;
            cout << "*    't'  = the error correcting capability for each BCH encoded codeword     *" << endl;
            cout << "*                                                                             *" << endl;
            cout << "*******************************************************************************" << endl;

            if(error) ErrorMessages(error, i, (int)max_error);
            error = 0;

            // Get "t" as user input
            cout << endl << "Enter the error correcting capability, t: ";
            if (fgets(user_input, sizeof(user_input), stdin)) {
                /* fgets succeeds, scan for newline character */
                h = strchr(user_input, '\n');
                if (h) {
                    if(user_input[0] == '\n') error = 6;
                    else {
                        *h = '\0';
                        //check input if only digits are used
                        for(i = 0; i < (signed)sizeof(user_input)-1; i++){
                            if(user_input[i] != '\0' && !isdigit(user_input[i])){
                                error = 1;
                                break;
                            }
                            if(user_input[i] == '\0') i = sizeof(user_input);
                        }
                        if(!error){
                            t = atoi(user_input);
                            break;
                        }
                    }
                }
                else {
                    /* newline not found, flush stdin to end of line */
                    while (((ch = getchar()) != '\n')
                            && !feof(stdin)
                            && !ferror(stdin)
                            );
                    error = 2;
                }
            }
            else {
                /* fgets failed, handle error */
                cin.clear();
                error = 3;
            }
        }

        d = 2 * t + 1;

        /* Search for roots 1, 2, ..., d-1 in cycle sets */
        kaux = 0;
        rdncy = 0;
        max_error = 0;
        for (ii = 1; ii <= nocycles; ii++) {
            min[kaux] = 0;
            test = 0;
            for (jj = 0; ((jj < size[ii]) && (!test)); jj++)
                for (root = 1; ((root < d) && (!test)); root++)
                    if (root == cycle[ii][jj])  {
                        test = 1;
                        min[kaux] = ii;
                    }
            if (min[kaux]) {
                rdncy += size[min[kaux]];
                kaux++;
            }
        }
        noterms = kaux;
        kaux = 1;
        for (ii = 0; ii < noterms; ii++)
            for (jj = 0; jj < size[min[ii]]; jj++) {
                zeros[kaux] = cycle[min[ii]][jj];
                kaux++;
            }

        k = length - rdncy;

        if (k<2){
             i = (int)(exp2((double(m))) - double(1));
             max_error = exp2((double(m-2))) - double(1);
             error = 9;
        }
        else done = 1;
    }

	/* Compute the generator polynomial */
	g[0] = alpha_to[zeros[1]];
	g[1] = 1;		/* g(x) = (X + zeros[1]) initially */
	for (ii = 2; ii <= rdncy; ii++) {
	  g[ii] = 1;
	  for (jj = ii - 1; jj > 0; jj--)
	    if (g[jj] != 0)
	      g[jj] = g[jj - 1] ^ alpha_to[(index_of[g[jj]] + zeros[ii]) % n];
	    else
	      g[jj] = g[jj - 1];
	  g[0] = alpha_to[(index_of[g[0]] + zeros[ii]) % n];
	}
}

void encode_bch()
/*
 * Compute redundacy bb[], the coefficients of b(x). The redundancy
 * polynomial b(x) is the remainder after dividing x^(length-k)*data(x)
 * by the generator polynomial g(x).
 */
{
	int    i, j;
	int    feedback;


	for (i = 0; i < length - k; i++)
		bb[i] = 0;
	for (i = k - 1; i >= 0; i--) {
		feedback = data[i] ^ bb[length - k - 1];
		if (feedback != 0) {
			for (j = length - k - 1; j > 0; j--)
				if (g[j] != 0)
					bb[j] = bb[j - 1] ^ feedback;
				else
					bb[j] = bb[j - 1];
			bb[0] = g[0] && feedback;
		} else {
			for (j = length - k - 1; j > 0; j--)
				bb[j] = bb[j - 1];
			bb[0] = 0;
		}
	}
}

int Calculation_encode(struct Item *item)
/*
 * Function to handle the actual calculation:
 *
 * -> Generate BCH encoded codewords.
 * -> Perform the linear repetition.
 * -> XOR codewords with the PUF Response.
 * -> Store the helper-data into a file.
 * -> Append the Pattern to the end of the file
 *
 * Inputs:
 * item  = pointer to the struct to get/store the necessary informations
 */
{
	FILE *fd;
    long puf_filesize;
    struct Pattern infos;
	int    i = 0, j = 0, z = 0;
	int    len = 0, u = 0, encode_binary_length = 0;
    char processing[3] = {' ',' ','\0'};


	// Check if PUF file is a file
    if(!IsFile(item->input_PUF_name)){
        return 12;
    }

	// Get the file size in binary int representation
    u = (item->input_Key_length*8);

    // Check if the file size is a multiplicative to the chosen factor 'k' of the BCH mode
    if( u%k != 0){
        // If it is not a multiplicative to the chosen factor 'k' of the BCH mode, determine the new length
        len = (((item->input_Key_length*8)/k)+1)*k;
    }
    else {
        // If it is a multiplicative to the chosen factor 'k' of the BCH mode use it
        len = (item->input_Key_length*8);
    }

    // Check if the encoded key is byte block aligned
    if (((len/k)*length)%8 != 0){
        encode_binary_length = (((len/k)*length) / 8)+1;
    }
    else {
        encode_binary_length = ((len/k)*length) / 8;
    }

    // Responsible for correct status progress visualization
    u = (len-1) / 100;

    cout << endl << "Calculating BCH Encoding:" << endl << "(This may take some time)..." << endl << endl;

    // Process the actual BCH encoding for the complete Key file
    for(j = 0; j < len ; j= j+k){

        // Fill the data vector with the input values
        for(i = 0; i < k; i++){
            if( i + j < (signed)input.size()){
                data[i] = input[i+j];
            }
            else {
                data[i] = 0;
            }
        }

        encode_bch();           /* encode data block wise */

		// Take the calculated redundancy and store it in the recd vector
        for (i = 0; i < length - k; i++){
            recd[i] = bb[i];
        }

		// Take the corresponding original input and store it in the recd vector at the corresponding position
        for (i = 0; i < k; i++){
            recd[i + (length - k)] = data[i];
        }

        // Store the BCH codewords in the output vector
        for (i = 0; i < length; i++){
            output.push_back(recd[i]);
        }

        // Increment the iteration counter
        z = z + 1;

        // Processing status visualization of the BCH encoding process
        if ((j+k) >= len) cout << "\r" << setw(-20) << StatusProgress(100) << " " << 100 << "% completed.  " << flush;
        else cout << "\r" << setw(-20) << StatusProgress((j/u)) << " " << (j/u) << "% completed." << processing << flush;

        // Visualization of the working process
        if(processing[0] == ' ' && processing[1] == ' ') processing[0] = '.';
        else if(processing[0] == '.' && processing[1] == ' ') processing[1] = '.';
        else if(processing[0] == '.' && processing[1] == '.') {
           processing[0] = ' ';
           processing[1] = ' ';
        }
	}

	// Allocate space for the LR encoded BCH code words, the binary encoded codeword representation and the PUF Response
	unsigned char *codeWordArrayBeforLR = (unsigned char *) malloc(sizeof(unsigned char) * encode_binary_length);
	unsigned char *encodeLR = (unsigned char *) malloc(sizeof(unsigned char) * (item->LR*encode_binary_length));
	unsigned char *puf = (unsigned char *) malloc(sizeof(unsigned char) * (item->LR*encode_binary_length));

    // Try to open the PUF file
    if ((fd = fopen(item->input_PUF_name, "rb")) == NULL) {
		fclose(fd);
        return 11;
    }

    // Get the file size
    fseek(fd, 0, SEEK_END);
    puf_filesize = ftell(fd);
    rewind(fd);

    // Go to the offset position in the file
    fseek(fd, item->offSet, SEEK_SET);

    // Check if the chosen part of the PUF-Response is valid
    if((item->LR*encode_binary_length) > puf_filesize) {
            fclose(fd);
            return 13;
    }

    // Read the PUF Response into the allocated space puf[]
    if ((signed)fread(&puf[0], sizeof(char), (item->LR*encode_binary_length), fd) != (item->LR*encode_binary_length)) {
        fclose(fd);
        return 14;
    }

    // Close PUF file
    fclose(fd);

	// Generate from the binary bit stream (a integer vector) each byte block for the encoded codewords
	for(j = 0; j < encode_binary_length; j++){
        codeWordArrayBeforLR[j] = 0;
        if(7+(j * 8) < (signed) output.size()){
            if(output.at(7+(j*8)) ==  1) codeWordArrayBeforLR[j]^=1;
            if(output.at(7+(j*8)-1) == 1) codeWordArrayBeforLR[j]^=2;
            if(output.at(7+(j*8)-2) == 1) codeWordArrayBeforLR[j]^=4;
            if(output.at(7+(j*8)-3) == 1) codeWordArrayBeforLR[j]^=8;
            if(output.at(7+(j*8)-4) == 1) codeWordArrayBeforLR[j]^=16;
            if(output.at(7+(j*8)-5) == 1) codeWordArrayBeforLR[j]^=32;
            if(output.at(7+(j*8)-6) == 1) codeWordArrayBeforLR[j]^=64;
            if(output.at(7+(j*8)-7) == 1) codeWordArrayBeforLR[j]^=128;
        }
	}

	// Cleanup - erase the input/output vectors
	input.erase(input.begin(), input.begin() + input.size());
	output.erase(output.begin(), output.begin() + output.size());

	i = 0;

	// Try to open the output file for the helperdata
	if ((fd = fopen(item->output_HD_name, "wb")) == NULL) {
		fclose(fd);
        return 11;
    }

	// Perform Linear Repetition and XOR with PUF Response
    if(item->LR == 7){
        while( i < encode_binary_length){
            if((codeWordArrayBeforLR[i] & 0x80) == 0x00){                       // MSB (bit at position 8 == 0)
                encodeLR[(i*item->LR)] = 0x00;
            }
            else {                                                              // MSB (bit at position 8 == 1)
                encodeLR[(i*item->LR)] = 0xFE;
            }
            if((codeWordArrayBeforLR[i] & 0x40) == 0x00){                       // (bit at position 7 == 0)
                encodeLR[(i*item->LR)+1] = 0x00;
            } else {                                                            // (bit at position 7 == 1)
                encodeLR[(i*item->LR)] = encodeLR[(i*item->LR)] ^ 0x01;
                encodeLR[(i*item->LR)+1] = 0xFC;
            }
            if((codeWordArrayBeforLR[i] & 0x20) == 0x00){                       // (bit at position 6 == 0)
                encodeLR[(i*item->LR)+2] = 0x00;
            } else {                                                            // (bit at position 6 == 1)
                encodeLR[(i*item->LR)+1] = encodeLR[(i*item->LR)+1] ^ 0x03;
                encodeLR[(i*item->LR)+2] = 0xF8;
            }
            if((codeWordArrayBeforLR[i] & 0x10) == 0x00){                       // (bit at position 5 == 0)
                encodeLR[(i*item->LR)+3] = 0x00;
            } else {                                                            // (bit at position 5 == 1)
                encodeLR[(i*item->LR)+2] = encodeLR[(i*item->LR)+2] ^ 0x07;
                encodeLR[(i*item->LR)+3] = 0xF0;
            }
            if((codeWordArrayBeforLR[i] & 0x08) == 0x00){                       // (bit at position 4 == 0)
                encodeLR[(i*item->LR)+4] = 0x00;
            } else {                                                            // (bit at position 4 == 1)
                encodeLR[(i*item->LR)+3] = encodeLR[(i*item->LR)+3] ^ 0x0F;
                encodeLR[(i*item->LR)+4] = 0xE0;
            }
            if((codeWordArrayBeforLR[i] & 0x04) == 0x00){                       // (bit at position 3 == 0)
                encodeLR[(i*item->LR)+5] = 0x00;
            } else {                                                            // (bit at position 3 == 1)
                encodeLR[(i*item->LR)+4] = encodeLR[(i*item->LR)+4] ^ 0x1F;
                encodeLR[(i*item->LR)+5] = 0xC0;
            }
            if((codeWordArrayBeforLR[i] & 0x02) == 0x00){                       // (bit at position 2 == 0)
                encodeLR[(i*item->LR)+6] = 0x00;
            } else {                                                            // (bit at position 2 == 1)
                encodeLR[(i*item->LR)+5] = encodeLR[(i*item->LR)+5] ^ 0x3F;
                encodeLR[(i*item->LR)+6] = 0x80;
            }
            if((codeWordArrayBeforLR[i] & 0x01) == 0x00){                       // (bit at position 1 == 0)
                encodeLR[(i*item->LR)+6] = encodeLR[(i*item->LR)+6] ^ 0x00;
            } else {                                                            // (bit at position 1 == 1
                encodeLR[(i*item->LR)+6] = encodeLR[(i*item->LR)+6] ^ 0x7F;
            }
            encodeLR[(i*item->LR)] = (encodeLR[(i*item->LR)] ^ puf[(i*item->LR)]);
            encodeLR[(i*item->LR)+1] = (encodeLR[(i*item->LR)+1] ^ puf[(i*item->LR)+1]);
            encodeLR[(i*item->LR)+2] = (encodeLR[(i*item->LR)+2] ^ puf[(i*item->LR)+2]);
            encodeLR[(i*item->LR)+3] = (encodeLR[(i*item->LR)+3] ^ puf[(i*item->LR)+3]);
            encodeLR[(i*item->LR)+4] = (encodeLR[(i*item->LR)+4] ^ puf[(i*item->LR)+4]);
            encodeLR[(i*item->LR)+5] = (encodeLR[(i*item->LR)+5] ^ puf[(i*item->LR)+5]);
            encodeLR[(i*item->LR)+6] = (encodeLR[(i*item->LR)+6] ^ puf[(i*item->LR)+6]);

            // Write each byte into the output file
            for(j = 0; j < item->LR; j++){
               fwrite(&encodeLR[(i*item->LR)+j], sizeof(char), 1, fd);
            }

            i++;
        }
    }
    else if(item->LR == 15){
		while( i < encode_binary_length){
            if((codeWordArrayBeforLR[i] & 0x80) == 0x00){                        // MSB (bit at position 8 == 0)
                encodeLR[(i*item->LR)] = 0x00;
                encodeLR[((i*item->LR)+1)] = 0x00;
            }
            else {                                                               // MSB (bit at position 8 == 1)
                encodeLR[(i*item->LR)] = 0xFF;
                encodeLR[((i*item->LR)+1)] = 0xFE;
            }
            if((codeWordArrayBeforLR[i] & 0x40) == 0x00){                        // (bit at position 7 == 0)
                encodeLR[((i*item->LR)+2)] = 0x00;
                encodeLR[((i*item->LR)+3)] = 0x00;
            } else {                                                             // (bit at position 7 == 1)
                encodeLR[((i*item->LR)+1)] = encodeLR[((i*item->LR)+1)] ^ 0x01;
                encodeLR[((i*item->LR)+2)] = 0xFF;
                encodeLR[((i*item->LR)+3)] = 0xFC;
            }
            if((codeWordArrayBeforLR[i] & 0x20) == 0x00){                        // (bit at position 6 == 0)
                encodeLR[((i*item->LR)+4)] = 0x00;
                encodeLR[((i*item->LR)+5)] = 0x00;
            } else {                                                             // (bit at position 6 == 1)
                encodeLR[((i*item->LR)+3)] = encodeLR[((i*item->LR)+3)] ^ 0x03;
                encodeLR[((i*item->LR)+4)] = 0xFF;
                encodeLR[((i*item->LR)+5)] = 0xF8;
            }
            if((codeWordArrayBeforLR[i] & 0x10) == 0x00){                        // (bit at position 5 == 0)
                encodeLR[((i*item->LR)+6)] = 0x00;
                encodeLR[((i*item->LR)+7)] = 0x00;
            } else {                                                             // (bit at position 5 == 1)
                encodeLR[((i*item->LR)+5)] = encodeLR[((i*item->LR)+5)] ^ 0x07;
                encodeLR[((i*item->LR)+6)] = 0xFF;
                encodeLR[((i*item->LR)+7)] = 0xF0;
            }
            if((codeWordArrayBeforLR[i] & 0x08) == 0x00){                        // (bit at position 4 == 0)
                encodeLR[((i*item->LR)+8)] = 0x00;
                encodeLR[((i*item->LR)+9)] = 0x00;
            } else {                                                             // (bit at position 4 == 1)
                encodeLR[((i*item->LR)+7)] = encodeLR[((i*item->LR)+7)] ^ 0x0F;
                encodeLR[((i*item->LR)+8)] = 0xFF;
                encodeLR[((i*item->LR)+9)] = 0xE0;
            }
            if((codeWordArrayBeforLR[i] & 0x04) == 0x00){                        // (bit at position 3 == 0)
                encodeLR[((i*item->LR)+10)] = 0x00;
                encodeLR[((i*item->LR)+11)] = 0x00;
            } else {                                                             // (bit at position 3 == 1)
                encodeLR[((i*item->LR)+9)] = encodeLR[((i*item->LR)+9)] ^ 0x1F;
                encodeLR[((i*item->LR)+10)] = 0xFF;
                encodeLR[((i*item->LR)+11)] = 0xC0;
            }
            if((codeWordArrayBeforLR[i] & 0x02) == 0x00){                        // (bit at position 2 == 0)
                encodeLR[((i*item->LR)+12)] = 0x00;
                encodeLR[((i*item->LR)+13)] = 0x00;
            } else {                                                           	 // (bit at position 2 == 1)
                encodeLR[((i*item->LR)+11)] = encodeLR[((i*item->LR)+11)] ^ 0x3F;
                encodeLR[((i*item->LR)+12)] = 0xFF;
                encodeLR[((i*item->LR)+13)] = 0x80;
            }
            if((codeWordArrayBeforLR[i] & 0x01) == 0x00){                        // (bit at position 1 == 0)
                encodeLR[((i*item->LR)+13)] = encodeLR[((i*item->LR)+13)] ^ 0x00;
                encodeLR[((i*item->LR)+14)] = 0x00;
            } else {                                                             // (bit at position 1 == 1
                encodeLR[((i*item->LR)+13)] = encodeLR[((i*item->LR)+13)] ^ 0x7F;
                encodeLR[((i*item->LR)+14)] = 0xFF;
            }

            encodeLR[(i*item->LR)] = (encodeLR[(i*item->LR)] ^ puf[(i*item->LR)]);
            encodeLR[(i*item->LR)+1] = (encodeLR[(i*item->LR)+1] ^ puf[(i*item->LR)+1]);
            encodeLR[(i*item->LR)+2] = (encodeLR[(i*item->LR)+2] ^ puf[(i*item->LR)+2]);
            encodeLR[(i*item->LR)+3] = (encodeLR[(i*item->LR)+3] ^ puf[(i*item->LR)+3]);
            encodeLR[(i*item->LR)+4] = (encodeLR[(i*item->LR)+4] ^ puf[(i*item->LR)+4]);
            encodeLR[(i*item->LR)+5] = (encodeLR[(i*item->LR)+5] ^ puf[(i*item->LR)+5]);
            encodeLR[(i*item->LR)+6] = (encodeLR[(i*item->LR)+6] ^ puf[(i*item->LR)+6]);
            encodeLR[(i*item->LR)+7] = (encodeLR[(i*item->LR)+7] ^ puf[(i*item->LR)+7]);
            encodeLR[(i*item->LR)+8] = (encodeLR[(i*item->LR)+8] ^ puf[(i*item->LR)+8]);
            encodeLR[(i*item->LR)+9] = (encodeLR[(i*item->LR)+9] ^ puf[(i*item->LR)+9]);
            encodeLR[(i*item->LR)+10] = (encodeLR[(i*item->LR)+10] ^ puf[(i*item->LR)+10]);
            encodeLR[(i*item->LR)+11] = (encodeLR[(i*item->LR)+11] ^ puf[(i*item->LR)+11]);
            encodeLR[(i*item->LR)+12] = (encodeLR[(i*item->LR)+12] ^ puf[(i*item->LR)+12]);
            encodeLR[(i*item->LR)+13] = (encodeLR[(i*item->LR)+13] ^ puf[(i*item->LR)+13]);
            encodeLR[(i*item->LR)+14] = (encodeLR[(i*item->LR)+14] ^ puf[(i*item->LR)+14]);

            // Write each byte into the output file
            for(j = 0; j < item->LR; j++){
               fwrite(&encodeLR[(i*item->LR)+j], sizeof(char), 1, fd);
            }

			i++;
        }
    }

    // Prepare the pattern with essential informations for the decoding
    infos.errorCode = 1;
    infos.Golay_k_BCH_m = m;
    infos.Golay_BCH_length = length;
    infos.Golay_d_BCH_t = t;
    infos.linearRep = item->LR;
    infos.puf_offSet = item->offSet;
    infos.original_filesize = filesize;

    // Add the pattern to the output file
    fwrite(&infos, sizeof(infos), 1, fd);

    // Cleanup
    fclose(fd);
    free(puf);
    free(encodeLR);
    free(codeWordArrayBeforLR);

	return 0;
}

/*
 * BCH-Decoder - Calculation
 *
 * Author: Sebastian Schurig
 *
 * Calculation Functions for the BCH-Decoder
 *
 */


void read_p_decode()
/*
 *	Read m, the degree of a primitive polynomial p(x) used to compute the
 *	Galois field GF(2**m). Get precomputed coefficients p[] of p(x). Read
 *	the code length.
 */
{
	int			    i;


    // Generate coefficients of a primitive polynomial used to generate GF(2**m)
    for (i=1; i<m; i++)
            p[i] = 0;

    p[0] = p[m] = 1;

    if 		(m == 4)	p[1] = 1;
    else if (m == 5)	p[2] = 1;
    else if (m == 6)	p[1] = 1;
    else if (m == 7)	p[1] = 1;
    else if (m == 8)	p[4] = p[5] = p[6] = 1;
    else if (m == 9)	p[4] = 1;
    else if (m == 10)	p[3] = 1;
    else if (m == 11)	p[2] = 1;
    else if (m == 12)	p[3] = p[4] = p[7] = 1;
    else if (m == 13)	p[1] = p[3] = p[4] = 1;
    else if (m == 14)	p[1] = p[11] = p[12] = 1;

    // Calculate 'n'
    n = 1;
    for (i = 0; i <= m; i++) {
        n *= 2;
    }

    // Calculate the supported BCH code length, limited by "m"
    n = n / 2 - 1;
}

void gen_poly_decode()
/*
 * Compute the generator polynomial of a binary BCH code. Fist generate the
 * cycle sets modulo 2**m - 1, cycle[][] =  (i, 2*i, 4*i, ..., 2^l*i). Then
 * determine those cycle sets that contain integers in the set of (d-1)
 * consecutive integers {1..(d-1)}. The generator polynomial is calculated
 * as the product of linear factors of the form (x+alpha^i), for every i in
 * the above cycle sets.
 */
{
	register int	ii, jj, ll, kaux, zero;
	register int	test, aux, nocycles, root, noterms, rdncy;
	int      cycle[8192][21];
	std::vector<int> size(8192);
    std::vector<int> min(2048);
    std::vector<int> zeros(16385);
    char processing[3] = {' ',' ','\0'};


	/* Generate cycle sets modulo n, n = 2**m - 1 */
	cycle[0][0] = 0;
	size[0] = 1;
	cycle[1][0] = 1;
	size[1] = 1;
	jj = 1;			/* cycle set index */

	if (m > 9)  {

		printf("\nComputing cycle sets modulo %d\n", n);
		printf("(This may take some time)...\n\n");
	}
	do {
		/* Generate the jj-th cycle set */
		ii = 0;
		do {
			ii++;
			cycle[jj][ii] = (cycle[jj][ii - 1] * 2) % n;
			size[jj]++;
			aux = (cycle[jj][ii] * 2) % n;
		} while (aux != cycle[jj][0]);

		/* Next cycle set representative */
		ll = 0;
		do {
			ll++;
			test = 0;
			for (ii = 1; ((ii <= jj) && (!test)); ii++)
			/* Examine previous cycle sets */
			  for (kaux = 0; ((kaux < size[ii]) && (!test)); kaux++)
			     if (ll == cycle[ii][kaux])
			        test = 1;
		} while ((test) && (ll < (n - 1)));
		if (!(test)) {
			jj++;	/* next cycle set index */
			cycle[jj][0] = ll;
			size[jj] = 1;
		}

    zero = ((n-1)/100);

    // Status progress
    if(zero != 0 && (ll/zero) <= 100) cout << "\r" << setw(-20) << StatusProgress((ll/zero)) << " " << (ll/zero) << "% completed." << processing << flush;
        else if(zero != 0 && (ll/zero) >= 100) cout << "\r" << setw(-20) << StatusProgress(100) << " " << 100 << "% completed.  " << flush;

    if(processing[0] == ' ' && processing[1] == ' '){
            processing[0] = '.';
    }
    else if(processing[0] == '.' && processing[1] == ' '){
        processing[1] = '.';
    }
    else if(processing[0] == '.' && processing[1] == '.') {
       processing[0] = ' ';
       processing[1] = ' ';
    }

	} while (ll < (n - 1));
	nocycles = jj;		/* number of cycle sets modulo n */

    d = 2 * t + 1;

    /* Search for roots 1, 2, ..., d-1 in cycle sets */
    kaux = 0;
    rdncy = 0;
    for (ii = 1; ii <= nocycles; ii++) {
        min[kaux] = 0;
        test = 0;
        for (jj = 0; ((jj < size[ii]) && (!test)); jj++)
            for (root = 1; ((root < d) && (!test)); root++)
                if (root == cycle[ii][jj])  {
                    test = 1;
                    min[kaux] = ii;
                }
        if (min[kaux]) {
            rdncy += size[min[kaux]];
            kaux++;
        }
    }
    noterms = kaux;
    kaux = 1;
    for (ii = 0; ii < noterms; ii++)
        for (jj = 0; jj < size[min[ii]]; jj++) {
            zeros[kaux] = cycle[min[ii]][jj];
            kaux++;
        }

    k = length - rdncy;

	/* Compute the generator polynomial */
	g[0] = alpha_to[zeros[1]];
	g[1] = 1;		/* g(x) = (X + zeros[1]) initially */
	for (ii = 2; ii <= rdncy; ii++) {
	  g[ii] = 1;
	  for (jj = ii - 1; jj > 0; jj--)
	    if (g[jj] != 0)
	      g[jj] = g[jj - 1] ^ alpha_to[(index_of[g[jj]] + zeros[ii]) % n];
	    else
	      g[jj] = g[jj - 1];
	  g[0] = alpha_to[(index_of[g[0]] + zeros[ii]) % n];
	}
}

void decode_bch()
/*
 * Simon Rockliff's implementation of Berlekamp's algorithm.
 *
 * Assume we have received bits in recd[i], i=0..(n-1).
 *
 * Compute the 2*t syndromes by substituting alpha^i into rec(X) and
 * evaluating, storing the syndromes in s[i], i=1..2t (leave s[0] zero) .
 * Then we use the Berlekamp algorithm to find the error location polynomial
 * elp[i].
 *
 * If the degree of the elp is >t, then we cannot correct all the errors, and
 * we have detected an uncorrectable error pattern. We output the information
 * bits uncorrected.
 *
 * If the degree of elp is <=t, we substitute alpha^i , i=1..n into the elp
 * to get the roots, hence the inverse roots, the error location numbers.
 * This step is usually called "Chien's search".
 *
 * If the number of errors located is not equal the degree of the elp, then
 * the decoder assumes that there are more than t errors and cannot correct
 * them, only detect them. We output the information bits uncorrected.
 */
{
	register int    i, j;
	register int    u, q;
	register int    t2;
	register int    count, syn_error;
	static int      elp[8194][8192];

    std::vector<int> d(8194);
    std::vector<int> l(8194);
	std::vector<int> u_lu(8194);
	std::vector<int> s(8192);
	std::vector<int> root(4096);
	std::vector<int> loc(4096);
	std::vector<int> err(8192);
	std::vector<int> reg(4097);


	count = 0;
	syn_error = 0;
	t2 = 2 * t;

	/* first form the syndromes */
	for (i = 1; i <= t2; i++) {
		s[i] = 0;
		for (j = 0; j < length; j++)
			if (recd[j] != 0)
				s[i] ^= alpha_to[(i * j) % n];
		if (s[i] != 0)
			syn_error = 1; /* set error flag if non-zero syndrome */
/*
 * Note:    If the code is used only for ERROR DETECTION, then
 *          exit program here indicating the presence of errors.
 */
		/* convert syndrome from polynomial form to index form  */
		s[i] = index_of[s[i]];
	}

	if (syn_error) {	/* if there are errors, try to correct them */
		/*
		 * Compute the error location polynomial via the Berlekamp
		 * iterative algorithm. Following the terminology of Lin and
		 * Costello's book :   d[u] is the 'mu'th discrepancy, where
		 * u='mu'+1 and 'mu' (the Greek letter!) is the step number
		 * ranging from -1 to 2*t (see L&C),  l[u] is the degree of
		 * the elp at that step, and u_l[u] is the difference between
		 * the step number and the degree of the elp.
		 */
		/* initialise table entries */
		d[0] = 0;			/* index form */
		d[1] = s[1];		/* index form */
		elp[0][0] = 0;		/* index form */
		elp[1][0] = 1;		/* polynomial form */
		for (i = 1; i < t2; i++) {
			elp[0][i] = -1;	/* index form */
			elp[1][i] = 0;	/* polynomial form */
		}
		l[0] = 0;
		l[1] = 0;
		u_lu[0] = -1;
		u_lu[1] = 0;
		u = 0;

		do {
			u++;
			if (d[u] == -1) {
				l[u + 1] = l[u];
				for (i = 0; i <= l[u]; i++) {
					elp[u + 1][i] = elp[u][i];
					elp[u][i] = index_of[elp[u][i]];
				}
			} else
				/*
				 * search for words with greatest u_lu[q] for
				 * which d[q]!=0
				 */
			{
				q = u - 1;
				while ((d[q] == -1) && (q > 0))
					q--;
				/* have found first non-zero d[q]  */
				if (q > 0) {
				  j = q;
				  do {
				    j--;
				    if ((d[j] != -1) && (u_lu[q] < u_lu[j]))
				      q = j;
				  } while (j > 0);
				}

				/*
				 * have now found q such that d[u]!=0 and
				 * u_lu[q] is maximum
				 */
				/* store degree of new elp polynomial */
				if (l[u] > l[q] + u - q)
					l[u + 1] = l[u];
				else
					l[u + 1] = l[q] + u - q;

				/* form new elp(x) */
				for (i = 0; i < t2; i++)
					elp[u + 1][i] = 0;
				for (i = 0; i <= l[q]; i++)
					if (elp[q][i] != -1)
						elp[u + 1][i + u - q] =
                                   alpha_to[(d[u] + n - d[q] + elp[q][i]) % n];
				for (i = 0; i <= l[u]; i++) {
					elp[u + 1][i] ^= elp[u][i];
					elp[u][i] = index_of[elp[u][i]];
				}
			}
			u_lu[u + 1] = u - l[u + 1];

			/* form (u+1)th discrepancy */
			if (u < t2) {
			/* no discrepancy computed on last iteration */
			  if (s[u + 1] != -1)
			    d[u + 1] = alpha_to[s[u + 1]];
			  else
			    d[u + 1] = 0;
			    for (i = 1; i <= l[u + 1]; i++)
			      if ((s[u + 1 - i] != -1) && (elp[u + 1][i] != 0))
			        d[u + 1] ^= alpha_to[(s[u + 1 - i]
			                      + index_of[elp[u + 1][i]]) % n];
			  /* put d[u+1] into index form */
			  d[u + 1] = index_of[d[u + 1]];
			}
		} while ((u < t2) && (l[u + 1] <= t));

		u++;

		if (l[u] <= t) {/* Can correct errors */
			/* put elp into index form */
			for (i = 0; i <= l[u]; i++)
				elp[u][i] = index_of[elp[u][i]];

			// Detected errors and correction
			for (i = 0; i <= l[u]; i++){
                if(elp[u][i] > 0) numerr++;
			}

			/* Chien search: find roots of the error location polynomial */
			for (i = 1; i <= l[u]; i++)
				reg[i] = elp[u][i];
			count = 0;
			for (i = 1; i <= n; i++) {
				q = 1;
				for (j = 1; j <= l[u]; j++)
					if (reg[j] != -1) {
						reg[j] = (reg[j] + j) % n;
						q ^= alpha_to[reg[j]];
					}
				if (!q) {	/* store root and error
						 * location number indices */
					root[count] = i;
					loc[count] = n - i;
					count++;
				}
			}
			if (count == l[u]){
			/* no. roots = degree of elp hence <= t errors */
				for (i = 0; i < l[u]; i++){
                    recd[loc[i]] ^= 1;
                    corerr++;
				}
            }
			else {
                /* elp has degree >t hence cannot solve */
				decerror++;
			}
		}
	}
}

int MajorityVoting(unsigned long pos, struct Item *item)
/*
 * Majority Vote
 *
 * Inputs:
 * pos = the actual bit position which will be checked
 * item = pointer to the struct to store the necessary informations
 *
 */
{
	int c = 0;
	int zero = 0;
	int one = 0;
	unsigned long posi = pos;


    while(c < item->LR){
       if( ((posi >> ((item->LR-1)-c)) & 0x00000001) == 1){
           one++;
           }
        else{
            zero++;
        }
        c++;
    }

    if(zero > one) return 0;
    else return 1;
}

int Calculation_decode(struct Item *item)
/*
 * Function to handle the actual calculation:
 *
 * -> Read in the HelperData
 * -> Extract the Settings (BCHmode, LR factor, encoded length and original Key filesize
 * -> Perform the XOR of the PUF with the HelperData
 * -> Majority vote to recover encoded codewords
 * -> Perform the decoding
 * -> Store the recovered Key into a file.
 *
 * Inputs:
 * item  = pointer to the struct to get/store the necessary informations
 */
{
    FILE *fd;
    long HD_filesize;
    long len;
    long encode_binary_length;
    unsigned char hex;
    unsigned char recLR;
    unsigned long pos8 = 0, pos7 = 0 ,pos6 = 0 , pos5 = 0, pos4 = 0, pos3 = 0, pos2 = 0, pos1 = 0;
    int bit0 = 0,  bit1 = 0,  bit2 = 0,  bit3 = 0,  bit4 = 0,  bit5 = 0,  bit6 = 0,  bit7 = 0;
    int i = 0, j = 0, u = 0, z = 0;
    struct Pattern infos;
    int error = 0;
    stringstream BCH;
    string bch_mode;
    char processing[3] = {' ',' ','\0'};

    // Check if HD Input file is a file
    if(!IsFile(item->input_HD_name)){
        return 10;
    }

    // Check if PUF Input file is a file
    if(!IsFile(item->input_PUF_name)){
        return 9;
    }

	// Try to open the HelperData file
    if ((fd = fopen(item->input_HD_name, "rb")) == NULL) {
		fclose(fd);
        return 11;
    }

    // Get the file size
    fseek(fd, 0, SEEK_END);
    HD_filesize = ftell(fd);
    rewind(fd);

    // Allocate space for the helperData to read it in
    unsigned char *helperData = (unsigned char *) malloc(sizeof(char) * (HD_filesize - sizeof(infos)));

    // Read the complete HelperData
    if (fread(&helperData[0], sizeof(char), (HD_filesize - sizeof(infos)), fd) != (HD_filesize - sizeof(infos))) {
		fclose(fd);
        return 12;
    }

    // Go to the end of the HelperData, at the start position of the Pattern struct
    fseek(fd, (HD_filesize - sizeof(infos)), SEEK_SET);

    // Read the coding informations (pattern)
    if ((signed)fread(&infos, sizeof(infos), 1, fd) != 1) {
		fclose(fd);
        return 12;
    }

    // Close the HelperData file
    fclose(fd);

    // Set the relevant informations for the decoding
    m = infos.Golay_k_BCH_m;
    t = infos.Golay_d_BCH_t;
    length = infos.Golay_BCH_length;
    item->LR = infos.linearRep;
    item->offSet = infos.puf_offSet;
    filesize = infos.original_filesize;

    // Check if all settings and informations are valid
    if(!(m >= 4 && m <= 14) ) return 14;
    else if(!(t >= 0 && t <= 4095))  return 15;
    else if(!(length >= 7 && length <= 16383))  return 16;
    else if(!(item->LR == 7 || item->LR == 15))  return 17;
    else if((item->offSet < 0 || item->offSet >= (signed)(HD_filesize - sizeof(infos))))  return 18;
    else if((filesize < 0 || filesize >= (signed)(HD_filesize - sizeof(infos))))  return 19;

    // Signal that all Settings are set and look valid
    isSet = 1;

    // BCH Mode calculation
    read_p();               /* Set primitive polynomial p(x) */
	generate_gf();          /* Construct the Galois Field GF(2**m) */
	gen_poly();             /* Compute the generator polynomial of BCH code */

    BCH << "BCH(" << length << ", " << k << ", " << d << ")";
    bch_mode = BCH.str();
    strcpy(item->BCHmode, bch_mode.c_str());
    BCH.str("");
    BCH.clear();

    /* Calculate length for the processing
     * Check if the original file size is a multiplicative to the chosen factor 'k' of the BCH mode
     *
     * len = length of the input before the bch coding (as integer [0 or 1] vector)
     *
     */
    if( (filesize * 8) % k != 0){

        // If it is not a multiplicative to the chosen factor 'k' of the BCH mode, determine the new length
        len = (((filesize * 8)/k)+1)*k;
    }
    else {

        // If it is a multiplicative to the chosen factor 'k' of the BCH mode use it
        len = (filesize * 8);
    }

    /* Check if the encoded key is byte block aligned
     *
     * encode_binary_length = length of bch encoded representation as byte blocks
     *
     */
    if (((len/k)*length)%8 != 0){

        // If not set the correct length
        encode_binary_length = (((len/k)*length) / 8)+1;
    }
    else {

        // If use the corresponding length
        encode_binary_length = ((len/k)*length) / 8;
    }

    // Try to open the PUF file
    if ((fd = fopen(item->input_PUF_name, "rb")) == NULL) {
		fclose(fd);
        return 8;
    }

    // Go to the offset position in the file
    fseek(fd, item->offSet, SEEK_SET);

    // Allocate space for the PUF
    unsigned char *sramData = (unsigned char *) malloc(sizeof(char) * (encode_binary_length * item->LR));

    // Allocate space for the BCH encoded byte stream
    unsigned char *recoverdFromLR = (unsigned char *) malloc(sizeof(char) * encode_binary_length);

    // Read the needed PUF Data
    if ((signed)fread(&sramData[0], sizeof(char), (encode_binary_length * item->LR), fd) != (encode_binary_length * item->LR)) {
		fclose(fd);
        return 7;
    }

    // Close file
    fclose(fd);

    // Perform the XOR operation of the HelperData and the PUF sramData and do the Majority Vote (LR = 7)to recover the BCH encoded byte stream
    if(item->LR == 7){
        while (j < encode_binary_length){
			recLR = 0x00;
			pos8 = (helperData[(j*item->LR)] ^ sramData[(j*item->LR)]) >> 1;
                    if(MajorityVoting(pos8, item)) {
                        recLR = 0x80;
                    }
            pos7 = ((helperData[((j*item->LR)+1)] ^ sramData[(j*item->LR)+1]) >> 2) ^ (((helperData[(j*item->LR)] ^ sramData[(j*item->LR)]) & 0x01) << 6);
                    if(MajorityVoting(pos7, item)){
                        recLR = (recLR ^ 0x40);
                    }
            pos6 = ((helperData[((j*item->LR)+2)] ^ sramData[(j*item->LR)+2]) >> 3) ^ (((helperData[((j*item->LR)+1)] ^ sramData[(j*item->LR)+1]) & 0x03) << 5);
                    if(MajorityVoting(pos6, item)){
                        recLR = (recLR ^ 0x20);
                    }
            pos5 = ((helperData[((j*item->LR)+3)] ^ sramData[(j*item->LR)+3]) >> 4) ^ (((helperData[((j*item->LR)+2)] ^ sramData[(j*item->LR)+2]) & 0x07) << 4);
                    if(MajorityVoting(pos5, item)){
                        recLR = (recLR ^ 0x10);
                    }
            pos4 = ((helperData[((j*item->LR)+4)] ^ sramData[(j*item->LR)+4]) >> 5) ^ (((helperData[((j*item->LR)+3)] ^ sramData[(j*item->LR)+3]) & 0x0F) << 3);
                    if(MajorityVoting(pos4, item)){
                        recLR = (recLR ^ 0x08);
                    }
            pos3 = ((helperData[((j*item->LR)+5)] ^ sramData[(j*item->LR)+5]) >> 6) ^ (((helperData[((j*item->LR)+4)] ^ sramData[(j*item->LR)+4]) & 0x1F) << 2);
                    if(MajorityVoting(pos3, item)){
                        recLR = (recLR ^ 0x04);
                    }
            pos2 = ((helperData[((j*item->LR)+6)] ^ sramData[(j*item->LR)+6]) >> 7) ^ (((helperData[((j*item->LR)+5)] ^ sramData[(j*item->LR)+5]) & 0x3F) << 1);
                    if(MajorityVoting(pos2, item)){
                        recLR = (recLR ^ 0x02);
                    }
            pos1 = ((helperData[((j*item->LR)+6)] ^ sramData[(j*item->LR)+6]) & 0x7F);
                    if(MajorityVoting(pos1, item)) {
                        recLR = (recLR ^ 0x01);
                    }
            // Set the recovered bits
            recoverdFromLR[j] = recLR;

            j++;
        }
    }
    // Perform the XOR operation of the HelperData and the PUF sramData and do the Majority Vote (LR = 15)to recover the BCH encoded byte stream
    else if(item->LR == 15){
        while (j < encode_binary_length){
            recLR = 0x00;
            pos8 = ((helperData[(j*item->LR)] ^ sramData[(j*item->LR)]) << 7) ^ ((helperData[(j*item->LR)+1] ^ sramData[(j*item->LR)+1]) >> 1);
                if(MajorityVoting(pos8, item)) {
                    recLR = 0x80;
                }
            pos7 = ((((helperData[(j*item->LR)+1] ^ sramData[(j*item->LR)+1]) & 0x01) << 14) ^ ((helperData[((j*item->LR)+2)] ^ sramData[(j*item->LR)+2]) << 6) ^ ((helperData[(j*item->LR)+3] ^ sramData[(j*item->LR)+3]) >> 2));
                if(MajorityVoting(pos7, item)){
                    recLR = (recLR ^ 0x40);
                }
            pos6 = ((((helperData[(j*item->LR)+3] ^ sramData[(j*item->LR)+3]) & 0x03) << 13) ^ ((helperData[((j*item->LR)+4)] ^ sramData[(j*item->LR)+4]) << 5) ^ ((helperData[(j*item->LR)+5] ^ sramData[(j*item->LR)+5]) >> 3));
                if(MajorityVoting(pos6, item)){
                    recLR = (recLR ^ 0x20);
                }
            pos5 = ((((helperData[(j*item->LR)+5] ^ sramData[(j*item->LR)+5]) & 0x07) << 12) ^ ((helperData[((j*item->LR)+6)] ^ sramData[(j*item->LR)+6]) << 4) ^ ((helperData[(j*item->LR)+7] ^ sramData[(j*item->LR)+7]) >> 4));
                if(MajorityVoting(pos5, item)){
                    recLR = (recLR ^ 0x10);
                }
            pos4 = ((((helperData[(j*item->LR)+7] ^ sramData[(j*item->LR)+7]) & 0x0F) << 11) ^ ((helperData[((j*item->LR)+8)] ^ sramData[(j*item->LR)+8]) << 3) ^ ((helperData[(j*item->LR)+9] ^ sramData[(j*item->LR)+9]) >> 5));
                if(MajorityVoting(pos4, item)){
                    recLR = (recLR ^ 0x08);
                }
            pos3 = ((((helperData[(j*item->LR)+9] ^ sramData[(j*item->LR)+9]) & 0x1F) << 10) ^ ((helperData[((j*item->LR)+10)] ^ sramData[(j*item->LR)+10]) << 2) ^ ((helperData[(j*item->LR)+11] ^ sramData[(j*item->LR)+11]) >> 6));
                if(MajorityVoting(pos3, item)){
                    recLR = (recLR ^ 0x04);
                }
            pos2 = ((((helperData[(j*item->LR)+11] ^ sramData[(j*item->LR)+11]) & 0x3F) << 9) ^ ((helperData[((j*item->LR)+12)] ^ sramData[(j*item->LR)+12]) << 1) ^ ((helperData[(j*item->LR)+13] ^ sramData[(j*item->LR)+13]) >> 7));
                if(MajorityVoting(pos2, item)){
                    recLR = (recLR ^ 0x02);
                }
            pos1 = ((((helperData[(j*item->LR)+13] ^ sramData[(j*item->LR)+13]) & 0x7F) << 8) ^ (helperData[(j*item->LR)+14] ^ sramData[(j*item->LR)+14])) ;
                if(MajorityVoting(pos1, item)) {
                    recLR = (recLR ^ 0x01);
                }
            // Set the recovered bits
            recoverdFromLR[j] = recLR;
            j++;
        }
    }

    // Generate a binary bit stream (for a integer vector) out of each byte block of the input file and store it in the data vector
    for(j = 0; j < encode_binary_length; j++){
        hex = recoverdFromLR[j];

        // Get the binary value for each bit position of an byte and insert it in the correct order into the input vector
        bit7 = hex&1;
        hex >>= 1;
        bit6 = hex&1;
        hex >>= 1;
        bit5 = hex&1;
        hex >>= 1;
        bit4 = hex&1;
        hex >>= 1;
        bit3 = hex&1;
        hex >>= 1;
        bit2 = hex&1;
        hex >>= 1;
        bit1 = hex&1;
        hex >>= 1;
        bit0 = hex&1;
        input.push_back(bit0);
        input.push_back(bit1);
        input.push_back(bit2);
        input.push_back(bit3);
        input.push_back(bit4);
        input.push_back(bit5);
        input.push_back(bit6);
        input.push_back(bit7);
    }

    // Cleanup
    free(helperData);
    free(sramData);
    free(recoverdFromLR);

	// Calculate the length of the binary representation (integer array with binary values) based on the file size of the encoded file
    len = (encode_binary_length * 8);
    u = (len / 100);

    // Perform the actual DECODING
    cout << endl << endl << "Calculating BCH Decoding:" << endl << "(This may take some time - depending on the used BCH(mode) and file size)..." << endl << endl;

	// Process a complete file
    for(j = 0; j < len; j = j+length){

        // Fill the recd vector with the input values
        for(i = 0; i < length; i++){
            if( i + j < (signed)input.size()){
                recd[i] = input[i+j];
            }
            else {
                recd[i] = 0;
            }
        }

		// Decoding of one codeword (placed in the beginning of the recv vector)
		decode_bch();             /* DECODE received codeword recv[] */

        // Store the recovered original into the data vector
		for (i = 0; i < k; i++) {
            output.push_back(recd[i+(length-k)]);
        }

		z = z + 1;

		// Processing status
        if ((j+length) >= len) cout << "\r" << setw(-20) << StatusProgress(100) << " " << 100 << "% completed.  " << flush;
        else cout << "\r" << setw(-20) << StatusProgress((j/u)) << " " << (j/u) << "% completed." << processing << flush;

        if(processing[0] == ' ' && processing[1] == ' '){
                processing[0] = '.';
        }
        else if(processing[0] == '.' && processing[1] == ' '){
            processing[1] = '.';
        }
        else if(processing[0] == '.' && processing[1] == '.') {
           processing[0] = ' ';
           processing[1] = ' ';
        }
	}

    // Save the complete decoded data (block wise) into a binary file
    error = SaveFile_bch(item);

    // Cleanup - erase the input/output vectors
	input.erase(input.begin(), input.begin() + input.size());
	output.erase(output.begin(), output.begin() + output.size());

    return error;
}
