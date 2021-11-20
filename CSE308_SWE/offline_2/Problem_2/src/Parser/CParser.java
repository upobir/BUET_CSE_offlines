package Parser;

public class CParser implements Parser{

    public CParser() { }

    @Override
    public void parse(String filename) {
        System.out.println("Parsing file " + filename + " in C");
        
    }

    @Override
    public String getParserName() {
        return "C parser";
    }
    
}
