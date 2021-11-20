package TeaGAS.Systems;
import TeaGAS.TeaGAS;

public class RaspberryPiTeaGAS extends TeaGAS{

    public RaspberryPiTeaGAS() {
        super("Raspberry Pi");
    }

    @Override
    protected boolean setIdentification() {
        this.identification = "NFC card";
        return true;
    }

    @Override
    protected boolean setStorage() {
        this.storage = "Default";
        return true;
    }

    @Override
    protected boolean setDisplay() {
        this.display = "Touch Screen";
        return true;
    }

    @Override
    protected boolean setController() {
        this.controller = "Touch Screen";
        return true;
    }
    
}
