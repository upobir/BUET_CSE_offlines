import java.io.PrintStream;
import java.util.Scanner;
import Bank.*;

public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);

        /// initialization
        int initialFunds = 1000000;
        String message = new String();
        Bank bank = new Bank(initialFunds);
        message += bank.getLog(); 

        /// employees
        bank.createEmployee("MD", "MANAGING DIRECTOR");
        message += bank.getLog();

        for(int i = 1; i<= 2; i++){
            bank.createEmployee("O"+i, "OFFICER");
            message += bank.getLog();
        }
        for(int i = 1; i<=5; i++){
            bank.createEmployee("C"+i, "CASHIER");
            message += bank.getLog();
        }
        
        System.out.println(message);

        /// I/O
        outer:
        while(true){
            System.out.println();
            System.out.print(">> ");

            switch(sc.next().toUpperCase()){
                case "END":{
                    break outer;
                }

                case "INC":{
                    bank.increment();
                    message = bank.getLog();
                    break;
                }

                case "CREATE":{
                    String name = sc.next();
                    String type = sc.next().toUpperCase();
                    if(type.equals("FIXED")){
                        type = type + " " + sc.next().toUpperCase();
                    }
                    double deposit = sc.nextDouble();

                    User user = bank.createAccount(name, type, deposit);
                    message = bank.getLog();

                    if(user != null){
                        System.out.println(message);
                        readCommands(user, sc, System.out);
                        continue outer;
                    }
                    break;
                }

                case "OPEN":{
                    String name = sc.next();
                    User user = bank.login(name);
                    message = bank.getLog();

                    if(user != null){
                        System.out.println(message);
                        readCommands(user, sc, System.out);
                        continue outer;
                    }
                    break;
                }
                default:{
                    message = "Invalid Command";
                    break;
                }
            }
            System.out.println(message);
        }

        sc.close();
    }

    private static void readCommands(User user, Scanner sc, PrintStream out){
        String message = new String();
        outer:
        while(true){
            out.println();
            out.print(">> ");

            switch(sc.next().toUpperCase()){
                case "CLOSE":{
                    message = "Logging out "+ user.getName();
                    out.println(message);
                    break outer;
                }

                case "LOOKUP":{
                    user.lookup(sc.next());
                    message = user.getLog();
                    break;
                }

                case "APPROVE":{
                    sc.next();
                    user.approveLoan();
                    message = user.getLog();
                    break;
                }

                case "CHANGE":{
                    String type = sc.next().toUpperCase();
                    if(type.equals("FIXED")){
                        type += " " + sc.next().toUpperCase();
                    }
                    double rate = sc.nextDouble();
                    user.changeInterestRate(type, rate);
                    message = user.getLog();
                    break;
                }

                case "SEE":{
                    user.getFund();
                    message = user.getLog();
                    break;
                }

                case "QUERY":{
                    user.getBalance();
                    message = user.getLog();
                    break;
                }

                case "DEPOSIT":{
                    double amount = sc.nextDouble();
                    user.deposit(amount);
                    message = user.getLog();
                    break;
                }

                case "WITHDRAW":{
                    double amount = sc.nextDouble();
                    user.withdraw(amount);
                    message = user.getLog();
                    break;
                }

                case "REQUEST":{
                    double amount = sc.nextDouble();
                    user.requestLoan(amount);
                    message = user.getLog();
                    break;
                }

                default:{
                    message = "Invalid Command";
                    break;
                }
            }
            out.println(message);
        }
    }

}