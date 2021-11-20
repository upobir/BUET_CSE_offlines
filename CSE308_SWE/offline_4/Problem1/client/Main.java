package client;

import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        if(args.length == 0){
            System.err.println("No name supplied");
            return;
        }
        Connection connection = new Connection(args[0]);
        connection.start();

        Scanner scanner = new Scanner(System.in);

        mainloop: while(true){
            String command = scanner.next();

            switch(command){
                case "S":{
                    String stockName = scanner.next();
                    connection.println(command + " " + stockName);
                } break;

                case "U":{
                    String stockName = scanner.next();
                    connection.println(command + " " + stockName);
                } break;

                case "E":{
                    break mainloop;
                }

                default:{
                    System.out.println("Invalid command");
                } break;
            }
        }

        scanner.close();

        connection.stopListening();
    }
}
