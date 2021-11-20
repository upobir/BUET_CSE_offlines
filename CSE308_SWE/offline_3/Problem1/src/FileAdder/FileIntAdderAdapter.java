package FileAdder;

import java.io.*;
import java.util.Scanner;

public class FileIntAdderAdapter{
    private FileIntAdder fileIntAdder;

    public FileIntAdderAdapter() {
        fileIntAdder = new FileIntAdder();
    }

    public int calculateSum(String filename) {
        String tempFileName = createTemporaryFile(filename);

        int result = fileIntAdder.calculateSum(tempFileName);

        deleteFile(tempFileName);
        
        return result;
    }

    

    private String createTemporaryFile(String filename) {
        String tempFilename = "temp_"+filename;

        try{
            File inputfile = new File(filename);
            Scanner scanner = new Scanner(inputfile);


            PrintWriter tempFileWriter = new PrintWriter(tempFilename);
            

            while(scanner.hasNext()){
                char c = scanner.next().charAt(0);

                tempFileWriter.print((int) c);
                tempFileWriter.print(' ');
            }

            scanner.close();
            tempFileWriter.close();


        } catch (IOException e){
            System.err.println("IO error occured");
            e.printStackTrace();
        }

        return tempFilename;
    }

    private void deleteFile(String tempFileName) {
        File file = new File(tempFileName);
        file.delete();
    }

}
