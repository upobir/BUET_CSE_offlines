package PublicOrg;

public abstract class Mediator {

    public Mediator(){
    }

    abstract public void addColleague(Colleague colleague);

    abstract public boolean request(Colleague colleague, String service);

    abstract public void serve(Colleague colleague);
}
