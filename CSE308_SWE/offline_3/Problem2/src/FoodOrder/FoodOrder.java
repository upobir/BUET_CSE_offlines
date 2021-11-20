package FoodOrder;

import java.io.PrintStream;

public interface FoodOrder {
    public double getPrice();
    public String getDescription();

    public default void serve(PrintStream printStream){
        printStream.println(getDescription() + " served.");
        return;
    }
}
