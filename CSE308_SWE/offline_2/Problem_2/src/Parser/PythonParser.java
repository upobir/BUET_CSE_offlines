package Parser;

public class PythonParser implements Parser{

    public PythonParser() { }

    @Override
    public void parse(String filename) {
        System.out.println("Parsing file " + filename + " in Python");
    }

    @Override
    public String getParserName() {
        return "Python Parser";
    }
    
}
