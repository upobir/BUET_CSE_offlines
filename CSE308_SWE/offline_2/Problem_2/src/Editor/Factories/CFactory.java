package Editor.Factories;

import Aesthetics.*;
import Parser.*;

public class CFactory implements LanguageFactory{

    public CFactory() { }

    @Override
    public Parser getParser() {
        return new CParser();
    }

    @Override
    public Aesthetics getAesthetics() {
        return new CAesthetics();
    }
    
}
