#ifdef JNI
#include "PUF_jni.h"
#include "toolkit.h"


Item item = Item();
JNIEXPORT void JNICALL Java_toolkit_hammingwt(JNIEnv *env, jobject obj, 
        jstring name, jstring op_name, jboolean mode)
{
    unsigned int error = 0;
    const char *fname = env->GetStringUTFChars(name, 0);
    const char *op_fname = env->GetStringUTFChars(op_name, 0);

    item.input_path_name.push_back("none");
    
    if(mode)
        item.input_path_name.at(0) = fname;
    else if(!mode)
        strcpy(item.input_file_name, fname);

    strcpy(item.output_file_name, op_fname);
    error = HammingWeight(&item, mode); //calling hamming wt function

    //saving file based on folder or file mode
    if(mode) {
        if(!error) {
                strcpy(item.result, "done - result saved");
                strcpy(item.input_file_name, item.output_file_name);
                Median_AVG(&item);
        }
    } else if (!mode) {
        error = SaveFile(&item, 1);
    }
    if (error) ErrorMessages(error, 0);
    //cleanup 
    env->ReleaseStringUTFChars(name, fname);
    env->ReleaseStringUTFChars(op_name, op_fname);

}

JNIEXPORT void JNICALL Java_toolkit_entropy
  (JNIEnv * env, jobject obj, jstring name, jstring op_name, jboolean mode)
{
    unsigned int error = 0;
    const char *fname = env->GetStringUTFChars(name, 0);
    const char *op_fname = env->GetStringUTFChars(op_name, 0);
    item.input_path_name.push_back("none");

    //setting file/folder names to item struct
    if(mode)
        item.input_path_name.at(0) = fname;
    else if(!mode)
        strcpy(item.input_file_name, fname);
    strcpy(item.output_file_name, op_fname);

    error = Entropy(&item, mode); // calling the entropy function
    //saving file based on folder or file mode
    if(mode) {
        if(!error) {
                strcpy(item.result, "done - result saved");
                strcpy(item.input_file_name, item.output_file_name);
                Median_AVG(&item);
        }
    } else if (!mode) {
        error = SaveFile(&item, 2);
    }

    if (error) ErrorMessages(error, 0);
    //cleanup
    env->ReleaseStringUTFChars(name, fname);
    env->ReleaseStringUTFChars(op_name, op_fname);
}

JNIEXPORT void JNICALL Java_toolkit_intra_1hd
  (JNIEnv * env, jobject obj, jstring name, jstring op_name, jboolean mode)
{
    unsigned int error = 0;
    const char *fname = env->GetStringUTFChars(name, 0);
    const char *op_fname = env->GetStringUTFChars(op_name, 0);
    item.input_path_name.push_back("none");
    item.input_path_name.at(0) = fname;
    strcpy(item.output_file_name, op_fname);

    error = IntraHD(&item, mode); // intra hd calculation
    if(!error) {
            strcpy(item.result, "done - result saved");
            if (mode)
                strcpy(item.input_file_name, item.output_file_name);
                Median_AVG(&item);
    }

    if (error) ErrorMessages(error, 0);
    //cleanup
    env->ReleaseStringUTFChars(name, fname);
    env->ReleaseStringUTFChars(op_name, op_fname);
}

JNIEXPORT void JNICALL Java_toolkit_min_1entropy
  (JNIEnv * env, jobject obj, jstring name, jstring op_name, jboolean mode)
{
    unsigned int error = 0;
    stringstream res;
    string result;
    const char *fname = env->GetStringUTFChars(name, 0);
    const char *op_fname = env->GetStringUTFChars(op_name, 0);
    item.input_path_name.push_back("none");
    item.input_path_name.at(0) = fname;
    strcpy(item.output_file_name, op_fname);

    error = MinEntropy(&item); // min entropy calculation
    if(error == 0) {
        res.str( std::string() );
        res.clear();
        res << setprecision(10) << item.entrp;
        result = res.str();
        strcpy(item.result, result.c_str());
    }

    error = SaveFile(&item, 3);

    if (error) ErrorMessages(error, 0);
    //cleanup
    env->ReleaseStringUTFChars(name, fname);
    env->ReleaseStringUTFChars(op_name, op_fname);
}

