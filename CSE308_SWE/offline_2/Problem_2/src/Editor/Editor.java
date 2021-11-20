package Editor;

import Aesthetics.Aesthetics;
import Editor.Factories.*;
import Parser.Parser;

public class Editor {
    static private Editor singletonEditor = new Editor();
    private String filename;
    private Parser parser;
    private Aesthetics aesthetics;

    protected Editor(){ 
        filename = null;
    }

    public static Editor getInstance(){
        return singletonEditor;
    }

    private String extractExtension(){
        String parts[] = filename.split("\\.");
        if(parts.length <= 1)
            return null;
        return parts[parts.length - 1];
    }

    

    public boolean openFile(String _filename){
        filename = _filename;
        String extension = extractExtension();

        LanguageFactory factory = LanguageFactory.getLanguageFactory(extension);
        if(factory == null){
            filename = null;
            return false;
        }

        parser = factory.getParser();
        aesthetics = factory.getAesthetics();

        return true;
    }

    public String getFilename(){
        return filename;
    }

    public String getParserName(){
        if(filename == null)
            return null;
        return parser.getParserName();
    }

    public String getFont(){
        if(filename == null)
            return null;
        return aesthetics.getFont();
    }

    public String getStyle(){
        if(filename == null)
            return null;
        return aesthetics.getStyle();
    }

    String getColor(){
        if(filename == null)
            return null;
        return aesthetics.getColor();
    }

}
