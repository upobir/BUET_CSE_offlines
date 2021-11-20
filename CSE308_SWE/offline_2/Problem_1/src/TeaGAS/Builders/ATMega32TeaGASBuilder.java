package TeaGAS.Builders;

import TeaGAS.TeaGASBuilder;
import TeaGAS.Systems.ATMega32TeaGAS;

public class ATMega32TeaGASBuilder extends TeaGASBuilder{

    public ATMega32TeaGASBuilder(){
        super();
    }

    @Override
    public void constructTeaGAS() {
        teaGAS = new ATMega32TeaGAS();
    }

}