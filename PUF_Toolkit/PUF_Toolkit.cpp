#include "PUF_Toolkit.h"

/*
 * PUF-ToolKit - Menu
 *
 * Author: Sebastian Schurig
 *
 * Menu for the PUF-ToolKit
 *
 */


void HammingWeight_Menu(struct Item *item)
/*
 * Console Menu for the Hamming Weight */
{
    char menuChoice[3];
    char *p;
    unsigned int ch;
    unsigned int error = 0;
    unsigned int exit = 0;
    unsigned int change = 0;

    while(true){
        change = 0;
        if(item->HW_ENTP_mode == 0){
            while(true){
                ClearScreen();
                cout << "*******************************************************************************" << endl;
                cout << "*                                                                             *" << endl;
                cout << "*                         Welcome to the PUF-Toolkit                          *" << endl;
                cout << "*                                                                             *" << endl;
                cout << "******************************* Hamming-Weight ********************************" << endl;
                cout << "*                                                                             *" << endl;
                cout << "*                        1 : Set File                                         *" << endl;
                cout << "*                        2 : Calculate Hamming Weight                         *" << endl;
                cout << "*                        3 : Save result                                      *" << endl;
                cout << "*                        4 : Change 'offset_begin' & 'offset_end'                       *" << endl;
                cout << "*                        5 : Switch mode (file / directory)                   *" << endl;
                cout << "*                        6 : Back                                             *" << endl;
                cout << "*                                                                             *" << endl;
                cout << "*******************************************************************************" << endl;
                cout << "          Settings:                                                            " << endl;
                cout << "                     offset_begin, offset_end  = " << item->offset_begin << ", " << item->offset_end    << endl;
                cout << "                     Length   = " << item->input_length << " byte (" << item->input_length*8 << " bit)" << endl;
                cout << "                     Mode     = single file                                    " << endl;
                cout << "                                                                               " << endl;
                cout << "                     Filename = " << item->input_file_name                       << endl;
                cout << "                                                                               " << endl;
                cout << "          Result:                                                              " << endl;
                cout << "                     HammingWeight = " << item->result                           << endl;
                cout << "*******************************************************************************" << endl;

                if(error) ErrorMessages(error, 0);
                error = 0;

                cout << endl << "Make a choice by typing in a number (1-6): ";

                if (fgets(menuChoice, sizeof(menuChoice), stdin)) {
                    /* fgets succeeds, scan for newline character */
                    p = strchr(menuChoice, '\n');
                    if (p) {
                        *p = '\0';
                        switch(menuChoice[0])
                            {
                            case '1':
                                cout << endl << " Processing : Set filename" << endl << endl;
                                DefineFilename(item, 1);
                                item->zeros = 0;
                                item->ones = 0;
                                error = HammingWeight(item, item->HW_ENTP_mode);
                                if(!error) {
                                    snprintf(item->result, 52, "%lu", item->ones); //itoa(item->ones,item->result,10);
                                    printf("prankur value at item->result: %s\n", item->result);
                                }
                                else strcpy(item->result, "none");
                                break;
                            case '2':
                                cout << endl << " Processing : HammingWeight calculation" << endl << endl;
                                error = HammingWeight(item, item->HW_ENTP_mode);
                                if(!error) snprintf(item->result, 52, "%lu", item->ones);//itoa(item->ones,item->result,10);
                                else strcpy(item->result, "none");
                                break;
                            case '3':
                                cout << endl << " Processing : Save result" << endl << endl;
                                if(strcmp(item->result, "none") == 0) error = 22;
                                else error = SaveFile(item,1);
                                break;
                            case '4':
                                cout << endl << " Change 'offset_begin' and 'offset_end'" << endl << endl;
                                item->zeros = 0;
                                item->ones = 0;
                                error = 0;
                                strcpy(item->result, "none");
                                ClearScreen();
                                DefineOffSetLength(item);
                                break;
                            case '5':
                                cout << endl << " Switch mode " << endl << endl;
                                item->HW_ENTP_mode = 1;
                                strcpy(item->result, "none");
                                strcpy(item->input_file_name, "none");
                                strcpy(item->output_file_name, "none");
                                change = 1;
                                break;
                            case '6':
                                cout << endl << " Back " << endl << endl;
                                error = 0;
                                exit = 1;
                                break;
                            default:
                                error = 15;
                                break;
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
                if(exit || change) break;
            }
        }
        else if(item->HW_ENTP_mode == 1){
            while(true){
                ClearScreen();
                cout << "*******************************************************************************" << endl;
                cout << "*                                                                             *" << endl;
                cout << "*                         Welcome to the PUF-Toolkit                          *" << endl;
                cout << "*                                                                             *" << endl;
                cout << "******************************* Hamming-Weight ********************************" << endl;
                cout << "*                                                                             *" << endl;
                cout << "*                        1 : Set Input-Path                                   *" << endl;
                cout << "*                        2 : Set Output-Filename                              *" << endl;
                cout << "*                        3 : Calculate Hamming Weight                         *" << endl;
                cout << "*                        4 : Change 'offset_begin' & 'offset_end'                       *" << endl;
                cout << "*                        5 : Switch mode (file / directory)                   *" << endl;
                cout << "*                        6 : Back                                             *" << endl;
                cout << "*                                                                             *" << endl;
                cout << "*******************************************************************************" << endl;
                cout << "          Settings:                                                            " << endl;
                cout << "                     offset_begin, offset_end  = " << item->offset_begin << ", " << item->offset_end    << endl;
                cout << "                     Length          = " << item->input_length << " byte (" << item->input_length*8 << " bit)" << endl;
                cout << "                     Mode            = complete directory                      " << endl;
                cout << "                                                                               " << endl;
                cout << "                     Input-Path      = " << item->input_path_name.at(0).c_str()  << endl;
                cout << "                     Output-Filename = " << item->output_file_name               << endl;
                cout << "                                                                               " << endl;
                cout << "          Result:                                                              " << endl;
                cout << "                     HammingWeight = " << item->result                           << endl;
                if(!(strcmp(item->result, "none") == 0)){
                cout << "                     Median        = " << item->median                           << endl;
                cout << "                     Average       = " << item->average                          << endl;}
                cout << "*******************************************************************************" << endl;

                if(error) ErrorMessages(error, 0);
                error = 0;

                cout << endl << "Make a choice by typing in a number (1-6): ";

                if (fgets(menuChoice, sizeof(menuChoice), stdin)) {
                    /* fgets succeeds, scan for newline character */
                    p = strchr(menuChoice, '\n');
                    if (p) {
                        *p = '\0';
                        switch(menuChoice[0])
                            {
                            case '1':
                                cout << endl << " Processing : Set Input-Path" << endl << endl;
                                DefinePathname(item, 1);
                                strcpy(item->result, "none");
                                error = 0;
                                item->zeros = 0;
                                item->ones = 0;
                                break;
                            case '2':
                                cout << endl << " Processing : Set Output-Filename" << endl << endl;
                                DefineFilename(item, 2);
                                strcpy(item->result, "none");
                                error = 0;
                                if((strcmp(item->input_path_name.at(0).c_str(), "none") == 0) || (strcmp(item->output_file_name, "none") == 0))
                                 break;
                            case '3':
                                cout << endl << " Processing : HammingWeight calculation" << endl << endl;
                                item->median = 0;
                                item->average = 0;
                                error = HammingWeight(item, item->HW_ENTP_mode);
                                if(!error) {
                                        strcpy(item->result, "done - result saved");
                                        strcpy(item->input_file_name, item->output_file_name);
                                        Median_AVG(item);
                                }
                                break;
                            case '4':
                                cout << endl << " Change 'offset_begin' and 'offset_end'" << endl << endl;
                                item->zeros = 0;
                                item->ones = 0;
                                error = 0;
                                strcpy(item->result, "none");
                                ClearScreen();
                                DefineOffSetLength(item);
                                break;
                            case '5':
                                cout << endl << " Switch mode " << endl << endl;
                                item->HW_ENTP_mode = 0;
                                strcpy(item->result, "none");
                                strcpy(item->input_file_name, "none");
                                change = 1;
                                break;
                            case '6':
                                cout << endl << " Back " << endl << endl;
                                error = 0;
                                exit = 1;
                                strcpy(item->input_file_name, "none");
                                break;
                            default:
                                error = 15;
                                break;
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
                if(exit || change) break;
            }
        }
        if(exit) break;
    }
}

void Entropy_Menu(struct Item *item)
/*
 * Console Menu for the (Shannon) Entropy */
{
    char menuChoice[3];
    char *p;
    unsigned int ch;
    unsigned int error = 0;
    unsigned int exit = 0;
    unsigned int change = 0;
    stringstream res;
    string result;

    while(true){
        change = 0;
        if(item->HW_ENTP_mode == 0){
            while(true){
                ClearScreen();
                cout << "*******************************************************************************" << endl;
                cout << "*                                                                             *" << endl;
                cout << "*                         Welcome to the PUF-Toolkit                          *" << endl;
                cout << "*                                                                             *" << endl;
                cout << "***************************** (Shannon) Entropy *******************************" << endl;
                cout << "*                                                                             *" << endl;
                cout << "*                        1 : Set File                                         *" << endl;
                cout << "*                        2 : Calculate (Shannon) Entropy                      *" << endl;
                cout << "*                        3 : Save result                                      *" << endl;
                cout << "*                        4 : Change 'offset_begin' & 'offset_end'                       *" << endl;
                cout << "*                        5 : Switch mode (file / directory)                   *" << endl;
                cout << "*                        6 : Back                                             *" << endl;
                cout << "*                                                                             *" << endl;
                cout << "*******************************************************************************" << endl;
                cout << "          Settings:                                                            " << endl;
                cout << "                     offset_begin, offset_end  = " << item->offset_begin << ", " << item->offset_end    << endl;
                cout << "                     Length   = " << item->input_length<< " byte (" << item->input_length*8 << " bit)" << endl;
                cout << "                     Mode     = single file                                    " << endl;
                cout << "                                                                               " << endl;
                cout << "                     Filename = " << item->input_file_name                       << endl;
                cout << "                                                                               " << endl;
                cout << "          Result:                                                              " << endl;
                cout << "                     (Shannon) Entropy = " << item->result                       << endl;
                cout << "*******************************************************************************" << endl;

                if(error) ErrorMessages(error, 0);
                error = 0;

                cout << endl << "Make a choice by typing in a number (1-6): ";

                if (fgets(menuChoice, sizeof(menuChoice), stdin)) {
                    /* fgets succeeds, scan for newline character */
                    p = strchr(menuChoice, '\n');
                    if (p) {
                        *p = '\0';
                        switch(menuChoice[0])
                            {
                            case '1':
                                cout << endl << " Processing : Set filename" << endl << endl;
                                DefineFilename(item, 1);
                                item->zeros = 0;
                                item->ones = 0;
                                item->entrp = 0;
                                error = Entropy(item, item->HW_ENTP_mode);
                                if(!error){
                                    res.str( std::string() );
                                    res.clear();
                                    res << setprecision(10) << item->entrp;
                                    result = res.str();
                                    strcpy(item->result, result.c_str());
                                }
                                else strcpy(item->result, "none");
                                break;
                            case '2':
                                cout << endl << " Processing : (Shannon) Entropy calculation" << endl << endl;
                                item->zeros = 0;
                                item->ones = 0;
                                item->entrp = 0;
                                strcpy(item->result, "none");
                                error = Entropy(item, item->HW_ENTP_mode);
                                if(!error){
                                    res.str( std::string() );
                                    res.clear();
                                    res << setprecision(10) << item->entrp;
                                    result = res.str();
                                    strcpy(item->result, result.c_str());
                                }
                                break;
                            case '3':
                                cout << endl << " Processing : Save result" << endl << endl;
                                if(strcmp(item->result, "none") == 0) error = 22;
                                else error = SaveFile(item,2);
                                break;
                            case '4':
                                cout << endl << " Change 'offset_begin' and 'offset_end'" << endl << endl;
                                item->zeros = 0;
                                item->ones = 0;
                                item->entrp = 0;
                                error = 0;
                                strcpy(item->result, "none");
                                ClearScreen();
                                DefineOffSetLength(item);
                                break;
                            case '5':
                                cout << endl << " Switch mode " << endl << endl;
                                item->HW_ENTP_mode = 1;
                                strcpy(item->result, "none");
                                strcpy(item->input_file_name, "none");
                                change = 1;
                                break;
                            case '6':
                                cout << endl << " Back " << endl << endl;
                                error = 0;
                                exit = 1;
                                break;
                            default:
                                error = 15;
                                break;
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
                if(exit || change) break;
            }
        }
        else if(item->HW_ENTP_mode == 1){
            while(true){
                ClearScreen();
                cout << "*******************************************************************************" << endl;
                cout << "*                                                                             *" << endl;
                cout << "*                         Welcome to the PUF-Toolkit                          *" << endl;
                cout << "*                                                                             *" << endl;
                cout << "***************************** (Shannon) Entropy *******************************" << endl;
                cout << "*                                                                             *" << endl;
                cout << "*                        1 : Set Input-Path                                   *" << endl;
                cout << "*                        2 : Set Output-Filename                              *" << endl;
                cout << "*                        3 : Calculate (Shannon) Entropy                      *" << endl;
                cout << "*                        4 : Change 'offset_begin' & 'offset_end'                       *" << endl;
                cout << "*                        5 : Switch mode (file / directory)                   *" << endl;
                cout << "*                        6 : Back                                             *" << endl;
                cout << "*                                                                             *" << endl;
                cout << "*******************************************************************************" << endl;
                cout << "          Settings:                                                            " << endl;
                cout << "                     offset_begin, offset_end  = " << item->offset_begin << ", " << item->offset_end    << endl;
                cout << "                     Length          = " << item->input_length<< " byte (" << item->input_length*8 << " bit)" << endl;
                cout << "                     Mode            = complete directory                      " << endl;
                cout << "                                                                               " << endl;
                cout << "                     Input-Path      = " << item->input_path_name.at(0).c_str()  << endl;
                cout << "                     Output-Filename = " << item->output_file_name               << endl;
                cout << "                                                                               " << endl;
                cout << "          Result:                                                              " << endl;
                cout << "                     (Shannon) Entropy = " << item->result                       << endl;
                if(!(strcmp(item->result, "none") == 0)){
                cout << "                     Median            = " << item->median                       << endl;
                cout << "                     Average           = " << item->average                      << endl;}
                cout << "*******************************************************************************" << endl;


                if(error) ErrorMessages(error, 0);
                error = 0;

                cout << endl << "Make a choice by typing in a number (1-6): ";

                if (fgets(menuChoice, sizeof(menuChoice), stdin)) {
                    /* fgets succeeds, scan for newline character */
                    p = strchr(menuChoice, '\n');
                    if (p) {
                        *p = '\0';
                        switch(menuChoice[0])
                            {
                            case '1':
                                cout << endl << " Processing : Set Input-Path" << endl << endl;
                                DefinePathname(item, 1);
                                strcpy(item->result, "none");
                                error = 0;
                                item->zeros = 0;
                                item->ones = 0;
                                item->entrp = 0;
                                break;
                            case '2':
                                cout << endl << " Processing : Set Output-Filename" << endl << endl;
                                DefineFilename(item, 2);
                                strcpy(item->result, "none");
                                error = 0;
                                if((strcmp(item->input_path_name.at(0).c_str(), "none") == 0) || (strcmp(item->output_file_name, "none") == 0))
                                 break;
                            case '3':
                                cout << endl << " Processing : (Shannon) Entropy calculation" << endl << endl;
                                item->zeros = 0;
                                item->ones = 0;
                                item->entrp = 0;
                                item->median = 0;
                                item->average = 0;
                                strcpy(item->result, "none");
                                error = Entropy(item, item->HW_ENTP_mode);
                                if(!error) {
                                        strcpy(item->result, "done - result saved");
                                        strcpy(item->input_file_name, item->output_file_name);
                                        Median_AVG(item);
                                }
                                break;
                            case '4':
                                cout << endl << " Change 'offset_begin' and 'offset_end'" << endl << endl;
                                item->zeros = 0;
                                item->ones = 0;
                                item->entrp = 0;
                                error = 0;
                                strcpy(item->result, "none");
                                ClearScreen();
                                DefineOffSetLength(item);
                                break;
                            case '5':
                                cout << endl << " Switch mode " << endl << endl;
                                item->HW_ENTP_mode = 0;
                                strcpy(item->result, "none");
                                strcpy(item->input_file_name, "none");
                                change = 1;
                                break;
                            case '6':
                                cout << endl << " Back " << endl << endl;
                                error = 0;
                                exit = 1;
                                strcpy(item->input_file_name, "none");
                                break;
                            default:
                                error = 15;
                                break;
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
                if(exit || change) break;
            }
        }
        if(exit) break;
    }
}

void IntraHD_Menu(struct Item *item)
/*
 * Console Menu for the Inter-Hamming Distance */
{
    char menuChoice[3];
    char *p;
    unsigned int ch;
    unsigned int error = 0;
    unsigned int exit = 0;


    while(true){
        ClearScreen();
        cout << "*******************************************************************************" << endl;
        cout << "*                                                                             *" << endl;
        cout << "*                         Welcome to the PUF-Toolkit                          *" << endl;
        cout << "*                                                                             *" << endl;
        cout << "*************************** Intra-Hamming Distance ****************************" << endl;
        cout << "*                                                                             *" << endl;
        cout << "*                        1 : Set Input-Path                                   *" << endl;
        cout << "*                        2 : Set Output-Filename                              *" << endl;
        cout << "*                        3 : Calculate Intra-Hamming Distance                 *" << endl;
        cout << "*                        4 : View Result                                      *" << endl;
        cout << "*                        5 : Change 'offset_begin' & 'offset_end'                       *" << endl;
        cout << "*                        6 : Switch Output-Style                              *" << endl;
        cout << "*                        7 : Back                                             *" << endl;
        cout << "*                                                                             *" << endl;
        cout << "*******************************************************************************" << endl;
        cout << "          Settings:                                                            " << endl;
        cout << "                     offset_begin, offset_end  = " << item->offset_begin << ", " << item->offset_end    << endl;
        cout << "                     Length          = " << item->input_length<< " byte (" << item->input_length*8 << " bit)" << endl;
        if(item->HD_mode == 0){
        cout << "                     Output-Style    = compact                                 " << endl;}
		if(item->HD_mode == 1){
        cout << "                     Output-Style    = minimal                                 " << endl;}
        cout << "                                                                               " << endl;
        cout << "                     Input-Path      = " << item->input_path_name.at(0).c_str()  << endl;
        cout << "                     Output-Filename = " << item->output_file_name               << endl;
        cout << "                                                                               " << endl;
        cout << "          Result:                                                              " << endl;
        cout << "                     Intra-HD        = " << item->result                         << endl;
        if((item->HD_mode == 1) && !(strcmp(item->result, "none") == 0)){
        cout << "                     Median          = " << item->median                         << endl;
        cout << "                     Average         = " << item->average                        << endl;}
        cout << "*******************************************************************************" << endl;

        if(error) ErrorMessages(error, 0);
        error = 0;

        cout << endl << "Make a choice by typing in a number (1-7): ";

        if (fgets(menuChoice, sizeof(menuChoice), stdin)) {
            /* fgets succeeds, scan for newline character */
            p = strchr(menuChoice, '\n');
            if (p) {
                *p = '\0';
                switch(menuChoice[0])
                    {
                    case '1':
                        cout << endl << " Processing : Set Input-Path" << endl << endl;
                        DefinePathname(item, 1);
                        strcpy(item->result, "none");
                        error = 0;
                        break;
                    case '2':
                        cout << endl << " Processing : Set Output-Filename" << endl << endl;
                        DefineFilename(item, 2);
                        strcpy(item->result, "none");
                        error = 0;
                        if((strcmp(item->input_path_name.at(0).c_str(), "none") == 0) || (strcmp(item->output_file_name, "none") == 0))
                         break;
                    case '3':
                        cout << endl << " Processing : Intra-Hamming Distance calculation" << endl << endl;
                        item->median = 0;
                        item->average = 0;
                        error = IntraHD(item, item->HD_mode);
                        if(!error){
                            strcpy(item->result, "done - result saved");
                            if(item->HD_mode == 1){
                                    strcpy(item->input_file_name, item->output_file_name);
                                    Median_AVG(item);
                            }
                        }
                        break;
                    case '4':
                        cout << endl << " Processing : View result" << endl << endl;
                        ClearScreen();
                        error = ViewFile(item, 1);
                        break;
                    case '5':
                        cout << endl << " Change 'offset_begin' and 'offset_end'" << endl << endl;
                        item->zeros = 0;
                        item->ones = 0;
                        item->entrp = 0;
                        strcpy(item->result, "none");
                        error = 0;
                        ClearScreen();
                        DefineOffSetLength(item);
                        break;
                    case '6':
                        cout << endl << " Switch Output-Style " << endl << endl;
                        if(item->HD_mode == 0){
                                item->median = 0;
                                item->average = 0;
                                item->HD_mode = 1;
                                strcpy(item->result, "none");
                                strcpy(item->input_file_name, "none");
                                strcpy(item->output_file_name, "none");
                        }
                        else if(item->HD_mode == 1){
                                item->median = 0;
                                item->average = 0;
                                item->HD_mode = 0;
                                strcpy(item->result, "none");
                                strcpy(item->input_file_name, "none");
                                strcpy(item->output_file_name, "none");
                        }
                        break;
                    case '7':
                        cout << endl << " Back " << endl << endl;
                        error = 0;
                        exit = 1;
                        strcpy(item->input_file_name, "none");
                        break;
                    default:
                        error = 15;
                        break;
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
		if(exit) break;
    }
}

void InterHD_Menu(struct Item *item)
/*
 * Console Menu for the Intra-Hamming Distance */
{
    char menuChoice[3];
    char *p;
    unsigned int ch;
    unsigned int error = 0;
    unsigned int exit = 0;
    unsigned int i = 0;


    while(true){
        ClearScreen();
								cout << "*******************************************************************************" << endl;
								cout << "*                                                                             *" << endl;
								cout << "*                         Welcome to the PUF-Toolkit                          *" << endl;
								cout << "*                                                                             *" << endl;
								cout << "*************************** Inter-Hamming Distance ****************************" << endl;
								cout << "*                                                                             *" << endl;
								cout << "*                        1 : Set Input-Paths                                  *" << endl;
								cout << "*                        2 : Set Output-Filename                              *" << endl;
								cout << "*                        3 : Calculate Inter-Hamming Distance                 *" << endl;
								cout << "*                        4 : View Result                                      *" << endl;
								cout << "*                        5 : Change 'offset_begin' & 'offset_end'                       *" << endl;
								cout << "*                        6 : Switch Output-Style                              *" << endl;
								cout << "*                        7 : Back                                             *" << endl;
								cout << "*                                                                             *" << endl;
								cout << "*******************************************************************************" << endl;
								cout << "          Settings:                                                            " << endl;
								cout << "                     offset_begin, offset_end  = " << item->offset_begin << ", " << item->offset_end    << endl;
								cout << "                     Length          = " << item->input_length << " byte (" << item->input_length*8 << " bit)" << endl;
		if(item->HD_mode == 0)  cout << "                     Output-Style    = compact                                 " << endl;
		if(item->HD_mode == 1)  cout << "                     Output-Style    = detailed                                " << endl;
		if(item->HD_mode == 2)  cout << "                     Output-Style    = minimal                                 " << endl;
                                cout << "                                                                               " << endl;
		for(i = 0; i < item->input_path_name.size(); i++){
            if(i == 0)  		cout << "                     Input-Paths     = " << item->input_path_name.at(i).c_str()  << endl;
            else        		cout << "                                       " << item->input_path_name.at(i).c_str()  << endl;}
								cout << "                     Output-Filename = " << item->output_file_name               << endl;
								cout << "                                                                               " << endl;
								cout << "          Result:                                                              " << endl;
								cout << "                     Inter-HD        = " << item->result                         << endl;
        if((item->HD_mode == 2) && !(strcmp(item->result, "none") == 0)){
                                cout << "                     Median          = " << item->median                         << endl;
                                cout << "                     Average         = " << item->average                        << endl;}
								cout << "*******************************************************************************" << endl;


        if(error) ErrorMessages(error, item->HD_error_pos);
        error = 0;

        cout << endl << "Make a choice by typing in a number (1-7): ";

        if (fgets(menuChoice, sizeof(menuChoice), stdin)) {
            /* fgets succeeds, scan for newline character */
            p = strchr(menuChoice, '\n');
            if (p) {
                *p = '\0';
                switch(menuChoice[0])
                    {
                    case '1':
                        cout << endl << " Processing : Set Input-Paths" << endl << endl;
                        DefinePathname(item, 2);
                        strcpy(item->result, "none");
                        error = 0;
                        break;
                    case '2':
                        cout << endl << " Processing : Set Output-Filename" << endl << endl;
                        DefineFilename(item, 2);
                        strcpy(item->result, "none");
                        error = 0;
                        if((strcmp(item->input_path_name.at(0).c_str(), "none") == 0) || (strcmp(item->output_file_name, "none") == 0))
                         break;
                    case '3':
                        cout << endl << " Processing : Inter-Hamming Distance calculation" << endl << endl;
                        item->median = 0;
                        item->average = 0;
                        error = InterHD(item, item->HD_mode);
                        if(!error) {
                                strcpy(item->result, "done - result saved");
                                if(item->HD_mode == 2){
                                    strcpy(item->input_file_name, item->output_file_name);
                                    Median_AVG(item);
                            }
                        }
                        break;
                    case '4':
                        cout << endl << " Processing : View result" << endl << endl;
                        ClearScreen();
                        error = ViewFile(item,1);
                        break;
                    case '5':
                        cout << endl << " Change 'offset_begin' and 'offset_end'" << endl << endl;
                        item->zeros = 0;
                        item->ones = 0;
                        item->entrp = 0;
                        strcpy(item->result, "none");
                        error = 0;
                        ClearScreen();
                        DefineOffSetLength(item);
                        break;
                    case '6':
                        cout << endl << " Switch Output-Style " << endl << endl;
                        if(item->HD_mode == 0) {
                                item->HD_mode = 1;
                                item->median = 0;
                                item->average = 0;
                                strcpy(item->result, "none");
                                strcpy(item->input_file_name, "none");
                                strcpy(item->output_file_name, "none");
                        }
                        else if(item->HD_mode == 1){
                                item->HD_mode = 2;
                                item->median = 0;
                                item->average = 0;
                                strcpy(item->result, "none");
                                strcpy(item->input_file_name, "none");
                                strcpy(item->output_file_name, "none");
                        }
                        else if(item->HD_mode == 2){
                                item->HD_mode = 0;
                                item->median = 0;
                                item->average = 0;
                                strcpy(item->result, "none");
                                strcpy(item->input_file_name, "none");
                                strcpy(item->output_file_name, "none");
                        }
                        break;
                    case '7':
                        cout << endl << " Back " << endl << endl;
                        item->input_path_name.erase(item->input_path_name.begin() + 1, item->input_path_name.end());
                        item->input_path_name.at(0) = "none";
                        strcpy(item->input_file_name, "none");
                        error = 0;
                        exit = 1;
                        break;
                    default:
                        error = 15;
                        break;
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
		if(exit) break;
    }
}

void MinEntropy_Menu(struct Item *item)
/*
 * Console Menu for the (Shannon) Entropy */
{
    char menuChoice[3];
    char *p;
    unsigned int ch;
    unsigned int error = 0;
    unsigned int done = 0;
    unsigned int exit = 0;
    stringstream res;
    string result;


    while(true){
        ClearScreen();
				cout << "*******************************************************************************" << endl;
				cout << "*                                                                             *" << endl;
				cout << "*                         Welcome to the PUF-Toolkit                          *" << endl;
				cout << "*                                                                             *" << endl;
				cout << "***************************** Min-Entropy (CTW) *******************************" << endl;
				cout << "*                                                                             *" << endl;
				cout << "*                        1 : Set Path                                         *" << endl;
				cout << "*                        2 : Calculate Min-Entropy (CTW)                      *" << endl;
				cout << "*                        3 : Save result                                      *" << endl;
				cout << "*                        4 : Change 'offset_begin' & 'offset_end'                       *" << endl;
				cout << "*                        5 : Back                                             *" << endl;
				cout << "*                                                                             *" << endl;
				cout << "*******************************************************************************" << endl;
				cout << "          Settings:                                                            " << endl;
				cout << "                     offset_begin, offset_end  = " << item->offset_begin << ", " << item->offset_end    << endl;
				cout << "                     Length   = " << item->input_length << " byte (" << item->input_length*8 << " bit)" << endl;
				cout << "                     Pathname = " << item->input_path_name.at(0).c_str()         << endl;
				cout << "                                                                               " << endl;
				cout << "          Result:                                                              " << endl;
				cout << "                     Min-Entropy (CTW)      = " << item->result                  << endl;
		if(strcmp(item->result, "none") == 0){
				cout << "                     Fractional Min-Entropy = none                             " << endl;}
		else    cout << "                     Fractional Min-Entropy = " << ((item->entrp) / double(item->input_length*8)) << " (" << (((item->entrp) / double(item->input_length*8)) * 100) << "%)" << endl;
				cout << "*******************************************************************************" << endl;

        if(error) ErrorMessages(error, 0);
        error = 0;

        cout << endl << "Make a choice by typing in a number (1-5): ";

        if (fgets(menuChoice, sizeof(menuChoice), stdin)) {
            /* fgets succeeds, scan for newline character */
            p = strchr(menuChoice, '\n');
            if (p) {
                *p = '\0';
                switch(menuChoice[0])
                    {
                    case '1':
                        cout << endl << " Processing : Set pathname" << endl << endl;
                        DefinePathname(item, 1);
                        item->zeros = 0;
                        item->ones = 0;
                        item->entrp = 0;
                        done = 0;
                        error = MinEntropy(item);
                        if(error == 0) {
                                done = 1;
                                res.str( std::string() );
                                res.clear();
                                res << setprecision(10) << item->entrp;
                                result = res.str();
                                strcpy(item->result, result.c_str());
                        }
                        break;
                    case '2':
                        cout << endl << " Processing : Min-Entropy calculation" << endl << endl;
                        item->zeros = 0;
                        item->ones = 0;
                        item->entrp = 0;
                        done = 0;
                        error = MinEntropy(item);
                        if(error == 0) {
                                done = 1;
                                res.str( std::string() );
                                res.clear();
                                res << setprecision(10) << item->entrp;
                                result = res.str();
                                strcpy(item->result, result.c_str());
                        }
                        break;
                    case '3':
                        cout << endl << " Processing : Save result" << endl << endl;
                        if(done == 0) error = 22;
                        else error = SaveFile(item,3);
                        break;
                    case '4':
                        cout << endl << " Change 'offset_begin' and 'offset_end'" << endl << endl;
                        item->zeros = 0;
                        item->ones = 0;
                        item->entrp = 0;
                        error = 0;
                        done = 0;
                        strcpy(item->result, "none");
                        ClearScreen();
                        DefineOffSetLength(item);
                        break;
                    case '5':
                        cout << endl << " Back " << endl << endl;
                        error = 0;
                        exit = 1;
                        break;
                    default:
                        error = 15;
                        break;
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
		if(exit) break;
    }
}

void Median_AVG_Menu(struct Item *item)
/*
 * Console Menu for the Median and Average */
{
    char menuChoice[3];
    char *p;
    unsigned int ch;
    unsigned int error = 0;
    unsigned int exit = 0;

    while(true){
        ClearScreen();
				cout << "*******************************************************************************" << endl;
				cout << "*                                                                             *" << endl;
				cout << "*                         Welcome to the PUF-Toolkit                          *" << endl;
				cout << "*                                                                             *" << endl;
				cout << "***************************** Median and Average ******************************" << endl;
				cout << "*                                                                             *" << endl;
				cout << "*                        1 : Set File                                         *" << endl;
				cout << "*                        2 : Calculate Median and Average                     *" << endl;
				cout << "*                        3 : Back                                             *" << endl;
				cout << "*                                                                             *" << endl;
				cout << "*******************************************************************************" << endl;
				cout << "          Settings:                                                            " << endl;
				cout << "                     Filename = " << item->input_file_name                       << endl;
				cout << "                                                                               " << endl;
				cout << "          Result:                                                              " << endl;
				cout << "                     Calculation progress = " << item->result                    << endl;
		if(!(strcmp(item->result, "none") == 0)){
                cout << "                     Median               = " << item->median                    << endl;
                cout << "                     Average              = " << item->average                   << endl;}
				cout << "*******************************************************************************" << endl;

        if(error) ErrorMessages(error, 0);
        error = 0;

        cout << endl << "Make a choice by typing in a number (1-3): ";

        if (fgets(menuChoice, sizeof(menuChoice), stdin)) {
            /* fgets succeeds, scan for newline character */
            p = strchr(menuChoice, '\n');
            if (p) {
                *p = '\0';
                switch(menuChoice[0])
                    {
                    case '1':
                        cout << endl << " Processing : Set filename" << endl << endl;
                        DefineFilename(item, 3);
                        item->zeros = 0;
                        item->ones = 0;
                        item->entrp = 0;
                        if((strcmp(item->input_file_name, "none") == 0))
                         break;
                    case '2':
                        cout << endl << " Processing : Median and Average calculation" << endl << endl;
                        error = Median_AVG(item);
                        if(error == 0) {
                                strcpy(item->result, "done");
                        }
                        break;
                    case '3':
                        cout << endl << " Back " << endl << endl;
                        error = 0;
                        exit = 1;
                        item->median = 0;
                        item->average = 0;
                        strcpy(item->result, "none");
                        strcpy(item->input_file_name, "none");
                        break;
                    default:
                        error = 15;
                        break;
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
		if(exit) break;
    }
}

int Main_Menu()
/*
 * Main Menu for the PUF-Toolkit */
{

    char menuChoice[3];
    char *p;
    unsigned int ch;
    unsigned int error = 0;
    unsigned int exit = 0;

    struct Item it1;

	// Set start values for the item struct
    it1.offset_begin = 0;
    it1.offset_end = 0;
    it1.input_length = 0;
    strcpy(it1.input_file_name, "none");
    strcpy(it1.result, "none");
    it1.input_path_name.push_back("none");
    strcpy(it1.output_file_name, "none");
    it1.zeros = 0;
    it1.ones = 0;
    it1.entrp = 0;
    it1.median = 0;
    it1.average = 0;
    it1.HD_error_pos = 0;
    it1.HD_mode = 0;
    it1.HW_ENTP_mode = 0;

    cout << "*******************************************************************************" << endl;
    cout << "*                                                                             *" << endl;
    cout << "*                         Welcome to the PUF-Toolkit                          *" << endl;
    cout << "*                                                                             *" << endl;
	cout << "******************************* Global Settings *******************************" << endl << endl;
    cout << "                 The 'offset_begin' and 'offset_end' have to be defined.                 " << endl;
    cout << "                 These settings will be used for all processing.               " << endl << endl;

    // Get the 'offset_begin' and 'offset_end' to be used
	DefineOffSetLength(&it1);

    while(true){
        ClearScreen();
        cout << "*******************************************************************************" << endl;
        cout << "*                                                                             *" << endl;
        cout << "*                         Welcome to the PUF-Toolkit                          *" << endl;
        cout << "*                                                                             *" << endl;
        cout << "********************************** Main Menu **********************************" << endl;
        cout << "*                                                                             *" << endl;
        cout << "*                        1 : Hamming-Weight                                   *" << endl;
        cout << "*                        2 : (Shannon) Entropy                                *" << endl;
        cout << "*                        3 : Intra-Hamming Distance                           *" << endl;
        cout << "*                        4 : Inter-Hamming Distance                           *" << endl;
        cout << "*                        5 : Min-Entropy (CTW)                                *" << endl;
        cout << "*                        6 : Median and Average                               *" << endl;
        cout << "*                        7 : Change 'offset_begin' & 'offset_end'                       *" << endl;
        cout << "*                        8 : Exit Program                                     *" << endl;
        cout << "*                                                                             *" << endl;
        cout << "*******************************************************************************" << endl;
        cout << "          Settings:                                                            " << endl;
        cout << "                     offset_begin, offset_end   = " << it1.offset_begin << ", " << it1.offset_end        << endl;
        cout << "                     Length   = " << it1.input_length << " byte (" << it1.input_length*8 << " bit)" << endl;
        cout << "*******************************************************************************" << endl;

        if(error) ErrorMessages(error, 0);
        error = 0;

        while(true){
            cout << endl << "Make a choice by typing in a number (1-8): ";
            if (fgets(menuChoice, sizeof(menuChoice), stdin)) {
                /* fgets succeeds, scan for newline character */
                p = strchr(menuChoice, '\n');
                if (p) {
                    *p = '\0';
                    switch(menuChoice[0])
                        {
                        case '1':
                            cout << endl << " Processing : Hamming-Weight" << endl << endl;
                            error = 0;
                            it1.zeros = 0;
                            it1.ones = 0;
                            it1.entrp = 0;
                            it1.HW_ENTP_mode = 0;
                            strcpy(it1.result, "none");
                            strcpy(it1.output_file_name, "none");
                            it1.input_path_name.at(0) = "none";
                            HammingWeight_Menu(&it1);
                            break;
                        case '2':
                            cout << endl << " Processing : (Shannon) Entropy" << endl << endl;
                            error = 0;
                            it1.zeros = 0;
                            it1.ones = 0;
                            it1.entrp = 0;
                            it1.HW_ENTP_mode = 0;
                            strcpy(it1.result, "none");
                            strcpy(it1.output_file_name, "none");
                            it1.input_path_name.at(0) = "none";
                            Entropy_Menu(&it1);
                            break;
                        case '3':
                            cout << endl << " Processing : Intra-Hamming Distance" << endl << endl;
                            error = 0;
                            strcpy(it1.result, "none");
                            strcpy(it1.output_file_name, "none");
                            it1.input_path_name.at(0) = "none";
                            it1.HD_mode = 0;
                            IntraHD_Menu(&it1);
                            break;
                        case '4':
                            cout << endl << " Processing : Inter-Hamming Distance" << endl << endl;
                            error = 0;
                            strcpy(it1.result, "none");
                            strcpy(it1.output_file_name, "none");
                            it1.input_path_name.at(0) = "none";
                            it1.HD_mode = 0;
                            InterHD_Menu(&it1);
                            break;
                        case '5':
                            cout << endl << " Processing : Min-Entropy (CTW)" << endl << endl;
                            it1.zeros = 0;
                            it1.ones = 0;
                            it1.entrp = 0;
                            strcpy(it1.result, "none");
                            MinEntropy_Menu(&it1);
                            break;
                        case '6':
                            cout << endl << " Processing : Median and Average" << endl << endl;
                            it1.median = 0;
                            it1.average = 0;
                            strcpy(it1.result, "none");
                            it1.input_path_name.at(0) = "none";
                            Median_AVG_Menu(&it1);
                            break;
                        case '7':
                            cout << endl << " Change 'offset_begin' and 'offset_end'" << endl << endl;
                            error = 0;
                            ClearScreen();
                            DefineOffSetLength(&it1);
                            break;
                        case '8':
                            ClearScreen();
                            cout << endl << " Exiting Program -  bye bye" << endl << endl;
                            error = 0;
                            exit = 1;
                            break;
                        default:
                            error = 15;
                            break;
                        }
                    break;
                }
                else {
                    /* newline not found, flush stdin to end of line */
                    while (((ch = getchar()) != '\n')
                             && !feof(stdin)
                             && !ferror(stdin)
                            );
                        error = 2;
                        break;
                }
            }
            else {
                /* fgets failed, handle error */
                cin.clear();
                error = 3;
                break;
            }
        }
        if(exit) return 0;
        ClearScreen();
    }
return 0;
}
