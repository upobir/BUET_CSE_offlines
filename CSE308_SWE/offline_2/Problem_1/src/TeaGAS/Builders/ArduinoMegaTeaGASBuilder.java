package TeaGAS.Builders;

import TeaGAS.TeaGASBuilder;
import TeaGAS.Systems.ArduinoMegaTeaGAS;

public class ArduinoMegaTeaGASBuilder extends TeaGASBuilder{

    public ArduinoMegaTeaGASBuilder(){
        super();
    }

    @Override
    public void constructTeaGAS() {
        teaGAS = new ArduinoMegaTeaGAS();
    }
    
}
