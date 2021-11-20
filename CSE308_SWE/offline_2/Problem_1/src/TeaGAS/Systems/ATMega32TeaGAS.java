package TeaGAS.Systems;

import TeaGAS.TeaGAS;

public class ATMega32TeaGAS extends TeaGAS{

    public ATMega32TeaGAS() {
        super("ATMega32");
    }

    @Override
    protected boolean setIdentification() {
        this.identification = "RFID card";
        return true;
    }

    @Override
    protected boolean setStorage() {
        this.storage = "SD card";
        return true;
    }

    @Override
    protected boolean setDisplay() {
        this.display = "LCD";    
        return true;
    }

    @Override
    protected boolean setController() {
        this.controller = "Buttons";
        return true;
    }

    @Override
    protected boolean setInternetConnection(String internetConenciton){
        if(internetConenciton.equals("Ethernet"))
            return false;
        return super.setInternetConnection(internetConenciton);
    }
    
}