JNIEXPORT void JNICALL Java_toolkit_median_1avg
  (JNIEnv * env, jobject obj, jstring name, jstring op_name)
{
    unsigned int error = 0;
    const char *fname = env->GetStringUTFChars(name, 0);
    const char *op_fname = env->GetStringUTFChars(op_name, 0);
    strcpy(item.input_file_name, fname);
    strcpy(item.output_file_name, op_fname);

    error = Median_AVG(&item); // median average calculation

    if (error) ErrorMessages(error, 0);
    //cleanup
    env->ReleaseStringUTFChars(name, fname);
    env->ReleaseStringUTFChars(op_name, op_fname);
}

JNIEXPORT void JNICALL Java_toolkit_bch_1encoder
  (JNIEnv *env, jobject obj, jstring PUF, jstring key, jstring helper)
{
    unsigned error = 0;
    const char *PUF_file = env->GetStringUTFChars(PUF, 0);
    const char *key_file = env->GetStringUTFChars(key, 0);
    const char *helper_file = env->GetStringUTFChars(helper, 0);

    read_p();
    generate_gf();
    gen_poly();
    //set LR to 7
    item.LR = 7;
    // setting the input key , PUF and output helper data
    strcpy(item.input_Key_name, key_file);
    strcpy(item.input_PUF_name, PUF_file);
    strcpy(item.output_HD_name, helper_file);

    //BCH encoding
    error = ReadKeyFile(&item);
    if (!error) error = Calculation_encode(&item);
    else if (error) ErrorMessages(error, 0);
    //cleanup
    env->ReleaseStringUTFChars(PUF, PUF_file);
    env->ReleaseStringUTFChars(key, key_file);
    env->ReleaseStringUTFChars(helper, helper_file);

}

JNIEXPORT void JNICALL Java_toolkit_bch_1decoder
  (JNIEnv *env, jobject obj, jstring PUF, jstring helperdata, jstring key_r)
{
    unsigned error = 0;
    const char *PUF_file = env->GetStringUTFChars(PUF, 0);
    const char *key_file = env->GetStringUTFChars(key_r, 0);
    const char *helper_file = env->GetStringUTFChars(helperdata, 0);

    // setting the output key , PUF and input helper data
    strcpy(item.output_Key_name, key_file);
    strcpy(item.input_PUF_name, PUF_file);
    strcpy(item.input_HD_name, helper_file);

    //BCH decoding
    error = Calculation_decode(&item);
    if (error) ErrorMessages(error, 0);
    //cleanup
    env->ReleaseStringUTFChars(PUF, PUF_file);
    env->ReleaseStringUTFChars(key_r, key_file);
    env->ReleaseStringUTFChars(helperdata, helper_file);
}
JNIEXPORT void JNICALL Java_toolkit_inter_1hd
  (JNIEnv *env, jobject obj, jobjectArray folders, jstring op_file)
{
    unsigned error = 0;
    int i = 0;
    //item.input_path_name.push_back("none");
    const char *op_f = env->GetStringUTFChars(op_file, 0);
    strcpy(item.output_file_name, op_f);
    jsize len = env->GetArrayLength(folders);
    for (i = 0; i < len; i++)
    {
        jstring string = (jstring) env->GetObjectArrayElement(folders, i);
        const char *folder = env->GetStringUTFChars(string, 0);
        item.input_path_name.push_back(folder);
        env->ReleaseStringUTFChars(string, folder);
    }

    error = InterHD(&item, 1);
    if (error) ErrorMessages(error, item.HD_error_pos);

    //printf("input size :%ld\n", item.input_path_name.size());
    //for(std::vector<string>::const_iterator i = item.input_path_name.begin();
    //        i != item.input_path_name.end(); ++i)
    //    std::cout << ' ' << *i;
    //std::cout << '\n';

    //cleanup
    env->ReleaseStringUTFChars(op_file, op_f);
}
#endif
