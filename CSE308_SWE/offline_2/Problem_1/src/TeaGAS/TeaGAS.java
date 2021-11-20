package TeaGAS;

import java.io.PrintStream;

public abstract class TeaGAS {
    private String microcontroller;
    protected String weightMeasurement;
    protected String identification;
    protected String storage;
    protected String display;
    protected String internetConenciton;
    protected String controller;
    protected String framework;

    protected TeaGAS(String microcontroller){
        this.microcontroller = microcontroller;
    }

    public void describe(PrintStream stream){
        stream.println("TeaGAS system Description:");
        stream.println("Microcontroller: " + microcontroller);
        stream.println("Weight Mesurement: " + weightMeasurement);
        stream.println("Identification: " + identification);
        stream.println("Storage: " + storage);
        stream.println("Display: " + display);
        stream.println("Internet Connection: " + internetConenciton);
        stream.println("Controller: " + controller);
        stream.println("Framework: " + framework);
    }

    boolean setWeightMeasurement(String weightMeasurement){
        if(!weightMeasurement.equals("Load Sensor") && !weightMeasurement.equals("Weight Module"))
            return false;
        this.weightMeasurement = weightMeasurement;
        return true;
    }

    protected abstract boolean setIdentification();

    protected abstract boolean setStorage();

    protected abstract boolean setDisplay();

    protected boolean setInternetConnection(String internetConnection){
        if(!internetConnection.equals("WiFi") && !internetConnection.equals("GSM") && !internetConnection.equals("Ethernet")){
            return false;
        }
        this.internetConenciton = internetConnection;
        return true;
    }

    protected abstract boolean setController();

    protected boolean setFrameWork(String framework){
        if(!framework.equals("Django") && !framework.equals("Spring") && !framework.equals("Laravel"))
            return false;
        this.framework = framework;
        return true;
    }

    /// usage functions
    public void useMicrocontroller(PrintStream stream){
        stream.println("Using microcontroller " + microcontroller);
    }

    public void useWeightMeasurement(PrintStream stream){
        stream.println("Using Weight Measurement via " + weightMeasurement);
    }

    public void useIdentification(PrintStream stream){
        stream.println("Using Identification via " + identification);
    }

    public void useStorage(PrintStream stream){
        stream.println("Using storage " + storage);
    }

    public void useDisplay(PrintStream stream){
        stream.println("Using display " + display);
    }

    public void useInternetConnection(PrintStream stream){
        stream.println("Using Internet Connection " + internetConenciton);
    }

    public void useController(PrintStream stream){
        stream.println("Using Controller " + controller);
    }

    public void useFramework(PrintStream stream){
        stream.println("Using framework " + framework);
    }
}
