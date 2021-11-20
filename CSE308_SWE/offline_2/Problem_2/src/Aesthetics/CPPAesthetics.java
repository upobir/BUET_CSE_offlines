package Aesthetics;

public class CPPAesthetics implements Aesthetics{
    private String font;
    private String style;
    private String color;

    public CPPAesthetics() {
        font = "Monaco";
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
