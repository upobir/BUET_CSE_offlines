import java.util.Scanner;

import TeaGAS.TeaGAS;
import TeaGAS.TeaGASFactory;

public class Main {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);

        System.out.println("Welcome to TeaGAS system administration");

        System.out.println("Choose a package (Silver/Gold/Platinum/Diamond):");
        String packageName = scanner.next();

        System.out.println("Choose an internet connection method (WiFi/GSM/Ethernet):");
        String internetConnection = scanner.next();

        System.out.println("Choose a web server framework (Django/Spring/Laravel):");
        String framework = scanner.next();

        TeaGASFactory factory = new TeaGASFactory();
        TeaGAS teaGAS = factory.getTeaGAS(packageName, internetConnection, framework);

        System.out.println();
        if(teaGAS == null){
            System.out.println("Invalid specifications");
        }
        else{
            teaGAS.describe(System.out);
        }

        scanner.close();
    }
}
