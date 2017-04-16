#ifdef JNI
#include "PUF_jni.h"
#include "PUF_Toolkit.h"
#include "toolkit.h"


Item it1 = Item();

JNIEXPORT void JNICALL Java_toolkit_hammingwt(JNIEnv *env, jobject obj, 
        jstring name, jboolean mode)
{
    unsigned int error = 0;
    const char *fname = env->GetStringUTFChars(name, 0);
    it1.input_path_name.push_back("none");
    
    if(mode)
        it1.input_path_name.at(0) = fname;
    else if(!mode)
        strcpy(it1.input_file_name, fname);

    printf("%s\n", it1.input_path_name.at(0).c_str());

    strcpy(it1.output_file_name, "folder_HD.txt");
    error = HammingWeight(&it1, mode);
    if(!error) {
            strcpy(it1.result, "done - result saved");
            strcpy(it1.input_file_name, it1.output_file_name);
            Median_AVG(&it1);
    }
    if (error) ErrorMessages(error, 0);
    //cleanup 
    env->ReleaseStringUTFChars(name, fname);

}
#endif
