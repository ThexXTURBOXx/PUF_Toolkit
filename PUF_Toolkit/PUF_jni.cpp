#ifdef JNI
#include "PUF_jni.h"
#include "PUF_Toolkit.h"
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
#endif
