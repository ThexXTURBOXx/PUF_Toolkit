import java.util.Scanner;
import java.io.File;

public class toolkit {
    public static boolean mode = false;
    public native void hammingwt(String name, boolean mode);
    
    static {
        System.loadLibrary("toolkit");
    }

    public static void interface_C(String name) {
        toolkit tk = new toolkit();
        tk.hammingwt(name, mode);
    }

    public static void main(String argv[]) {
        Scanner reader = new Scanner(System.in);
        String filename = "none";
        File f = null;

        System.out.println("Enter file/folder name");
        filename = reader.nextLine();
        //check if file or folder name and set mode to pass to native C api
        f = new File(filename);
        mode = f.isDirectory();
        System.out.println(mode);
        interface_C(filename);
    }
}

    
