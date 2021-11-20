package Editor.Factories;

import Aesthetics.Aesthetics;
import Parser.Parser;

public interface LanguageFactory {
    
    public Parser getParser();

    public Aesthetics getAesthetics();

    static public LanguageFactory getLanguageFactory(String extension){
        if(extension == null)
            return null;
        switch(extension){
            case "c":{
                return new CFactory();
            }
            case "cpp":{
                return new CPPFactory();
            }
            case "py":{
                return new PythonFactory();
            }
            default:{
                return null;
            }
        }
    }
}
