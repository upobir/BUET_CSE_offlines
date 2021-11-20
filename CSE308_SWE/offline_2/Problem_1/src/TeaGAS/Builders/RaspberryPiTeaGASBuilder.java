package TeaGAS.Builders;

import TeaGAS.TeaGASBuilder;
import TeaGAS.Systems.RaspberryPiTeaGAS;

public class RaspberryPiTeaGASBuilder extends TeaGASBuilder{

    public RaspberryPiTeaGASBuilder(){
        super();
    }

    @Override
    public void constructTeaGAS() {
        teaGAS = new RaspberryPiTeaGAS();
    }
    
}
