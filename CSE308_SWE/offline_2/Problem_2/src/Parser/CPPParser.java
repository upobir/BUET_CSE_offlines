package Parser;

public class CPPParser implements Parser{

    public CPPParser() { }

    @Override
    public void parse(String filename) {
        System.out.println("Parsing file " + filename + " in CPP");
        
    }

    @Override
    public String getParserName() {
        return "CPP Parser";
    }
    
}
