import java.util.Scanner;
import java.io.File;

public class toolkit {
    public static boolean mode = false;
    public static String filename = "none";
    public static String key_file = "private_key.txt";
    public static String key_r = "private_r.txt";
    public static String PUF = "PUF";
    public static String helperdata = "helper";
    public static String op_fname = "none";
    public static int n = 0;

    public native void hammingwt(String name, String op_fname, boolean mode);
    public native void entropy(String name, String op_fname, boolean mode);
    public native void intra_hd(String name, String op_fname, boolean mode);
    public native void inter_hd(String name[], String op_fname);
    public native void min_entropy(String name, String op_fname, boolean mode);
    public native void median_avg(String name, String op_fname);
    public native void bch_encoder(String PUF, String key_file, String helperdata);
    public native void bch_decoder(String PUF, String helper_data, String key_r);
    
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
        Scanner reader = new Scanner(System.in);
        toolkit tk = new toolkit();
        File f = null;
        //tk.hammingwt(filename, op_fname, mode);
        //tk.entropy(filename, op_fname, mode);
        //call intra_hd only when mode = true (folder mode)
        //if (mode) {
        //    tk.intra_hd(filename, op_fname, mode);
        //    tk.min_entropy(filename, op_fname, mode);
        //}
        //tk.median_avg(filename, op_fname);
        //inter hd user input
        System.out.println("enter number of folders");
        n = Integer.parseInt(reader.nextLine());
        String name[] = new String[n];

        for(int i = 0; i < n; i++)
        {
            System.out.format("enter %d folder name%n", i+1);
            name[i] = reader.nextLine();
            f = new File(name[i]);
            if (!f.isDirectory()) {
                System.out.println("error! incorrect folder name");
                return;
            }
        }
        tk.inter_hd(name, "inter_hd");
        //tk.bch_encoder(PUF, key_file, helperdata);
        //tk.bch_decoder(PUF, helperdata, key_r);
    }

    public static void main(String argv[]) {
        //setparams();
        interface_C();
    }
}
