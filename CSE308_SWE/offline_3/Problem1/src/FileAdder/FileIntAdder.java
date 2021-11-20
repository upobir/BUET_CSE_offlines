package FileAdder;

import java.io.*;
import java.util.Scanner;

public class FileIntAdder{

    public FileIntAdder() {}

    public int calculateSum(String filename){
        int result = 0;

        try{
            File file = new File(filename);
            Scanner scanner = new Scanner(file);

            while(scanner.hasNextInt()){
                result += scanner.nextInt();
            }

            scanner.close();

        } catch(FileNotFoundException e){
            System.err.println("File was not found");
            e.printStackTrace();
        }
        
        return result;
    }
}
