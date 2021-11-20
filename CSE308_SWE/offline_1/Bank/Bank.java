package Bank;

import java.util.ArrayList;
import java.util.HashMap;

final public class Bank {
    private class Loan{
        Account account;
        double amount;

        public Loan(Account account, double amount) {
            this.account = account;
            this.amount = amount;
        }
    }

    private double fund;
    private int time;
    private HashMap<String, Account> accounts;
    private HashMap<String, Employee> employees;
    private ArrayList<Loan> loans;
    private StringBuffer log;

    public Bank(double fund){
        this.fund = fund;
        this.time = 0;
        this.accounts = new HashMap<>();
        this.employees = new HashMap<>();
        this.loans = new ArrayList<>();
        this.log = new StringBuffer("Bank created. ");
    }

    public String getLog(){
        return new String(log);
    }

    public int getTime() {
        return time;
    }

    double getFund(){
        return fund;
    }

    public User createEmployee(String name, String type){
        log.setLength(0);
        Employee employee = null;

        if(employees.containsKey(name)){
            log.append("Employee with the name '" + name + "' already exists.");
            return employee;
        }

        switch(type){
            case "MANAGING DIRECTOR":
                employee = new ManagingDirector(name, this);
                break;
            case "OFFICER":
                employee = new Officer(name, this);
                break;
            case "CASHIER":
                employee = new Cashier(name, this);
                break;
            /// default?
        }

        if(employee != null){
            log.append(name + " created. ");
            employees.put(name, employee);
        }

        return employee;
    }

    public User createAccount(String name, String type, Double deposit){
        log.setLength(0);
        Account account = null;

        if(accounts.containsKey(name)){
            log.append("Account with the name '" + name + "' already exists. ");
            return account;
        }

        switch(type){
            case "SAVINGS":
                account = SavingsAccount.createAccount(name, deposit, this, log);
                break;
            case "STUDENT":
                account = StudentAccount.createAccount(name, deposit, this, log);
                break;
            case "FIXED DEPOSIT":
                account = FixedDepositAccount.createAccount(name, deposit, this, log);
                break;
            // NOTE dafault
        }

        if(account != null){
            log.append(type + " account for " + name + " created: intial balance " + deposit + "$ ");
            accounts.put(name, account);
        }

        return account;
    }

    public User login(String name){
        log.setLength(0);

        if(accounts.containsKey(name)){
            log.append("Welcome back, " + name);
            return accounts.get(name);
        }
        else if(employees.containsKey(name)){
            Employee employee = employees.get(name);
            log.append(name + " active.");

            if(!loans.isEmpty() && employee.canApproveLoan()){
                log.append(" There are " + loans.size() + " loan(s) active.");
            }
            return employee;
        }
        else{
            log.append("No such account");
            return null;
        }
    }

    boolean requestLoan(Account account, double amount){
        log.setLength(0);

        double totalLoan = 0;
        for(Loan l : loans){
            totalLoan += l.amount;
        }
        totalLoan += amount;

        if(totalLoan >= fund){
            log.append("Loan request denined");
            return false;
        }
        else{
            loans.add(new Loan(account, amount));
            log.append("Loan request successful, sent for approval");
            return true;
        }
    }

    public void increment(){
        log.setLength(0);

        time++;
        
        for(Account account : accounts.values()){
            account.increment();
        }
        log.append(time + " year(s) passed");
    }

    double lookup(String name) {
        log.setLength(0);

        if(!accounts.containsKey(name)){
            log.append("No such account found.");
            return -1;
        }

        Account account = accounts.get(name);
        double balance = account.getBalance();
        log.append("Current balance of "+ name + " : " + balance + "$");

        return balance;
    }

    boolean approveLoan() {
        log.setLength(0);

        if(loans.isEmpty()){
            log.append("No loans pending for approval");
            return false;
        }

        log.append("Loan approved for ");
        for(int i = 0; i<loans.size(); i++){
            Loan loan = loans.get(i);

            fund -= loan.amount;
            loan.account.addLoan(loan.amount);

            if(i != 0) log.append(", ");
            log.append(loan.account.getName());
        }

        loans.clear();
        log.append(". ");
        return true;
    }
}
