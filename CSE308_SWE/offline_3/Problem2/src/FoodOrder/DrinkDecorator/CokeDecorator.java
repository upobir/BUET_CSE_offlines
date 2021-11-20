package FoodOrder.DrinkDecorator;

import FoodOrder.FoodOrder;

public class CokeDecorator extends DrinkDecorator{

    public CokeDecorator(FoodOrder _foodOrder) {
        super(_foodOrder);
    }

    @Override
    protected double getDrinkPrice() {
        return 30.00;
    }

    @Override
    protected String getDrinkDescription() {
        return "Coke";
    }
    
}
