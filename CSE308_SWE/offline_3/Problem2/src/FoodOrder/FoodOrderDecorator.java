package FoodOrder;

public abstract class FoodOrderDecorator implements FoodOrder{
    protected FoodOrder foodOrder;

    public FoodOrderDecorator(FoodOrder _foodOrder){
        foodOrder = _foodOrder;
    }
}
