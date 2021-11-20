package Editor.Factories;

import Aesthetics.*; 
import Parser.*;

public class PythonFactory implements LanguageFactory{

    public PythonFactory() { }

    @Override
    public Parser getParser() {
        return new PythonParser();
    }

    @Override
    public Aesthetics getAesthetics() {
        return new PythonAesthetics();
    }

}
