package Bank;

public abstract class User {
    protected String name;
    protected StringBuffer log;
    protected Bank bank;

    public User(String name, Bank bank){
        this.name = name;
        this.bank = bank;
        this.log = new StringBuffer();
    }

    public String getName(){
        return name;
    }

    public String getLog(){
        return new String(log);
    }

    public double lookup(String name){
        log.setLength(0);
        log.append("Invalid Command");
        return -1;
    }

    public boolean approveLoan(){
        log.setLength(0);
        log.append("Invalid Command");
        return false;
    }

    public double getFund(){
        log.setLength(0);
        log.append("Invalid Command");
        return -1;
    }

    public boolean changeInterestRate(String type, double rate){
        log.setLength(0);
        log.append("Invalid Command");
        return false;
    }

    public double getBalance(){
        log.setLength(0);
        log.append("Invalid Command");
        return -1;
    }

    public boolean deposit(double amount){
        log.setLength(0);
        log.append("Invalid Command");
        return false;
    }

    public boolean withdraw(double amount){
        log.setLength(0);
        log.append("Invalid Command");
        return false;
    }

    public boolean requestLoan(double amount){
        log.setLength(0);
        log.append("Invalid Command");
        return false;
    }
}
