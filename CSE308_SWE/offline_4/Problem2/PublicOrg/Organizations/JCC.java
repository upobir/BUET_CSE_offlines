package PublicOrg.Organizations;

import java.util.HashMap;
import java.util.LinkedList;
import java.util.Map;
import java.util.Queue;

import PublicOrg.Colleague;
import PublicOrg.Mediator;

public class JCC extends Mediator {
    
    HashMap<Colleague, Queue<Colleague>> queues;

    public JCC() {
        super();
        this.queues = new HashMap<>();
    }

    @Override
    public void addColleague(Colleague colleague) {
        queues.put(colleague, new LinkedList<>());
    }

    @Override
    public boolean request(Colleague colleague, String service) {
        if(colleague == null)
            return false;

        for(Map.Entry<Colleague, Queue<Colleague>> pair: queues.entrySet()){
            if(pair.getKey().getServices().equals(service)){
                pair.getValue().add(colleague);
                return true;
            }
        }

        return false;
    }

    @Override
    public void serve(Colleague colleague) {
        if(colleague == null)
            return;

        Queue<Colleague> queue = queues.get(colleague);
        Colleague other = queue.poll();
        colleague.serve(other);
    }

    public Colleague getByName(String name){
        for(Colleague c : queues.keySet()){
            if(c.getName().equals(name))
                return c;
        }
        return null;
    }

}
