package Aesthetics;

public class CAesthetics implements Aesthetics{
    private String font;
    private String style;
    private String color;

    public CAesthetics() {
        font = "Courier New";
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
