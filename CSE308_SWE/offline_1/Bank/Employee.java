package Bank;

public abstract class Employee extends User{

    public Employee(String name, Bank bank){
        super(name, bank);
    }

    @Override
    public double lookup(String name){
        log.setLength(0);

        double result = bank.lookup(name);
        log.append(bank.getLog());
        return result;
    }

    boolean canApproveLoan(){
        return true;
    }

    @Override
    public boolean approveLoan(){
        log.setLength(0);

        if(canApproveLoan()){
            boolean result = bank.approveLoan();
            log.append(bank.getLog());
            return result;
        }
        else{
            log.append("No permission");
            return false;
        }   
    }

    @Override
    public boolean changeInterestRate(String type, double rate){
        log.setLength(0);
        log.append("No permission");
        return false;
    }

    @Override
    public double getFund() {
        log.setLength(0);
        log.append("No permission. ");
        return 0;
    }
}

final class ManagingDirector extends Employee {

    public ManagingDirector(String name, Bank bank) {
        super(name, bank);
    }

    @Override
    public double getFund() {
        log.setLength(0);

        double fund = bank.getFund();
        log.append("Current funds: " + fund + "$. ");
        return fund;
    }

    @Override
    public boolean changeInterestRate(String type, double rate) {
        log.setLength(0);

        switch(type){
            case "FIXED_DEPOSIT":
                FixedDepositAccount.changeInterestRate(rate);
                break;
            case "SAVINGS":
                SavingsAccount.changeInterestRate(rate);
                break;
            case "STUDENT":
                StudentAccount.changeInterestRate(rate);
                break;
            /// NOTE default?
        }
        log.append("Interest rate for '" + type + "' changed to "+rate+"%. ");
        return true;
    }
}

final class Officer extends Employee {

    public Officer(String name, Bank bank) {
        super(name, bank);
    }

}

final class Cashier extends Employee {

    public Cashier(String name, Bank bank) {
        super(name, bank);
    }

    @Override
    boolean canApproveLoan() {
        return false;
    }
}