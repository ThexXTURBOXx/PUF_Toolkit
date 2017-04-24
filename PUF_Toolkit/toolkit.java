import java.util.Scanner;
import java.io.File;

public class toolkit {
    public static boolean mode = false;
    public static String filename = "none";
    public static String op_fname = "none";
    public native void hammingwt(String name, String op_fname, boolean mode);
    public native void entropy(String name, String op_fname, boolean mode);
    public native void intra_hd(String name, String op_fname, boolean mode);
    public native void min_entropy(String name, String op_fname, boolean mode);
    public native void median_avg(String name, String op_fname);
    
    static {
        System.loadLibrary("toolkit");
    }

    public static void setparams() {
        Scanner reader = new Scanner(System.in);
        File f = null;
        System.out.println("Enter file/folder name");
        filename = reader.nextLine();
        System.out.println("Enter output file name");
        op_fname = reader.nextLine();
        //check if file or folder name and set mode to pass to native C api
        f = new File(filename);
        mode = f.isDirectory();
        System.out.println(mode);
    }

    public static void interface_C() {
        toolkit tk = new toolkit();
        tk.hammingwt(filename, op_fname, mode);
        //tk.entropy(filename, op_fname, mode);
        //call intra_hd only when mode = true (folder mode)
        if (mode) {
            tk.intra_hd(filename, op_fname, mode);
            tk.min_entropy(filename, op_fname, mode);
        }
        //tk.median_avg(filename, op_fname);
    }

    public static void main(String argv[]) {
        setparams();
        interface_C();
    }
}
