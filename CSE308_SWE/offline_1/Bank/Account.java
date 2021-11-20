package Bank;

public abstract class Account extends User{
    private final static double LOAN_INTEREST = 10;

    protected double balance, loan;
    protected int maturity;

    public Account(String name, double deposit, Bank bank){
        super(name, bank);
        this.balance = deposit;
        this.loan = 0;
        this.maturity = 0;
    }

    @Override
    public double getBalance(){
        log.setLength(0);

        log.append("Current balance: "+balance+"$. ");
        if(loan > 0){
            log.append("Loan " + loan + "$. ");
        }
        return balance;
    }

    @Override
    public boolean deposit(double amount){
        log.setLength(0);

        balance += amount;
        log.append(amount + "$ deposited; current balance: " + balance + "$ ");
        return true;
    }

    @Override
    public boolean withdraw(double amount){
        log.setLength(0);

        if(balance < amount){
            log.append("Cannot withdraw more than balance. ");
            return false;
        }
        balance -= amount;
        log.append(amount + "$ withdrew; current balance: " + balance + "$. ");
        return true;
    }

    @Override
    public boolean requestLoan(double amount){
        log.setLength(0);

        if(amount + loan > getMaxLoan()){
            log.append("Total loan cannot be more than " + getMaxLoan() + "$. ");
            return false;
        }
        boolean result = bank.requestLoan(this, amount);
        log.append(bank.getLog());
        return result;
    }

    abstract double getInterestRate();

    abstract double getMaxLoan();

    void addLoan(double amount){
        balance += amount;
        loan += amount;
    }

    void increment(){
        maturity++;
        double change = ( getInterestRate() * balance - LOAN_INTEREST * loan ) / 100.0;
        balance += change;
    }
}

final class SavingsAccount extends Account{
    private final static double MIN_REMAINING_BALANCE = 1000;
    private final static double MAX_LOAN = 10000;
    private static double ACCOUNT_INTEREST = 10;

    static Account createAccount(String name, double deposit, Bank bank, StringBuffer log){
        return new SavingsAccount(name, deposit, bank);
    }
    
    static void changeInterestRate(double rate){
        ACCOUNT_INTEREST = rate;
    }

    public SavingsAccount(String name, double deposit, Bank bank) {
        super(name, deposit, bank);
    }

    @Override
    public boolean withdraw(double amount) {
        log.setLength(0);
        if(balance - amount < MIN_REMAINING_BALANCE){
            log.append("Savings account must have at least " + MIN_REMAINING_BALANCE + "$ after withdrawing. ");
            return false;
        }
        return super.withdraw(amount);
    }

    @Override
    double getInterestRate() {
        return ACCOUNT_INTEREST;
    }

    @Override
    double getMaxLoan() {
        return MAX_LOAN;
    }

}

final class StudentAccount extends Account{
    private static final double MAX_WITHDRAW = 10000;
    private final static double MAX_LOAN = 1000;
    private static double ACCOUNT_INTEREST = 5;

    static Account createAccount(String name, double deposit, Bank bank, StringBuffer log){
        return new StudentAccount(name, deposit, bank);
    }

    static void changeInterestRate(double rate){
        ACCOUNT_INTEREST = rate;
    }

    public StudentAccount(String name, double deposit, Bank bank) {
        super(name, deposit, bank);
    }

    @Override
    public boolean withdraw(double amount) {
        log.setLength(0);

        if(amount > MAX_WITHDRAW){
            log.append("More than " + MAX_WITHDRAW + "$ cannot be withdrew from student account. ");
            return false;
        }
        return super.withdraw(amount);
    }

    @Override
    double getInterestRate() {
        return ACCOUNT_INTEREST;
    }

    @Override
    double getMaxLoan() {
        return MAX_LOAN;
    }
}

final class FixedDepositAccount extends Account{
    private final static double MIN_INITIAL_DEPOSIT = 100000;
    private final static double MIN_DEPOSIT = 50000;
    private final static int MIN_MATURITY = 1;
    private final static double MAX_LOAN = 100000;
    private static double ACCOUNT_INTEREST = 15;

    static Account createAccount(String name, double deposit, Bank bank, StringBuffer log){
        if(deposit < MIN_INITIAL_DEPOSIT){
            log.append("Account could not be created, Fixed Deposit accounts must have at least " + MIN_INITIAL_DEPOSIT + "$ balance. ");
            return null;
        }
        return new FixedDepositAccount(name, deposit, bank);
    }

    static void changeInterestRate(double rate){
        ACCOUNT_INTEREST = rate;
    }

    public FixedDepositAccount(String name, double deposit, Bank bank) {
        super(name, deposit, bank);
    }

    @Override
    public boolean deposit(double amount){
        log.setLength(0);

        if(amount < MIN_DEPOSIT){
            log.append("Deposit into Fixed Deposit accounts must be at least " + MIN_DEPOSIT + "$. ");
            return false;
        }
        return super.deposit(amount);
    }

    @Override
    public boolean withdraw(double amount) {
        log.setLength(0);
        
        if(maturity < 1){
            log.append("Fixed Deposit Account cannot withdraw before maturity period " + MIN_MATURITY + " years. ");
            return false;
        }
        return super.withdraw(amount);
    }

    @Override
    double getInterestRate() {
        return ACCOUNT_INTEREST;
    }

    @Override
    double getMaxLoan() {
        return MAX_LOAN;
    }
}