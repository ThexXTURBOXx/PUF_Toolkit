package jni;

import soloader.NativeUtils;
import java.util.Scanner;
import java.io.File;
import java.io.IOException;

public class toolkit {
    public static boolean mode = false;
    public static String filename = "none";
    public static String key_file = "private_key.txt";
    public static String key_r = "private_r.txt";
    public static String PUF = "PUF";
    public static String helperdata = "helper";
    public static String op_fname = "none";
    public static int n = 0;
    public static String name[];

    public static native void hammingwt(String name, String op_fname, boolean mode);
    public static native void entropy(String name, String op_fname, boolean mode);
    public static native void intra_hd(String name, String op_fname, boolean mode);
    public static native void inter_hd(String name[], String op_fname);
    public static native void min_entropy(String name, String op_fname, boolean mode);
    public static native void median_avg(String name, String op_fname);
    public static native void bch_encoder(String PUF, String key_file, String helperdata);
    public static native void bch_decoder(String PUF, String helper_data, String key_r);
    
    static {
        try {
            NativeUtils.loadLibraryFromJar("/libtoolkit.so");
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    //public static void setparams() {
    //    Scanner reader = new Scanner(System.in);
    //    File f = null;
    //    System.out.println("Enter file/folder name");
    //    filename = reader.nextLine();
    //    System.out.println("Enter output file name");
    //    op_fname = reader.nextLine();
    //    //check if file or folder name and set mode to pass to native C api
    //    f = new File(filename);
    //    mode = f.isDirectory();
    //    System.out.println(mode);
    //}

    //public static void interface_C() {
    //    Scanner reader = new Scanner(System.in);
    //    toolkit tk = new toolkit();
    //    File f = null;
    //    tk.hammingwt(filename, op_fname, mode);
    //    //tk.entropy(filename, op_fname, mode);
    //    //call intra_hd only when mode = true (folder mode)
    //    //if (mode) {
    //    //    tk.intra_hd(filename, op_fname, mode);
    //    //    tk.min_entropy(filename, op_fname, mode);
    //    //}
    //    //tk.median_avg(filename, op_fname);
    //    //inter hd user input
    //    //System.out.println("enter number of folders");
    //    //n = Integer.parseInt(reader.nextLine());
    //    //String name[] = new String[]{"/home/prankur/code_seb/PUF_Tookit/data/input", 
    //    //    "/home/prankur/code_seb/PUF_Toolkit/data/ouput", "/home/prankur/code_seb/PUF_Toolkit/data/test"};

    //    //for(int i = 0; i < n; i++)
    //    //{
    //    //    System.out.format("enter %d folder name%n", i+1);
    //    //    name[i] = reader.nextLine();
    //    //    //f = new File(name[i]);
    //    //    //if (!f.isDirectory()) {
    //    //    //    System.out.println("error! incorrect folder name");
    //    //    //    return;
    //    //    //}
    //    //}
    //    //tk.inter_hd(name, "inter_hd");
    //    //tk.bch_encoder(PUF, key_file, helperdata);
    //    //tk.bch_decoder(PUF, helperdata, key_r);
    //}

    public static void main(String [] args){
        //setparams();
        //interface_C();
        String [] name = {"/home/prankur/code_seb/PUF_Toolkit/data/input", 
            "/home/prankur/code_seb/PUF_Toolkit/data/output"};
        inter_hd(name, "inter_jni");
    }
}
