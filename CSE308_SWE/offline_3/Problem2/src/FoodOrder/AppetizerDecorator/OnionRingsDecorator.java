package FoodOrder.AppetizerDecorator;

import FoodOrder.FoodOrder;

public class OnionRingsDecorator extends AppetizerDecorator {

    public OnionRingsDecorator(FoodOrder _foodOrder) {
        super(_foodOrder);
    }

    @Override
    protected String getAppetizerDescription() {
        return "Onion Rings";
    }
    
}
