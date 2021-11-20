package TeaGAS;

public abstract class TeaGASBuilder {
    protected TeaGAS teaGAS;
    
    public TeaGASBuilder(){
        constructTeaGAS();
    }

    public abstract void constructTeaGAS();

    public void setWeightMeasurement(String weightMeasurement){
        if(teaGAS != null){
            if(!teaGAS.setWeightMeasurement(weightMeasurement)){
                teaGAS = null;
            }
        }
    }  

    public void setIdentification(){
        if(teaGAS != null){
            if(!teaGAS.setIdentification()){
                teaGAS = null;
            }
        }
    }

    public void setStorage(){
        if(teaGAS != null){
            if(!teaGAS.setStorage()){
                teaGAS = null;
            }
        }
    }

    public void setDisplay(){
        if(teaGAS != null){
            if(!teaGAS.setDisplay()){
                teaGAS = null;
            }
        }
    }

    public void setInternetConnection(String internetConnection){
        if(teaGAS != null){
            if(!teaGAS.setInternetConnection(internetConnection)){
                teaGAS = null;
            }
        }
    }

    public void setController(){
        if(teaGAS != null){
            if(!teaGAS.setController()){
                teaGAS = null;
            }
        }
    }

    public void setFrameWork(String framework){
        if(teaGAS != null){
            if(!teaGAS.setFrameWork(framework)){
                teaGAS = null;
            }
        }
    }

    public TeaGAS getTeaGAS(){
        return teaGAS;
    }
}
