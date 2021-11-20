package PublicOrg;

public abstract class Colleague {

    protected Mediator mediator;
    protected String name;
    protected String services;

    public Colleague(String name, String services, Mediator mediator){
        this.name = name;
        this.mediator = mediator;
        this.services = services;
    }

    public String getName() {
        return name;
    }

    public String getServices() {
        return services;
    }

    public void request(String service){
        if(mediator.request(this, service))
            System.out.println(getName() + " request for " + service + " service");
    }

    public void serve(Colleague other) {
        if(other == null){
            System.out.println("No requrests remain");
        }
        else{
            System.out.println(getName() + " serves the request for " + other.getName());
        }
    }

    public void serve() {
        mediator.serve(this);
    }
}
