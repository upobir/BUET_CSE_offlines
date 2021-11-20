package FoodOrder;

import FoodOrder.PizzaItem.*;
import FoodOrder.AppetizerDecorator.*;
import FoodOrder.DrinkDecorator.*;

public class FoodOrderBuilder {
    private FoodOrder foodOrder;

    public FoodOrderBuilder(){
        clearOrder();
    }

    public FoodOrder getFoodOrder(){
        return foodOrder;
    }

    public void clearOrder(){
        foodOrder = null;
    }

    public void addPizza(String pizzaName){
        if(foodOrder != null) return;
        switch(pizzaName.toLowerCase()){
            case "beef":
                foodOrder = new BeefPizza();
                break;
            case "veggi":
                foodOrder = new VeggiPizza();
                break;
            default:
        }
        return;
    }

    public void addAppetizer(String appetizerName){
        if(foodOrder == null) return;

        switch(appetizerName.toLowerCase()){
            case "onion rings":
                foodOrder = new OnionRingsDecorator(foodOrder);
                break;
            case "french fries":
                foodOrder = new FrenchFriesDecorator(foodOrder);
                break;
            default:
        }
        return;
    }

    public void addDrink(String drinkName){
        if(foodOrder == null) return;
        switch(drinkName.toLowerCase()){
            case "coffee":
                foodOrder = new CoffeeDecorator(foodOrder);
                break;
            case "coke":
                foodOrder = new CokeDecorator(foodOrder);
                break;
            default:
        }
        return;
    }
}
