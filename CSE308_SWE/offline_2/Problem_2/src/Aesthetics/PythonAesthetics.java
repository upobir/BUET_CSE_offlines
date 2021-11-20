package Aesthetics;

public class PythonAesthetics implements Aesthetics{
    private String font;
    private String style;
    private String color;

    public PythonAesthetics() { 
        font = "Consolas";
        style = "Normal";
        color = "Blue";
    }

    @Override
    public String getFont() {
        return font;
    }

    @Override
    public String getStyle() {
        return style;
    }

    @Override
    public String getColor() {
        return color;
    }
    
}
