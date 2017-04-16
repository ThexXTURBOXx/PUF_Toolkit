#ifdef JNI
#include "PUF_jni.h"
#include "PUF_Toolkit.h"
#include "toolkit.h"


Item it1 = Item();

JNIEXPORT void JNICALL Java_toolkit_hammingwt(JNIEnv *env, jobject obj, 
        jstring name, jstring op_name, jboolean mode)
{
    unsigned int error = 0;
    const char *fname = env->GetStringUTFChars(name, 0);
    const char *op_fname = env->GetStringUTFChars(op_name, 0);
    it1.input_path_name.push_back("none");
    
    if(mode)
        it1.input_path_name.at(0) = fname;
    else if(!mode)
        strcpy(it1.input_file_name, fname);

    strcpy(it1.output_file_name, op_fname);
    error = HammingWeight(&it1, mode);
    if(!error) {
            strcpy(it1.result, "done - result saved");
            strcpy(it1.input_file_name, it1.output_file_name);
            Median_AVG(&it1);
    }
    if (error) ErrorMessages(error, 0);
    //cleanup 
    env->ReleaseStringUTFChars(name, fname);
    env->ReleaseStringUTFChars(op_name, op_fname);

}
#endif
