package src.common;

import java.util.Date;

public class Logger {
    private boolean activated = true;


    private Logger(){
    }


    public void flip(){
        activated = !activated;
    }

    
    public void log(String msg){
        synchronized(this){
            if(activated){
                Date date = new Date();
                System.err.println("[ "+date.toString()+" ] " + msg);
            }
        }
    }


    public static Logger logger = null;


    public static Logger getLogger(){
        if(logger == null){
            logger = new Logger();
        }

        return logger;
    }
}
