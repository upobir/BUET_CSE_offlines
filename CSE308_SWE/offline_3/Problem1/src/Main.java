import java.util.Scanner;

import FileAdder.*;

public class Main{
    public static void main(String[] args) {
        FileIntAdderAdapter fileAdder = new FileIntAdderAdapter();
        Scanner scanner = new Scanner(System.in);
        
        while(true){
            System.out.println("Enter name of file with characters: ");
            String filename = scanner.next();

            if(filename.equals("exit")) 
                break;

            int ans = fileAdder.calculateSum(filename);
            System.out.println("sum of ascii values: "+ ans);
        }

        scanner.close();
    }
}