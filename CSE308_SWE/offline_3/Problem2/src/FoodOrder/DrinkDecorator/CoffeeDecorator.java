package FoodOrder.DrinkDecorator;

import FoodOrder.FoodOrder;

public class CoffeeDecorator extends DrinkDecorator {

    public CoffeeDecorator(FoodOrder _foodOrder) {
        super(_foodOrder);
    }

    @Override
    protected double getDrinkPrice() {
        return 60.00;
    }

    @Override
    protected String getDrinkDescription() {
        return "Coffee";
    }
    
}
