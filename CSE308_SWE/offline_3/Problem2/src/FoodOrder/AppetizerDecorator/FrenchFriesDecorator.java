package FoodOrder.AppetizerDecorator;

import FoodOrder.FoodOrder;

public class FrenchFriesDecorator extends AppetizerDecorator{

    public FrenchFriesDecorator(FoodOrder _foodOrder) {
        super(_foodOrder);
    }

    @Override
    protected String getAppetizerDescription() {
        return "French Fries";
    }
    
}
