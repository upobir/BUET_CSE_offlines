package FoodOrder.PizzaItem;

import FoodOrder.FoodOrder;

public abstract class Pizza implements FoodOrder {

    abstract public String getName();

    @Override
    public String getDescription() {
        return getName();
    }    
}
