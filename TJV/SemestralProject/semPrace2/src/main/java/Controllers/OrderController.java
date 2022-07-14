package Controllers;

import Entities.Order;
import javax.ejb.Stateless;

@Stateless
public class OrderController extends AbstractCRUDController<Order> {

    public OrderController() {
        super(Order.class);
    }

    @Override
    protected Object getEntityId(Order e) {
        return e.getId();
    }
}
