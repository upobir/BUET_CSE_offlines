package Editor.Factories;

import Aesthetics.*;
import Parser.*;

public class CPPFactory implements LanguageFactory{

    public CPPFactory() {}

    @Override
    public Parser getParser() {
        return new CPPParser();
    }

    @Override
    public Aesthetics getAesthetics() {
        return new CPPAesthetics();
    }
}
