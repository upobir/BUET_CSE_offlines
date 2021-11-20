package FoodOrder.AppetizerDecorator;

import FoodOrder.*;

public abstract class AppetizerDecorator extends FoodOrderDecorator{

    AppetizerDecorator(FoodOrder _foodOrder) {
        super(_foodOrder);
    }

    protected double getAppetizerPrice(){
        return 100.00;
    }

    protected abstract String getAppetizerDescription();

    @Override
    public double getPrice() {
        double price = foodOrder.getPrice();
        price += getAppetizerPrice();
        return price;
    }

    @Override
    public String getDescription() {
        String description = foodOrder.getDescription();
        description += ", " + getAppetizerDescription();
        return description;
    }

    
    
}
