package server;

public interface SubjectInterface {
    public void attach(ObserverInterface o);

    public void detach(ObserverInterface o);

    public void notifyObservers();
}
