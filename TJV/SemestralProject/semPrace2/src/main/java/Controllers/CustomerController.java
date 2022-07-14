package Controllers;

import Entities.Contact;
import Entities.Customer;
import Entities.Order;
import java.util.Objects;
import javax.ejb.Stateless;

@Stateless
public class CustomerController extends AbstractCRUDController<Customer> {

    public CustomerController() {
        super(Customer.class);
    }

    @Override
    protected Object getEntityId(Customer e) {
        return e.getId();
    }

    public Customer addOrder(Long id, Order o) {
        Customer c = retrieveEntity(id);
        c.getOrders().add(Objects.requireNonNull(o));
        updateOrCreate(c);
        return c;
    }

    public Customer removeOrder(Long id, Order o) {
        Customer c = retrieveEntity(id);
        c.getOrders().remove(Objects.requireNonNull(o));
        updateOrCreate(c);
        return c;
    }

    public Customer addContact(Long id, Contact c) {
        Customer cus = retrieveEntity(id);
        cus.getContacts().add(Objects.requireNonNull(c));
        updateOrCreate(cus);
        return cus;
    }

    public Customer removeContact(Long id, Contact c) {
        Customer cus = retrieveEntity(id);
        cus.getContacts().remove(Objects.requireNonNull(c));
        updateOrCreate(cus);
        return cus;
    }
}
