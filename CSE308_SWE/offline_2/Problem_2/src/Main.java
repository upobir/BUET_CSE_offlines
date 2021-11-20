import java.util.Scanner;

import Editor.Editor;

public class Main{
    public static void main(String[] args) {
        System.out.println("Welcome to code editor");
        Scanner scanner = new Scanner(System.in);

        while(true){
            System.out.print("Enter filename: ");
            
            String filename = scanner.next();
            if(filename.equals("exit"))
                break;

            Editor editor = Editor.getInstance();
            editor.openFile(filename);

            System.out.println("Editor is using parser: " + editor.getParserName());
            System.out.println("Editor is using font: " + editor.getFont());
            System.out.println();
        }
        
        scanner.close();
    }
}