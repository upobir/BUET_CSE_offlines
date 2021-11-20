package FoodOrder.DrinkDecorator;

import FoodOrder.*;

public abstract class DrinkDecorator extends FoodOrderDecorator{
    

    public DrinkDecorator(FoodOrder _foodOrder) {
        super(_foodOrder);
    }

    protected abstract double getDrinkPrice();

    protected abstract String getDrinkDescription();

    @Override
    public double getPrice() {
        double price = foodOrder.getPrice();
        price += getDrinkPrice();
        return price;
    }

    @Override
    public String getDescription() {
        String description = foodOrder.getDescription();
        description += ", " + getDrinkDescription();
        return description;
    }
}
