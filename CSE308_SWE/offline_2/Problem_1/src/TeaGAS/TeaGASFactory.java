package TeaGAS;

import TeaGAS.Builders.*;

public class TeaGASFactory {
    public TeaGASFactory(){}

    public TeaGAS getTeaGAS(String packageName, String internetConnection, String framework){
        TeaGASBuilder teaGASBuilder;
        switch(packageName){
            case "Silver":{
                teaGASBuilder = new ATMega32TeaGASBuilder();
                teaGASBuilder.setWeightMeasurement("Load Sensor");
                break;
            }
            case "Gold":{
                teaGASBuilder = new ArduinoMegaTeaGASBuilder();
                teaGASBuilder.setWeightMeasurement("Weight Module");
                break;
            }
            case "Platinum":{
                teaGASBuilder = new RaspberryPiTeaGASBuilder();
                teaGASBuilder.setWeightMeasurement("Load Sensor");
                break;
            }
            case "Diamond":{
                teaGASBuilder = new RaspberryPiTeaGASBuilder();
                teaGASBuilder.setWeightMeasurement("Weight Module");
                break;
            }
            default:{
                return null;
            }
        }

        teaGASBuilder.setIdentification();
        teaGASBuilder.setStorage();
        teaGASBuilder.setDisplay();
        teaGASBuilder.setInternetConnection(internetConnection);
        teaGASBuilder.setController();
        teaGASBuilder.setFrameWork(framework);

        return teaGASBuilder.getTeaGAS();
    }
}
